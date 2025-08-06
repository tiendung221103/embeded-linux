/**
 * @file peer.h
 * @brief Defines the Peer structure and global variables for managing P2P connections.
 */

#ifndef PEER_H
#define PEER_H

#include <netinet/in.h>
#include <pthread.h>

#define MAX_PEERS 10
#define MAX_MSG_LEN 100
#define MAX_CMD_LEN 256

/**
 * @struct Peer
 * @brief Represents a connected peer in the P2P network.
 * 
 * This structure stores metadata and connection info for each peer.
 */
typedef struct {
    int id;                         ///< Unique ID assigned to the peer
    int sockfd;                     ///< Socket file descriptor
    char ip[INET_ADDRSTRLEN];       ///< IP address of the peer
    int port;                       ///< Port number used by the peer
    pthread_t thread_id;            ///< Thread handling the peer's connection
    int active;                     ///< Flag indicating whether the peer is active (1) or not (0)
} Peer;

// Global declarations used across modules

extern Peer peers[MAX_PEERS];       ///< Array to store all connected peers
extern int next_peer_id;            ///< Used to assign unique IDs to peers
extern int server_port;             ///< Port this peer/server listens on
extern int listener_socket;         ///< Listening socket descriptor

#endif
