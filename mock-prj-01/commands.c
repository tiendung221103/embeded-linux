#include "commands.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CMD_LEN 256
#define MAX_MSG_LEN 100

// Function to display the list of supported commands to the user
void print_help() {
    printf("Available commands:\n");
    printf("help                         : Display this help message\n");
    printf("myip                         : Display the IP address of this process\n");
    printf("myport                       : Display the port on which this process is listening\n");
    printf("connect <destination> <port> : Connect to the specified IP and port\n");
    printf("list                         : List all active connections\n");
    printf("send <connection id> <msg>   : Send a message to a specific connection\n");
    printf("terminate <connection id>    : Terminate a specific connection\n");
    printf("exit                         : Close all connections and terminate the program\n");
}

// Function to continuously read and handle commands entered by the user
void handle_commands(ChatApp *app) {
    char command[MAX_CMD_LEN]; // Buffer to store the user command

    while (1) {
        printf(">> "); // Prompt for user input
        if (fgets(command, sizeof(command), stdin) == NULL) { // Read a line from stdin
            continue; // If reading failed, skip to next iteration
        }

        // Remove trailing newline character if present
        command[strcspn(command, "\n")] = 0;

        // Check for each command and execute the corresponding action
        if (strcmp(command, "help") == 0) {
            print_help(); // Show available commands

        } else if (strcmp(command, "myip") == 0) {
           get_local_ip(); // Display local IP address

        } else if (strcmp(command, "myport") == 0) {
            printf("Listening on port: %d\n", app->server_port); // Display listening port

        } else if (strncmp(command, "connect ", 8) == 0) {
            char ip[64];
            int port;
            // Parse the IP and port from the command string
            if (sscanf(command + 8, "%63s %d", ip, &port) == 2) {
                connect_to_peer(app, ip, port); // Attempt to connect to the peer
            } else {
                printf("Usage: connect <destination> <port>\n"); // Show usage if parsing failed
            }

        } else if (strcmp(command, "list") == 0) {
            list_connections(app); // List all active peer connections

        } else if (strncmp(command, "send ", 5) == 0) {
            int id;
            char msg[MAX_MSG_LEN];
            // Parse connection ID and message from the command string
            if (sscanf(command + 5, "%d %[^\n]", &id, msg) == 2) {
                send_message(app, id, msg); // Send the message to the selected peer
            } else {
                printf("Usage: send <connection id> <message>\n"); // Show usage if parsing failed
            }

        } else if (strncmp(command, "terminate ", 10) == 0) {
            int id;
            // Parse connection ID from the command string
            if (sscanf(command + 10, "%d", &id) == 1) {
                terminate_connection(app, id); // Terminate the specified connection
            } else {
                printf("Usage: terminate <connection id>\n"); // Show usage if parsing failed
            }

        } else if (strcmp(command, "exit") == 0) {
            close_all_connections(app); // Close all connections before exiting
            exit(0); // Terminate the program

        } else {
            // If the command does not match any known command
            printf("Unknown command. Type 'help' for a list of commands.\n");
        }
    }
}
