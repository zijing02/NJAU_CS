from environment_lad import OffloadEnvironment
from model_lad import *
import matplotlib.pyplot as plt


def LAD_TS_algorithm():
    # Initial environment parameters
    NUM_BSs = 20  # The number of Base Stations （BSs）or Edge Servers (ESs)
    NUM_TASKS = 50  # The max number of task in each BS
    BIT_RANGE = [2, 5]  # The range [2, 5] (in Mbits) of task size.
    QUALITY_RANGE = [1, 15]  # The demand range [1, 15] of AIGC quality
    NUM_TIME_SLOTS = 60  # The number of time slot set
    ES_capacity_max = 50  # The maximal computing capacity in ESs.
    np.random.seed(2)
    ES_capacity = np.random.randint(10, ES_capacity_max + 1, size=[NUM_BSs])  # The computing capacities of all ESs

    # Initial DRL model parameters
    actor_lr = 1e-4  # The learn rate of the actor (i.e., diffusion) network
    critic_lr = 1e-3  # The learn rate of the critic network
    alpha = 0.05  # The temperature (i.e., \xi in the paper) of action entropy regularization （）
    alpha_lr = 3e-4  # The learning rate of entropy
    episodes = 500  # The number of episodes in network training procedure
    denoising_steps = 5  # The denoising steps in the diffusion-based scheduling model
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
    agent_list = list()
    train_buffer_list = list()
    for i in range(env.n_BSs):
        agent_list.append(LADSAC(env.state_dim, hidden_dim, env.action_dim, actor_lr, critic_lr, alpha, alpha_lr,
                                 target_entropy, tau, gamma, denoising_steps, device))
        train_buffer_list.append(ReplayBuffer(train_buffer_size))

    # =============== LAD-based Online Distributed Task Scheduling and Network Model Training ===================
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
            # Distributed LADSAC-based Task Scheduling
            for b in range(env.n_BSs):
                task_set_len = len(env.tasks_bit[t][b])
                for n in range(task_set_len):
                    state = np.hstack([env.tasks_bit[t][b][n],
                                       env.comp_density[n] * env.task_quality[n],
                                       env.proc_queue_len[t]])  # Observe the system state
                    latent_action_probs = env.latent_action_prob_space[t][b][n]  # Observe the latent action probability
                    action, action_probs = agent_list[b].take_action(state, latent_action_probs)  # Generate the offloading decision using Actor
                    env.latent_action_prob_space[t][b][n] = action_probs  # Update the latent action space
                    next_state, next_latent_actions, reward, delay = env.step(t, b, n, action)  # Perform the processing of task n
                    train_buffer_list[b].add(state, action, latent_action_probs, reward, next_state, next_latent_actions)  # Store history tuple

                    episode_delays.append(delay)  # Record the service delay (i.e., negative reward) of task n
                if train_buffer_list[b].size() > minimal_size:  # Network Model Training
                    b_s, b_a, b_p, b_r, b_ns, b_np = train_buffer_list[b].sample(batch_size)  # Sampling a batch of Sample
                    transition_dict = {'states': b_s, 'actions': b_a, 'latent_action_probs': b_p,
                                       'rewards': b_r, 'next_states': b_ns, 'next_latent_action_probs': b_np}
                    agent_list[b].update(transition_dict)  # Train and update network model parameters
            env.update_proc_queues(t)  # Update the processing queue of all ESS

        average_delays.append(np.mean(episode_delays))  # Store the average delay of each episode
        print({'episode': '%d' % (i_episode + 1), 'average delay': '%.4f' % average_delays[-1]})

    print('============ Finish all tasks offloading and model training ==========')

    episodes_list = list(range(len(average_delays)))
    # Save and plot the average delay varying episodes
    np.savetxt('results/AveDelay_lad_tasks1-' + str(NUM_TASKS) +
               '_quality1-' + str(QUALITY_RANGE[1]) +
               '_BS' + str(NUM_BSs) +
               '_f10-' + str(ES_capacity_max) +
               '_steps' + str(denoising_steps) +
               '_episode' + str(episodes) + '.csv', average_delays, delimiter=',', fmt='%.4f')
    plt.figure(1)
    plt.plot(episodes_list, average_delays)
    plt.ylabel('Average delay')
    plt.xlabel('Episode')
    plt.savefig('results/AveDelay_lad_tasks1-' + str(NUM_TASKS) +
                '_quality1-' + str(QUALITY_RANGE[1]) +
                '_BS' + str(NUM_BSs) +
                '_f10-' + str(ES_capacity_max) +
                '_steps' + str(denoising_steps) +
                '_episode' + str(episodes) + '.png')
    plt.close()


if __name__ == '__main__':
    LAD_TS_algorithm()
