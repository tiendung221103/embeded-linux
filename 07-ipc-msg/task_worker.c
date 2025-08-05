#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <signal.h>
#include <unistd.h>

#define QUEUE_NAME "/my_task_queue"
#define MAX_MSG_SIZE 256
#define MAX_MESSAGES 10

mqd_t mq; // message queue descriptor

void cleanup(int sig) {
    mq_close(mq);
    mq_unlink(QUEUE_NAME);
    printf("\nQueue closed and unlinked. Exiting.\n");
    exit(0);
}

int main() {
    struct mq_attr attr;
    char buffer[MAX_MSG_SIZE];
    unsigned int prio;

    // Đảm bảo luôn tạo queue mới
    mq_unlink(QUEUE_NAME);

    // Cấu hình queue
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    // Tạo queue mới
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
    if (mq == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, cleanup);
    printf("Worker started. Waiting for tasks...\n");

    while (1) {
        ssize_t bytes_read = mq_receive(mq, buffer, MAX_MSG_SIZE, &prio);
        if (bytes_read >= 0) {
            buffer[bytes_read] = '\0';
            printf("Processing task (Priority: %u): %s\n", prio, buffer);
            sleep(1);
        } else {
            perror("mq_receive");
        }
    }

    return 0;
}
