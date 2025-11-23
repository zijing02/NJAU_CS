from environment_opt import OffloadEnvironment
import numpy as np
import matplotlib.pyplot as plt

if __name__ == "__main__":
    # Initial environment parameters
    NUM_BSs = 20  # The number of Base Stations （BSs）or Edge Servers (ESs)
    NUM_TASKS = 50  # The max number of tasks in each BS
    BIT_RANGE = [2, 5]  # The range of task bits
    QUALITY_RANGE = [1, 15]  # The range of task workload
    NUM_TIME_SLOTS = 60  # The number of time slot set
    ES_capacity_max = 20
    np.random.seed(2)
    ES_capacity = np.random.randint(10, ES_capacity_max + 1, size=[NUM_BSs])  # The computing capacity of ES
    episodes = 500
    # Generate offloading environment
    env = OffloadEnvironment(NUM_TASKS, BIT_RANGE, QUALITY_RANGE, NUM_BSs, NUM_TIME_SLOTS, ES_capacity)

    average_delays = []  # service delays
    for i_episode in range(episodes):
        # Generate the arrival tasks
        arrival_tasks = []
        for i in range(env.time_slots):
            # np.random.seed(1)
            task_dim = np.random.randint(1, env.n_tasks + 1, size=[env.n_BSs])
            BS_tasks = []
            for j in range(env.n_BSs):
                # np.random.seed(1)
                BS_tasks.append(np.random.uniform(env.min_bit, env.max_bit, size=[task_dim[j]]))
            arrival_tasks.append(BS_tasks)
        env.reset(arrival_tasks)  # Initialize the system environment

        episode_delays = []
        for t in range(env.time_slots - 1):
            for b in range(env.n_BSs):
                task_set_len = len(env.tasks_bit[t][b])
                for n in range(task_set_len):
                    n_delay = env.step(t, b, n)
                    episode_delays.append(n_delay)
            env.update_proc_queues(t)  # Update the processing queue of all ESs

        average_delays.append(np.mean(episode_delays))
        print({'episode': '%d' % (i_episode + 1), 'average delay': '%.7f' % average_delays[-1]})

    print('============  Finish all tasks offloading and model training ==========')

    episodes_list = list(range(len(average_delays)))
    # Plot the average delay varying episodes
    np.savetxt('../../results/AveDelay_opt_tasks1-' + str(NUM_TASKS) +
               '_quality1-' + str(QUALITY_RANGE[1]) +
               '_BS' + str(NUM_BSs) +
               '_f10-' + str(ES_capacity_max) +
               '_episode' + str(episodes) + '.csv', average_delays, delimiter=',', fmt='%.4f')
    plt.figure(1)
    plt.plot(episodes_list, average_delays)
    plt.ylabel('Average make-span')
    plt.xlabel('Episode')
    plt.savefig('../../results/AveDelay_opt_tasks1-' + str(NUM_TASKS) +
                '_quality1-' + str(QUALITY_RANGE[1]) +
                '_BS' + str(NUM_BSs) +
                '_f10-' + str(ES_capacity_max) +
                '_episode' + str(episodes) + '.png')
    plt.close()
