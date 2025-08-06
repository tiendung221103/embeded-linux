/**
 * @file connection.c
 * @brief Handles peer connections for a P2P chat application.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "peer.h"
#include "connection.h"

Peer peers[MAX_PEERS];      ///< Array to store connected peers
int next_peer_id = 1;       ///< Used to assign a unique ID to each peer

/**
 * @brief Add a new peer to the peer list.
 * 
 * @param sockfd Socket file descriptor for the new peer.
 * @param ip IP address of the new peer.
 * @param port Port number of the new peer.
 * @param tid Thread ID handling this peer.
 * @return int The assigned ID of the peer, or -1 if list is full.
 */
int add_peer(int sockfd, const char *ip, int port, pthread_t tid) {
    for (int i = 0; i < MAX_PEERS; i++) {
        if (!peers[i].active) {
            // Populate peer struct
            peers[i] = (Peer){
                .id = next_peer_id++,
                .sockfd = sockfd,
                .port = port,
                .thread_id = tid,
                .active = 1
            };
            strncpy(peers[i].ip, ip, INET_ADDRSTRLEN);  // Copy IP safely
            return peers[i].id;
        }
    }
    return -1;  // Peer list full
}

/**
 * @brief Thread function that listens for incoming peer connections.
 * 
 * Accepts incoming connections and creates a thread to handle each one.
 * Adds each connection to the peer list.
 * 
 * @param arg Unused.
 * @return void* Always returns NULL.
 */
void *listener_thread(void *arg) {
    while (1) {
        struct sockaddr_in peer_addr;
        socklen_t len = sizeof(peer_addr);

        // Accept an incoming connection
        int peer_sock = accept(listener_socket, (struct sockaddr *)&peer_addr, &len);
        if (peer_sock < 0) {
            perror("accept");
            continue;
        }

        // Extract IP and port
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &peer_addr.sin_addr, ip, sizeof(ip));
        int port = ntohs(peer_addr.sin_port);

        printf("\n[+] New incoming connection from %s:%d\n", ip, port);

        // Create thread to handle peer communication
        pthread_t tid;
        int *pclient = malloc(sizeof(int));
        *pclient = peer_sock;
        pthread_create(&tid, NULL, peer_handler_thread, pclient);

        // Add peer to peer list
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

/**
 * @brief Thread function to handle communication with a connected peer.
 * 
 * Continuously receives messages from the peer and prints them.
 * If the peer disconnects, closes socket and exits the thread.
 * 
 * @param arg Pointer to the socket file descriptor (malloc'ed).
 * @return void* Always returns NULL.
 */
void *peer_handler_thread(void *arg) {
    int sockfd = *((int *)arg);
    free(arg);  // Free dynamically allocated socket descriptor

    char buffer[MAX_MSG_LEN + 1];
    while (1) {
        memset(buffer, 0, sizeof(buffer));

        // Receive message from peer
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
