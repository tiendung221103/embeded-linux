#include "peer.h"
#include <string.h>

void peer_init(Peer *p) {
    p->id = 0;
    p->sockfd = -1;
    p->port = 0;
    p->thread_id = 0;
    p->active = 0;
    memset(p->ip, 0, sizeof(p->ip));
}

void peer_set(Peer *p, int id, int sockfd, const char *ip, int port, pthread_t tid) {
    p->id = id;
    p->sockfd = sockfd;
    p->port = port;
    p->thread_id = tid;
    p->active = 1;
    strncpy(p->ip, ip, sizeof(p->ip));
    p->ip[sizeof(p->ip)-1] = '\0';
}
