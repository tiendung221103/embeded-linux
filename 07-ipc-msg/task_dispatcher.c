#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>

#define QUEUE_NAME "/my_task_queue"
#define MAX_MSG_SIZE 256

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
    mqd_t mq = mq_open(QUEUE_NAME, O_WRONLY);
    if (mq == -1) {
        perror("mq_open");
        return EXIT_FAILURE;
    }

    if (mq_send(mq, task, strlen(task), prio) == -1) {
        perror("mq_send");
        return EXIT_FAILURE;
    }

    printf("Task sent with priority %u: %s\n", prio, task);
    mq_close(mq);
    return 0;
}
