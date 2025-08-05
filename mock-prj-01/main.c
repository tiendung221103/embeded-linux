#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_PEERS 10           ///< Maximum number of peers that can be connected
#define MAX_MSG_LEN 100        ///< Maximum length of a message
#define MAX_CMD_LEN 256        ///< Maximum length of a user command

/**
 * @brief Structure to hold information about a connected peer.
 */
typedef struct {
    int id;                             ///< Unique peer ID
    int sockfd;                         ///< Socket file descriptor
    char ip[INET_ADDRSTRLEN];          ///< IP address of the peer
    int port;                           ///< Port number of the peer
    pthread_t thread_id;               ///< Thread ID for handling the peer
    int active;                         ///< 1 if peer is connected, 0 otherwise
} Peer;

Peer peers[MAX_PEERS];     ///< Array of connected peers
int next_peer_id = 1;      ///< Used to assign incremental peer IDs
int server_port;           ///< Port number this node listens on
int listener_socket;       ///< Listening socket descriptor

// Function declarations
void *listener_thread(void *arg);
void *peer_handler_thread(void *arg);
void process_user_input();
int add_peer(int sockfd, const char *ip, int port, pthread_t tid);

/**
 * @brief Entry point for the P2P chat program.
 * @param argc Number of command-line arguments
 * @param argv Argument vector (expects listening port)
 * @return int Exit status
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <listening_port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    server_port = atoi(argv[1]);

    // Create TCP socket
    listener_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listener_socket < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Bind socket to the provided port
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(server_port)
    };

    if (bind(listener_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(listener_socket, MAX_PEERS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("[INFO] Listening on port %d...\n", server_port);

    // Start the listener thread
    pthread_t listener_tid;
    pthread_create(&listener_tid, NULL, listener_thread, NULL);

    // Start reading user input in main thread
    process_user_input();

    close(listener_socket);
    return 0;
}

/**
 * @brief Thread function to accept incoming peer connections.
 * @param arg Unused
 * @return void* Always returns NULL
 */
void *listener_thread(void *arg) {
    while (1) {
        struct sockaddr_in peer_addr;
        socklen_t len = sizeof(peer_addr);
        int peer_sock = accept(listener_socket, (struct sockaddr *)&peer_addr, &len);
        if (peer_sock < 0) {
            perror("accept");
            continue;
        }

        // Convert IP address to string
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &peer_addr.sin_addr, ip, sizeof(ip));
        int port = ntohs(peer_addr.sin_port);
        printf("\n[+] New incoming connection from %s:%d\n", ip, port);

        // Start a new thread to handle communication with this peer
        pthread_t tid;
        int *pclient = malloc(sizeof(int));
        *pclient = peer_sock;
        pthread_create(&tid, NULL, peer_handler_thread, pclient);

        // Add to peer list
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
 * @brief Thread function to handle communication with a single peer.
 * @param arg Pointer to socket file descriptor (int*)
 * @return void* Always returns NULL
 */
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

/**
 * @brief Adds a new peer to the list of connected peers.
 * @param sockfd Socket file descriptor
 * @param ip IP address of the peer
 * @param port Port number of the peer
 * @param tid Thread ID handling this peer
 * @return int Assigned peer ID or -1 if the list is full
 */
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

/**
 * @brief Reads and processes user commands from the CLI.
 */
void process_user_input() {
    char command[MAX_CMD_LEN];

    while (1) {
        printf("> ");
        fflush(stdout);

        if (!fgets(command, sizeof(command), stdin))
            continue;
        command[strcspn(command, "\n")] = 0;  // Remove newline

        // ------------------ HELP ------------------
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

        // ------------------ MY IP ------------------
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

        // ------------------ MY PORT ------------------
        } else if (strcmp(command, "myport") == 0) {
            printf("Listening port: %d\n", server_port);

        // ------------------ CONNECT ------------------
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

        // ------------------ LIST ------------------
        } else if (strcmp(command, "list") == 0) {
            printf("ID\tIP Address\tPort\n");
            for (int i = 0; i < MAX_PEERS; i++) {
                if (peers[i].active) {
                    printf("%d\t%s\t%d\n", peers[i].id, peers[i].ip, peers[i].port);
                }
            }

        // ------------------ SEND ------------------
        } else if (strncmp(command, "send ", 5) == 0) {
            int id;
            char msg[MAX_MSG_LEN + 1];
            if (sscanf(command + 5, "%d %[^\n]", &id, msg) != 2) {
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

        // ------------------ TERMINATE ------------------
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

        // ------------------ EXIT ------------------
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

        // ------------------ UNKNOWN ------------------
        } else {
            printf("Unknown command. Type 'help'.\n");
        }
    }
}
