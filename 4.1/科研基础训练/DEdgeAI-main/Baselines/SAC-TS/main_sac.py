import random
import numpy as np
from environment_sac import OffloadEnvironment
from model_sac import *
import matplotlib.pyplot as plt


def SAC_TS_algorithm():
    # Initial environment parameters
    NUM_BSs = 20  # The number of Base Stations （BSs）or Edge Servers (ESs)
    NUM_TASKS = 50  # The max number of task in each BS
    BIT_RANGE = [2, 5]  # The range [2, 5] of task bits
    QUALITY_RANGE = [1, 15]  # The demand range [1, 15] of AIGC quality
    NUM_TIME_SLOTS = 60  # The number of time slot set
    ES_capacity_max = 50
    np.random.seed(2)
    ES_capacity = np.random.randint(10, ES_capacity_max + 1, size=[NUM_BSs])  # The computing capacity of ES

    # Initial DRL model parameters
    actor_lr = 1e-4  # The learn rate of the actor (i.e., diffusion) network
    critic_lr = 1e-3  # The learn rate of the critic network
    alpha = 0.05  # The temperature of action entropy regularization
    alpha_lr = 3e-4  # The learning rate of entropy
    episodes = 500  # The number of episodes in network training procedure
    hidden_dim = 20  # The hidden neurons of the DNs, CNs, and TNs
    gamma = 0.95  # The reward decay parameter
    tau = 0.005  # The weight parameter of soft updating operation
    train_buffer_size = 1000  # The capacity of experience pool in the memory
    minimal_size = 300  # The minimal size of history tuple in the experience pool
    batch_size = 64  # The batch size of sampling history tuple
    target_entropy = -1  # The target entropy parameter
    device = torch.device("cuda") if torch.cuda.is_available() else torch.device("cpu")

    # Generate offloading environment
    env = OffloadEnvironment(NUM_TASKS, BIT_RANGE, QUALITY_RANGE, NUM_BSs, NUM_TIME_SLOTS, ES_capacity)

    # Distributed task scheduling model: Each BS has an agent to make task scheduling and network training
    agent_list = list()  # Initialize a list for all agents
    train_buffer_list = list()  # Initialize a list for all experience pools
    for i in range(env.n_BSs):
        agent_list.append(SAC(env.state_dim, hidden_dim, env.action_dim, actor_lr, critic_lr, alpha, alpha_lr,
                              target_entropy, tau, gamma, device))
        train_buffer_list.append(ReplayBuffer(train_buffer_size))

    # =============== Distributed SAC-based Task Scheduling and Network Model Training ===================
    average_delays = []  # service delays
    for i_episode in range(episodes):
        # Generate the arrival tasks
        arrival_tasks = []
        for i in range(env.time_slots):
            task_dim = np.random.randint(1, env.n_tasks + 1, size=[env.n_BSs])
            BS_tasks = []
            for j in range(env.n_BSs):
                BS_tasks.append(np.random.uniform(env.min_bit, env.max_bit, size=[task_dim[j]]))
            arrival_tasks.append(BS_tasks)
        # Reset environment
        env.reset_env(arrival_tasks)
        episode_delays = []
        for t in range(env.time_slots - 1):
            # ----- Distributed DSAC-based Task Scheduling ------
            for b in range(env.n_BSs):
                task_set_len = len(env.tasks_bit[t][b])
                for n in range(task_set_len):
                    state = np.hstack([env.tasks_bit[t][b][n],
                                       env.comp_density[n] * env.task_quality[n],
                                       env.proc_queue_len[t]])  # Observe the system state
                    action = agent_list[b].take_action(state)  # Generate the offloading decision using Actor
                    next_state, reward, delay = env.step(t, b, n, action)  # # Perform the processing of task n
                    train_buffer_list[b].add(state, action, reward, next_state)  # Store history tuple
                    episode_delays.append(delay)  # Record the service delay (i.e., negative reward) of task n
                if train_buffer_list[b].size() > minimal_size:
                    b_s, b_a, b_r, b_ns = train_buffer_list[b].sample(batch_size)  # Sampling a batch of Sample
                    transition_dict = {'states': b_s, 'actions': b_a,
                                       'next_states': b_ns, 'rewards': b_r}
                    agent_list[b].update(transition_dict)  # Train and update network model parameters
            env.update_proc_queues(t)  # Update the processing queue of all ESS

        average_delays.append(np.mean(episode_delays))  # Store the average delay of each episode
        print({'episode': '%d' % (i_episode + 1), 'average delay': '%.4f' % average_delays[-1]})

    print('============ Finish all tasks offloading and model training ==========')

    episodes_list = list(range(len(average_delays)))
    # Plot the average delay varying episodes
    np.savetxt('../../results/AveDelay_sac_tasks1-' + str(NUM_TASKS) +
               '_quality1-' + str(QUALITY_RANGE[1]) +
               '_BS' + str(NUM_BSs) +
               '_f10-' + str(ES_capacity_max) +
               '_episode' + str(episodes) + '_hidden2.csv', average_delays, delimiter=',', fmt='%.4f')
    plt.figure(1)
    plt.plot(episodes_list, average_delays)
    plt.ylabel('Average make-span')
    plt.xlabel('Episode')
    plt.savefig('../../results/AveDelay_sac_tasks1-' + str(NUM_TASKS) +
                '_quality1-' + str(QUALITY_RANGE[1]) +
                '_BS' + str(NUM_BSs) +
                '_f10-' + str(ES_capacity_max) +
                '_episode' + str(episodes) + '_hidden2.png')
    plt.close()

if __name__ == '__main__':
    SAC_TS_algorithm()
