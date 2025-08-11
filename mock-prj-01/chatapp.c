#include "chatapp.h"
#include "utils.h"
#include "peer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <limits.h>

static void *listen_thread(void *arg);
static void *peer_handle_thread(void *arg);
static void peer_cleanup(void *arg);

void chatapp_setup(ChatApp *app, int port) {
    app->next_peer_id = 1;
    app->server_port = port;
    app->running = 1;
    pthread_mutex_init(&app->mutex, NULL);
    for (int i = 0; i < MAX_PEERS; i++) {
        peer_init(&app->peers[i]);
    }

    app->listener_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (app->listener_socket < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int yes = 1;
    setsockopt(app->listener_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(app->listener_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(app->listener_socket, MAX_PEERS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

void chatapp_launch(ChatApp *app) {
    printf("[INFO] Listening on port %d...\n", app->server_port);
    if (pthread_create(&app->listener_tid, NULL, listen_thread, app) != 0) {
        perror("pthread_create listener");
        close(app->listener_socket);
        exit(EXIT_FAILURE);
    }
}

void chatapp_shutdown(ChatApp *app) {
    pthread_mutex_lock(&app->mutex);
    app->running = 0;
    pthread_mutex_unlock(&app->mutex);

    close(app->listener_socket);
    pthread_join(app->listener_tid, NULL);

    pthread_mutex_lock(&app->mutex);
    for (int i = 0; i < MAX_PEERS; i++) {
        if (app->peers[i].active) {
            close(app->peers[i].sockfd);
            pthread_cancel(app->peers[i].thread_id);
            app->peers[i].active = 0;
        }
    }
    pthread_mutex_unlock(&app->mutex);
    pthread_mutex_destroy(&app->mutex);
}

static void *listen_thread(void *arg) {
    ChatApp *app = (ChatApp *)arg;
    while (1) {
        pthread_mutex_lock(&app->mutex);
        int keep_running = app->running;
        pthread_mutex_unlock(&app->mutex);
        if (!keep_running) break;

        struct sockaddr_in peer_addr;
        socklen_t len = sizeof(peer_addr);
        int peer_sock = accept(app->listener_socket, (struct sockaddr *)&peer_addr, &len);
        if (peer_sock < 0) {
            perror("accept");
            continue;
        }

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &peer_addr.sin_addr, ip, sizeof(ip));
        int port = ntohs(peer_addr.sin_port);
        printf("\n[+] New incoming connection from %s:%d\n", ip, port);

        pthread_t tid;
        PeerThreadArg *thread_arg = (PeerThreadArg *)malloc(sizeof(PeerThreadArg));
        if (!thread_arg) {
            perror("malloc");
            close(peer_sock);
            continue;
        }
        thread_arg->app = app;
        thread_arg->sockfd = peer_sock;
        if (pthread_create(&tid, NULL, peer_handle_thread, thread_arg) != 0) {
            perror("pthread_create peer");
            close(peer_sock);
            free(thread_arg);
            continue;
        }

        int id = chatapp_add_peer(app, peer_sock, ip, port, tid);
        if (id == -1) {
            printf("[-] Connection limit reached.\n");
            close(peer_sock);
            pthread_cancel(tid);
        } else {
            printf("[ID %d] Connection accepted.\n", id);
        }
    }
    return NULL;
}

static void *peer_handle_thread(void *arg) {
    PeerThreadArg *thread_arg = (PeerThreadArg *)arg;
    ChatApp *app = thread_arg->app;
    int sockfd = thread_arg->sockfd;
    free(thread_arg);

    pthread_cleanup_push(peer_cleanup, &sockfd);

    char buffer[MAX_MSG_LEN + 1];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int n = recv(sockfd, buffer, MAX_MSG_LEN, 0);
        if (n <= 0) {
            printf("[-] A peer disconnected.\n");
            pthread_mutex_lock(&app->mutex);
            for (int i = 0; i < MAX_PEERS; i++) {
                if (app->peers[i].active && app->peers[i].sockfd == sockfd) {
                    app->peers[i].active = 0;
                    break;
                }
            }
            pthread_mutex_unlock(&app->mutex);
            close(sockfd);
            break;
        }
        buffer[n] = '\\0';
        printf("\\n[Message received]: %s\\n> ", buffer);
        fflush(stdout);
    }

    pthread_cleanup_pop(1);
    return NULL;
}

static void peer_cleanup(void *arg) {
    int sockfd = *(int *)arg;
    if (sockfd >= 0) close(sockfd);
}

int chatapp_add_peer(ChatApp *app, int sockfd, const char *ip, int port, pthread_t tid) {
    pthread_mutex_lock(&app->mutex);
    if (app->next_peer_id > INT_MAX - 1) {
        printf("[-] Peer ID limit reached.\n");
        pthread_mutex_unlock(&app->mutex);
        return -1;
    }
    for (int i = 0; i < MAX_PEERS; i++) {
        if (!app->peers[i].active) {
            peer_set(&app->peers[i], app->next_peer_id++, sockfd, ip, port, tid);
            int id = app->peers[i].id;
            pthread_mutex_unlock(&app->mutex);
            return id;
        }
    }
    pthread_mutex_unlock(&app->mutex);
    return -1;
}

int chatapp_connect_peer(ChatApp *app, const char *ip, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &addr.sin_addr) != 1) {
        printf("Invalid IP address\\n");
        close(sockfd);
        return -1;
    }

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(sockfd);
        return -1;
    }

    printf("[+] Connected to %s:%d\\n", ip, port);
    pthread_t tid;
    PeerThreadArg *thread_arg = (PeerThreadArg *)malloc(sizeof(PeerThreadArg));
    if (!thread_arg) {
        perror("malloc");
        close(sockfd);
        return -1;
    }
    thread_arg->app = app;
    thread_arg->sockfd = sockfd;
    if (pthread_create(&tid, NULL, peer_handle_thread, thread_arg) != 0) {
        perror("pthread_create connect");
        close(sockfd);
        free(thread_arg);
        return -1;
    }

    int id = chatapp_add_peer(app, sockfd, ip, port, tid);
    if (id == -1) {
        printf("[-] Connection list full.\\n");
        close(sockfd);
        pthread_cancel(tid);
        return -1;
    }
    printf("[ID %d] Connection established.\\n", id);
    return id;
}

