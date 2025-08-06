/**
 * @file command.c
 * @brief Handles user commands for P2P CLI chat application.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "peer.h"
#include "connection.h"
#include "command.h"

/**
 * @brief Process user commands from standard input.
 * 
 * Provides a command interface to interact with peers,
 * manage connections, and exchange messages.
 */
void process_user_input() {
    char command[MAX_CMD_LEN];

    while (1) {
        printf("> ");
        fflush(stdout);

        // Read command line input
        if (!fgets(command, sizeof(command), stdin))
            continue;

        // Remove trailing newline character
        command[strcspn(command, "\n")] = 0;

        // Show help
        if (strcmp(command, "help") == 0) {
            printf("Commands:\n");
            printf("help               - Show this help\n");
            printf("myip               - Show local IP\n");
            printf("myport             - Show listening port\n");
            printf("connect <ip> <port>- Connect to peer\n");
            printf("list               - List connections\n");
            printf("send <id> <msg>    - Send message\n");
            printf("terminate <id>     - End connection\n");
            printf("exit               - Exit program\n");

        // Display local IP address
        } else if (strcmp(command, "myip") == 0) {
            int tmp_sock = socket(AF_INET, SOCK_DGRAM, 0);
            struct sockaddr_in temp_addr = {
                .sin_family = AF_INET,
                .sin_port = htons(80)
            };
            inet_pton(AF_INET, "8.8.8.8", &temp_addr.sin_addr);
            connect(tmp_sock, (struct sockaddr *)&temp_addr, sizeof(temp_addr));

            struct sockaddr_in local_addr;
            socklen_t len = sizeof(local_addr);
            getsockname(tmp_sock, (struct sockaddr *)&local_addr, &len);

            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &local_addr.sin_addr, ip, sizeof(ip));
            printf("Local IP: %s\n", ip);
            close(tmp_sock);

        // Display listening port
        } else if (strcmp(command, "myport") == 0) {
            printf("Listening port: %d\n", server_port);

        // Connect to a peer
        } else if (strncmp(command, "connect ", 8) == 0) {
            char ip[INET_ADDRSTRLEN];
            int port;
            if (sscanf(command + 8, "%s %d", ip, &port) != 2) {
                printf("Usage: connect <ip> <port>\n");
                continue;
            }

            int sockfd = socket(AF_INET, SOCK_STREAM, 0);
            struct sockaddr_in addr = {
                .sin_family = AF_INET,
                .sin_port = htons(port)
            };
            inet_pton(AF_INET, ip, &addr.sin_addr);

            if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
                perror("connect");
                close(sockfd);
                continue;
            }

            printf("[+] Connected to %s:%d\n", ip, port);
            
            pthread_t tid;
            int *p = malloc(sizeof(int));
            *p = sockfd;
            pthread_create(&tid, NULL, peer_handler_thread, p);

            int id = add_peer(sockfd, ip, port, tid);
            if (id == -1) {
                printf("[-] Connection list full.\n");
                close(sockfd);
            } else {
                printf("[ID %d] Connection established.\n", id);
            }

        // List active connections
        } else if (strcmp(command, "list") == 0) {
            printf("ID\tIP Address\tPort\n");
            for (int i = 0; i < MAX_PEERS; i++) {
                if (peers[i].active) {
                    printf("%d\t%s\t%d\n", peers[i].id, peers[i].ip, peers[i].port);
                }
            }

        // Send message to peer
        } else if (strncmp(command, "send ", 5) == 0) {
            int id;
            char msg[MAX_MSG_LEN + 1];
            if (sscanf(command + 5, "%d %[^\"]", &id, msg) != 2) {
                printf("Usage: send <id> <message>\n");
                continue;
            }

            int found = 0;
            for (int i = 0; i < MAX_PEERS; i++) {
                if (peers[i].active && peers[i].id == id) {
                    send(peers[i].sockfd, msg, strlen(msg), 0);
                    found = 1;
                    break;
                }
            }
            if (!found) printf("[-] No such connection ID %d\n", id);

        // Terminate a connection
        } else if (strncmp(command, "terminate ", 10) == 0) {
            int id;
            if (sscanf(command + 10, "%d", &id) != 1) {
                printf("Usage: terminate <id>\n");
                continue;
            }

            int found = 0;
            for (int i = 0; i < MAX_PEERS; i++) {
                if (peers[i].active && peers[i].id == id) {
                    close(peers[i].sockfd);
                    pthread_cancel(peers[i].thread_id);
                    peers[i].active = 0;
                    printf("[x] Terminated connection ID %d\n", id);
                    found = 1;
                    break;
                }
            }
            if (!found) printf("[-] ID %d not found\n", id);

        // Exit program
        } else if (strcmp(command, "exit") == 0) {
            printf("Exiting and closing all connections...\n");
            for (int i = 0; i < MAX_PEERS; i++) {
                if (peers[i].active) {
                    close(peers[i].sockfd);
                    pthread_cancel(peers[i].thread_id);
                    peers[i].active = 0;
                }
            }
            close(listener_socket);
            exit(0);

        // Unknown command
        } else {
            printf("Unknown command. Type 'help'.\n");
        }
    }
}
