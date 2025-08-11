#include "peer.h"
#include <string.h>

/**
 * @brief Initialize a Peer structure with default values.
 * 
 * Sets all fields to safe initial values:
 *  - id = 0
 *  - sockfd = -1 (invalid socket)
 *  - port = 0
 *  - thread_id = 0
 *  - active = 0 (inactive peer)
 *  - ip[] = empty string
 * 
 * @param p Pointer to the Peer structure to initialize.
 */
void peer_init(Peer *p) {
    p->id = 0;
    p->sockfd = -1;
    p->port = 0;
    p->thread_id = 0;
    p->active = 0;
    memset(p->ip, 0, sizeof(p->ip));
}

/**
 * @brief Set the properties of a Peer structure.
 * 
 * Assigns the provided values to the peer and marks it as active.
 * Ensures the IP address is safely copied and null-terminated.
 * 
 * @param p Pointer to the Peer structure to modify.
 * @param id Unique identifier for the peer.
 * @param sockfd Socket file descriptor associated with the peer.
 * @param ip String containing the peer's IPv4 address.
 * @param port Port number the peer is connected to.
 * @param tid Thread ID handling communication with the peer.
 */
void peer_set(Peer *p, int id, int sockfd, const char *ip, int port, pthread_t tid) {
    p->id = id;
    p->sockfd = sockfd;
    p->port = port;
    p->thread_id = tid;
    p->active = 1;
    strncpy(p->ip, ip, sizeof(p->ip));
    p->ip[sizeof(p->ip) - 1] = '\0'; // Ensure null-termination
}
