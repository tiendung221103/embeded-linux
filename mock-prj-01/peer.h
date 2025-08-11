#ifndef PEER_H
#define PEER_H

#include <pthread.h>
#include <netinet/in.h>

#define MAX_MSG_LEN 100

typedef struct {
    int id;
    int sockfd;
    char ip[INET_ADDRSTRLEN];
    int port;
    pthread_t thread_id;
    int active;
} Peer;

void peer_init(Peer *p);
void peer_set(Peer *p, int id, int sockfd, const char *ip, int port, pthread_t tid);

#endif
