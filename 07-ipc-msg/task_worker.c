#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <signal.h>
#include <unistd.h>

#define QUEUE_NAME "/my_task_queue"   ///< Name of the message queue
#define MAX_MSG_SIZE 256              ///< Maximum size of a message
#define MAX_MESSAGES 10               ///< Maximum number of messages in the queue

mqd_t mq; ///< Message queue descriptor

/**
 * @brief Cleanup function called on SIGINT to close and remove the message queue.
 * @param sig The received signal (e.g., SIGINT)
 */
void cleanup(int sig) {
    mq_close(mq);              // Close the message queue
    mq_unlink(QUEUE_NAME);     // Remove the queue from the system
    printf("\nQueue closed and unlinked. Exiting.\n");
    exit(0);
}

/**
 * @brief Entry point of the worker process. Waits for tasks via message queue and processes them.
 * @return int Exit status
 */
int main() {
    struct mq_attr attr;
    char buffer[MAX_MSG_SIZE];
    unsigned int prio;

    // Remove the queue if it already exists to ensure a clean start
    mq_unlink(QUEUE_NAME);

    // Configure queue attributes
    attr.mq_flags = 0;                // Blocking mode
    attr.mq_maxmsg = MAX_MESSAGES;    // Maximum number of messages
    attr.mq_msgsize = MAX_MSG_SIZE;   // Maximum size per message
    attr.mq_curmsgs = 0;              // Initially no messages

    // Create a new message queue for reading (worker only receives)
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
    if (mq == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    // Handle Ctrl+C (SIGINT) to clean up resources
    signal(SIGINT, cleanup);

    printf("Worker started. Waiting for tasks...\n");

    // Main loop: receive and process tasks
    while (1) {
        ssize_t bytes_read = mq_receive(mq, buffer, MAX_MSG_SIZE, &prio);
        if (bytes_read >= 0) {
            buffer[bytes_read] = '\0'; // Null-terminate the received string
            printf("Processing task (Priority: %u): %s\n", prio, buffer);
            sleep(1); // Simulate processing time
        } else {
            perror("mq_receive");
        }
    }

    return 0;
}
