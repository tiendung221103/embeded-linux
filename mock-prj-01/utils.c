#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

void get_local_ip(void) {
    int tmp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (tmp_sock < 0) {
        perror("socket");
        return;
    }
    struct sockaddr_in temp_addr;
    memset(&temp_addr, 0, sizeof(temp_addr));
    temp_addr.sin_family = AF_INET;
    temp_addr.sin_port = htons(80);
    inet_pton(AF_INET, "8.8.8.8", &temp_addr.sin_addr);
    connect(tmp_sock, (struct sockaddr *)&temp_addr, sizeof(temp_addr));

    struct sockaddr_in local_addr;
    socklen_t len = sizeof(local_addr);
    if (getsockname(tmp_sock, (struct sockaddr *)&local_addr, &len) < 0) {
        perror("getsockname");
        close(tmp_sock);
        return;
    }

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &local_addr.sin_addr, ip, sizeof(ip));
    printf("Local IP: %s\n", ip);
    close(tmp_sock);
}
