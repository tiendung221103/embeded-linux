#ifndef CHATAPP_H
#define CHATAPP_H

#include <pthread.h>
#include "peer.h"

#define MAX_PEERS 10  // Maximum number of peers that can be connected

// Structure representing the entire chat application state
typedef struct {
    Peer peers[MAX_PEERS];       // Array of connected peers
    int next_peer_id;            // Counter for assigning unique peer IDs
    int server_port;             // Port number for listening to incoming connections
    int listener_socket;         // Socket descriptor for the listening server
    pthread_mutex_t mutex;       // Mutex to protect shared peer data
    int running;                 // Flag indicating if the app is running (1 = yes, 0 = no)
    pthread_t listener_tid;      // Thread ID for the listener thread
} ChatApp;

// Structure for passing arguments to peer-handling threads
typedef struct {
    ChatApp *app;   // Pointer to the main ChatApp instance
    int sockfd;     // Socket descriptor for the connected peer
} PeerThreadArg;

/**
 * @brief Initialize the chat application structure.
 * 
 * @param app Pointer to the ChatApp instance.
 * @param port Port number to listen on for incoming connections.
 */
void chatapp_setup(ChatApp *app, int port);

/**
 * @brief Start the listener thread to accept incoming peer connections.
 * 
 * @param app Pointer to the ChatApp instance.
 */
void chatapp_launch(ChatApp *app);

/**
 * @brief Shut down the chat application, close sockets, and stop threads.
 * 
 * @param app Pointer to the ChatApp instance.
 */
void chatapp_shutdown(ChatApp *app);

/**
 * @brief Add a new peer to the application's peer list.
 * 
 * @param app Pointer to the ChatApp instance.
 * @param sockfd Socket descriptor for the peer.
 * @param ip IP address of the peer.
 * @param port Port number of the peer.
 * @param tid Thread ID handling the peer.
 * @return int Assigned peer ID, or -1 on failure.
 */
int chatapp_add_peer(ChatApp *app, int sockfd, const char *ip, int port, pthread_t tid);

/**
 * @brief Connect to a peer given its IP address and port.
 * 
 * @param app Pointer to the ChatApp instance.
 * @param ip IP address of the target peer.
 * @param port Port number of the target peer.
 * @return int 0 on success, -1 on failure.
 */
int chatapp_connect_peer(ChatApp *app, const char *ip, int port);

/**
 * @brief Print the list of currently connected peers.
 * 
 * @param app Pointer to the ChatApp instance.
 */
void chatapp_list_peers(ChatApp *app);

/**
 * @brief Send a text message to a peer by its ID.
 * 
 * @param app Pointer to the ChatApp instance.
 * @param id Peer ID to send the message to.
 * @param msg The message string to send.
 * @return int 0 on success, -1 on failure.
 */
int chatapp_send_message(ChatApp *app, int id, const char *msg);

/**
 * @brief Disconnect a peer by its ID and clean up resources.
 * 
 * @param app Pointer to the ChatApp instance.
 * @param id Peer ID to disconnect.
 * @return int 0 on success, -1 on failure.
 */
int chatapp_disconnect_peer(ChatApp *app, int id);

#endif
