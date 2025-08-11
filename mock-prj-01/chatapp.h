#ifndef CHATAPP_H
#define CHATAPP_H

#include <pthread.h>
#include "peer.h"

#define MAX_PEERS 10

typedef struct {
    Peer peers[MAX_PEERS];
    int next_peer_id;
    int server_port;
    int listener_socket;
    pthread_mutex_t mutex;
    int running;
    pthread_t listener_tid;
} ChatApp;

typedef struct {
    ChatApp *app;
    int sockfd;
} PeerThreadArg;

void chatapp_setup(ChatApp *app, int port);
void chatapp_launch(ChatApp *app);
void chatapp_shutdown(ChatApp *app);
int chatapp_add_peer(ChatApp *app, int sockfd, const char *ip, int port, pthread_t tid);
int chatapp_connect_peer(ChatApp *app, const char *ip, int port);
void chatapp_list_peers(ChatApp *app);
int chatapp_send_message(ChatApp *app, int id, const char *msg);
int chatapp_disconnect_peer(ChatApp *app, int id);

#endif
