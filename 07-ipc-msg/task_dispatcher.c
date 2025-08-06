#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>

#define QUEUE_NAME "/my_task_queue"  ///< Name of the message queue
#define MAX_MSG_SIZE 256             ///< Maximum size of a message

/**
 * @brief Entry point for the producer program.
 *
 * Sends a message (task description) with a specified priority to the message queue.
 *
 * @param argc Argument count. Must be 3: program name, priority, message.
 * @param argv Argument vector:
 *             - argv[1]: Priority (0–31)
 *             - argv[2]: Task description (string)
 *
 * @return int Exit status: 0 on success, non-zero on failure.
 */
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <priority> \"<task_description>\"\n", argv[0]);
        return EXIT_FAILURE;
    }

    unsigned int prio = atoi(argv[1]);
    if (prio > 31) {
        fprintf(stderr, "Priority must be <= 31 (POSIX MQ limit).\n");
        return EXIT_FAILURE;
    }

    const char *task = argv[2];

    // Open the message queue for writing
    mqd_t mq = mq_open(QUEUE_NAME, O_WRONLY);
    if (mq == -1) {
        perror("mq_open");
        return EXIT_FAILURE;
    }

    // Send the message with the given priority
    if (mq_send(mq, task, strlen(task), prio) == -1) {
        perror("mq_send");
        return EXIT_FAILURE;
    }

    printf("Task sent with priority %u: %s\n", prio, task);

    mq_close(mq);
    return 0;
}