void chatapp_list_peers(ChatApp *app) {
    printf("ID\\tIP Address\\tPort\\n");
    pthread_mutex_lock(&app->mutex);
    for (int i = 0; i < MAX_PEERS; i++) {
        if (app->peers[i].active) {
            printf("%d\\t%s\\t%d\\n", app->peers[i].id, app->peers[i].ip, app->peers[i].port);
        }
    }
    pthread_mutex_unlock(&app->mutex);
}

int chatapp_send_message(ChatApp *app, int id, const char *msg) {
    pthread_mutex_lock(&app->mutex);
    for (int i = 0; i < MAX_PEERS; i++) {
        if (app->peers[i].active && app->peers[i].id == id) {
            send(app->peers[i].sockfd, msg, strlen(msg), 0);
            pthread_mutex_unlock(&app->mutex);
            return 0;
        }
    }
    pthread_mutex_unlock(&app->mutex);
    printf("[-] No such connection ID %d\\n", id);
    return -1;
}

int chatapp_disconnect_peer(ChatApp *app, int id) {
    pthread_mutex_lock(&app->mutex);
    for (int i = 0; i < MAX_PEERS; i++) {
        if (app->peers[i].active && app->peers[i].id == id) {
            close(app->peers[i].sockfd);
            pthread_cancel(app->peers[i].thread_id);
            app->peers[i].active = 0;
            printf("[x] Terminated connection ID %d\\n", id);
            pthread_mutex_unlock(&app->mutex);
            return 0;
        }
    }
    pthread_mutex_unlock(&app->mutex);
    printf("[-] ID %d not found\\n", id);
    return -1;
}
