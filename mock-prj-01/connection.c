#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "peer.h"
#include "connection.h"

Peer peers[MAX_PEERS];
int next_peer_id = 1;

int add_peer(int sockfd, const char *ip, int port, pthread_t tid) {
    for (int i = 0; i < MAX_PEERS; i++) {
        if (!peers[i].active) {
            peers[i] = (Peer){
                .id = next_peer_id++,
                .sockfd = sockfd,
                .port = port,
                .thread_id = tid,
                .active = 1
            };
            strncpy(peers[i].ip, ip, INET_ADDRSTRLEN);
            return peers[i].id;
        }
    }
    return -1;
}

void *listener_thread(void *arg) {
    while (1) {
        struct sockaddr_in peer_addr;
        socklen_t len = sizeof(peer_addr);
        int peer_sock = accept(listener_socket, (struct sockaddr *)&peer_addr, &len);
        if (peer_sock < 0) {
            perror("accept");
            continue;
        }

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &peer_addr.sin_addr, ip, sizeof(ip));
        int port = ntohs(peer_addr.sin_port);
        printf("\n[+] New incoming connection from %s:%d\n", ip, port);

        pthread_t tid;
        int *pclient = malloc(sizeof(int));
        *pclient = peer_sock;
        pthread_create(&tid, NULL, peer_handler_thread, pclient);

        int id = add_peer(peer_sock, ip, port, tid);
        if (id == -1) {
            printf("[-] Connection limit reached.\n");
            close(peer_sock);
        } else {
            printf("[ID %d] Connection accepted.\n", id);
        }
    }
    return NULL;
}

void *peer_handler_thread(void *arg) {
    int sockfd = *((int *)arg);
    free(arg);

    char buffer[MAX_MSG_LEN + 1];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int n = recv(sockfd, buffer, MAX_MSG_LEN, 0);
        if (n <= 0) {
            printf("[-] A peer disconnected.\n");
            close(sockfd);
            break;
        }
        buffer[n] = '\0';
        printf("\n[Message received]: %s\n> ", buffer);
        fflush(stdout);
    }
    return NULL;
}