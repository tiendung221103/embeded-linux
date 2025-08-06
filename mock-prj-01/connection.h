#ifndef CONNECTION_H
#define CONNECTION_H

#include <pthread.h>

void *listener_thread(void *arg);
void *peer_handler_thread(void *arg);
int add_peer(int sockfd, const char *ip, int port, pthread_t tid);

#endif
