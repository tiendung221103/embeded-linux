#ifndef PEER_H
#define PEER_H

#include <netinet/in.h>
#include <pthread.h>

#define MAX_PEERS 10
#define MAX_MSG_LEN 100
#define MAX_CMD_LEN 256

typedef struct {
    int id;
    int sockfd;
    char ip[INET_ADDRSTRLEN];
    int port;
    pthread_t thread_id;
    int active;
} Peer;

extern Peer peers[MAX_PEERS];
extern int next_peer_id;
extern int server_port;
extern int listener_socket;

#endif