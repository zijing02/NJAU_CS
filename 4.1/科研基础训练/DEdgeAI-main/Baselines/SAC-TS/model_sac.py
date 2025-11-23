import numpy as np
import torch
import torch.nn.functional as F
import collections
import random


class ReplayBuffer:
    def __init__(self, capacity):
        self.buffer = collections.deque(maxlen=capacity)

    def add(self, state, action, reward, next_state):
        self.buffer.append((state, action, reward, next_state))

    def sample(self, batch_size):
        transitions = random.sample(self.buffer, batch_size)
        state, action, reward, next_state = zip(*transitions)
        return np.array(state), action, reward, np.array(next_state)

    def size(self):
        return len(self.buffer)

    def clear(self):
        self.buffer.clear()


class PolicyNet(torch.nn.Module):
    def __init__(self, state_dim, hidden_dim, action_dim):
        super(PolicyNet, self).__init__()
        self.fc1 = torch.nn.Linear(state_dim, hidden_dim)
        self.fc2 = torch.nn.Linear(hidden_dim, hidden_dim)
        self.fc3 = torch.nn.Linear(hidden_dim, action_dim)

    def forward(self, x):
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        return F.softmax(self.fc3(x), dim=1)


class QValueNet(torch.nn.Module):
    ''' 只有一层隐藏层的Q网络 '''

    def __init__(self, state_dim, hidden_dim, action_dim):
        super(QValueNet, self).__init__()
        self.fc1 = torch.nn.Linear(state_dim, hidden_dim)
        self.fc2 = torch.nn.Linear(hidden_dim, hidden_dim)
        self.fc3 = torch.nn.Linear(hidden_dim, action_dim)

    def forward(self, x):
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        return self.fc3(x)


class SAC:
    ''' 处理离散动作的SAC算法 '''

    def __init__(self, state_dim, hidden_dim, action_dim,  actor_lr, critic_lr,
                 alpha, alpha_lr, target_entropy, tau, gamma, device):
        # Define the policy actor networks
        self.actor = PolicyNet(state_dim, hidden_dim, action_dim).to(device)
        # Define the critic1 (Q0) networks
        self.critic_1 = QValueNet(state_dim, hidden_dim, action_dim).to(device)
        # Define the critic2 (Q1) networks
        self.critic_2 = QValueNet(state_dim, hidden_dim, action_dim).to(device)

        self.target_1 = QValueNet(state_dim, hidden_dim, action_dim).to(device)  # # Define the target1 (V) networks
        self.target_2 = QValueNet(state_dim, hidden_dim, action_dim).to(device)  # Define the target2 (target v) networks

        # Define the initial parameters of the target 1 and 2 networks that are the same with Q0 and Q1 networks
        self.target_1.load_state_dict(self.critic_1.state_dict())
        self.target_2.load_state_dict(self.critic_2.state_dict())

        # Define the optimizers of three networks
        self.actor_optimizer = torch.optim.Adam(self.actor.parameters(), lr=actor_lr)
        self.critic_1_optimizer = torch.optim.Adam(self.critic_1.parameters(), lr=critic_lr)
        self.critic_2_optimizer = torch.optim.Adam(self.critic_2.parameters(), lr=critic_lr)

        # 使用alpha (the temperature of action entropy regularization)的log值,可以使训练结果比较稳定
        self.log_alpha = torch.tensor(np.log(alpha), dtype=torch.float)
        self.log_alpha.requires_grad = True  # 可以对alpha求梯度
        self.log_alpha_optimizer = torch.optim.Adam([self.log_alpha], lr=alpha_lr)
        self.target_entropy = target_entropy  # 目标熵的大小
        self.gamma = gamma
        self.tau = tau
        self.device = device
        self.history_loss = []

    def take_action(self, state):
        state = torch.tensor(np.array([state]), dtype=torch.float).to(self.device)
        probs = self.actor(state)
        action_dist = torch.distributions.Categorical(probs)
        action = action_dist.sample()  # Get the index tensor of the maximal probability
        return action.item()  # Get the index

    # Calulcate the target Q values
    # Using the actor output, V output, and target V output with the input of reward and next state.
    def calc_target_q(self, rewards, next_states):
        next_probs = self.actor(next_states)
        next_log_probs = torch.log(next_probs + 1e-8)  # 1e-8 is used to ensure the definition sense of log function

        entropy = -torch.sum(next_probs * next_log_probs, dim=1, keepdim=True)

        target1_q1_value = self.target_1(next_states)
        target2_q2_value = self.target_2(next_states)

        min_q_value = torch.sum(next_probs * torch.min(target1_q1_value, target2_q2_value), dim=1, keepdim=True)
        next_value = min_q_value + self.log_alpha.exp() * entropy
        # q_target = rewards + self.gamma * next_value * (1 - dones)
        q_target = rewards + self.gamma * next_value
        return q_target

    def soft_update(self, net, target_net):
        for param_target, param in zip(target_net.parameters(), net.parameters()):
            param_target.data.copy_(param_target.data * (1.0 - self.tau) + param.data * self.tau)

    def update(self, transition_dict):
        states = torch.tensor(transition_dict['states'], dtype=torch.float).to(self.device)
        actions = torch.tensor(transition_dict['actions']).view(-1, 1).to(self.device)  # 动作不再是float类型
        rewards = torch.tensor(transition_dict['rewards'], dtype=torch.float).view(-1, 1).to(self.device)
        next_states = torch.tensor(transition_dict['next_states'], dtype=torch.float).to(self.device)
        # dones = torch.tensor(transition_dict['dones'], dtype=torch.float).view(-1, 1).to(self.device)

        # Updated the parameters of the two Q0 and Q1 networks
        target_q_values = self.calc_target_q(rewards, next_states)  # Calculate the target q values
        critic_1_q_values = self.critic_1(states).gather(1, actions)
        critic_1_loss = torch.mean(F.mse_loss(critic_1_q_values, target_q_values.detach()))
        self.critic_1_optimizer.zero_grad()
        critic_1_loss.backward()
        self.critic_1_optimizer.step()

        critic_2_q_values = self.critic_2(states).gather(1, actions)
        critic_2_loss = torch.mean(F.mse_loss(critic_2_q_values, target_q_values.detach()))
        self.critic_2_optimizer.zero_grad()
        critic_2_loss.backward()
        self.critic_2_optimizer.step()

        # Updated the parameters of the actor network
        probs = self.actor(states)
        log_probs = torch.log(probs + 1e-8)  # 1e-8 is used to ensure the definition sense of log function
        entropy = -torch.sum(probs * log_probs, dim=1, keepdim=True)  # Calculate the entropy
        q1_value = self.critic_1(states)
        q2_value = self.critic_2(states)
        min_qvalue = torch.sum(probs * torch.min(q1_value, q2_value), dim=1, keepdim=True)  # Calculate the expectation
        actor_loss = torch.mean(-self.log_alpha.exp() * entropy - min_qvalue)
        self.actor_optimizer.zero_grad()
        actor_loss.backward()
        self.actor_optimizer.step()

        # Update the alpha value, i.e., log(alpha)
        alpha_loss = torch.mean((entropy - self.target_entropy).detach() * self.log_alpha.exp())
        self.log_alpha_optimizer.zero_grad()
        alpha_loss.backward()
        self.log_alpha_optimizer.step()

        # By soft operation, update the parameters of the V and target V networks.
        self.soft_update(self.critic_1, self.target_1)  # Update the V network parameters
        self.soft_update(self.critic_2, self.target_2)  # Update the target V network parameters


