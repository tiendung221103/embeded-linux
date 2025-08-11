#include "commands.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CMD_LEN 256
#define MAX_MSG_LEN 100

void show_help(void) {
    printf("Commands:\n");
    printf("help               - Show this help\n");
    printf("myip               - Show local IP\n");
    printf("myport             - Show listening port\n");
    printf("connect <ip> <port>- Connect to peer\n");
    printf("list               - List connections\n");
    printf("send <id> <msg>    - Send message\n");
    printf("terminate <id>     - End connection\n");
    printf("exit               - Exit program\n");
}

void handle_commands(ChatApp *app) {
    char command[MAX_CMD_LEN];
    while (1) {
        printf("> ");
        fflush(stdout);

        if (!fgets(command, sizeof(command), stdin))
            continue;
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "help") == 0) {
            show_help();
        } else if (strcmp(command, "myip") == 0) {
            get_local_ip();
        } else if (strcmp(command, "myport") == 0) {
            printf("Listening port: %d\n", app->server_port);
        } else if (strncmp(command, "connect ", 8) == 0) {
            char ip[INET_ADDRSTRLEN];
            int port;
            if (sscanf(command + 8, "%15s %d", ip, &port) != 2) {
                printf("Usage: connect <ip> <port>\n");
                continue;
            }
            chatapp_connect_peer(app, ip, port);
        } else if (strcmp(command, "list") == 0) {
            chatapp_list_peers(app);
        } else if (strncmp(command, "send ", 5) == 0) {
            int id;
            char msg[MAX_MSG_LEN + 1];
            if (sscanf(command + 5, "%d %[^\n]", &id, msg) != 2) {
                printf("Usage: send <id> <message>\n");
                continue;
            }
            chatapp_send_message(app, id, msg);
        } else if (strncmp(command, "terminate ", 10) == 0) {
            int id;
            if (sscanf(command + 10, "%d", &id) != 1) {
                printf("Usage: terminate <id>\n");
                continue;
            }
            chatapp_disconnect_peer(app, id);
        } else if (strcmp(command, "exit") == 0) {
            printf("Exiting and closing all connections...\n");
            chatapp_shutdown(app);
            exit(0);
        } else {
            printf("Unknown command. Type 'help'.\n");
        }
    }
}
