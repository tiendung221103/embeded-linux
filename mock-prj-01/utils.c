#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

/**
 * @brief Retrieve and print the local machine's IP address.
 * 
 * This function creates a temporary UDP socket and connects it to a known 
 * external IP address (Google DNS: 8.8.8.8). The connection is not actually 
 * sending any data, but it allows us to determine the local IP address 
 * used for outbound connections.
 */
void get_local_ip(void) {
    // Create a UDP socket
    int tmp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (tmp_sock < 0) {
        perror("socket"); // Print error if socket creation fails
        return;
    }

    // Prepare the destination address (Google DNS at port 80)
    struct sockaddr_in temp_addr;
    memset(&temp_addr, 0, sizeof(temp_addr));
    temp_addr.sin_family = AF_INET;
    temp_addr.sin_port = htons(80);
    inet_pton(AF_INET, "8.8.8.8", &temp_addr.sin_addr);

    // Connect to the destination to determine the local IP
    connect(tmp_sock, (struct sockaddr *)&temp_addr, sizeof(temp_addr));

    // Retrieve local address used in the connection
    struct sockaddr_in local_addr;
    socklen_t len = sizeof(local_addr);
    if (getsockname(tmp_sock, (struct sockaddr *)&local_addr, &len) < 0) {
        perror("getsockname"); // Print error if retrieval fails
        close(tmp_sock);
        return;
    }

    // Convert the local IP address to a readable string
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &local_addr.sin_addr, ip, sizeof(ip));

    // Display the local IP
    printf("Local IP: %s\n", ip);

    // Close the temporary socket
    close(tmp_sock);
}
