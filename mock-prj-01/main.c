#include <stdio.h>
#include <stdlib.h>
#include "chatapp.h"
#include "commands.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <listening_port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    ChatApp app;
    chatapp_setup(&app, atoi(argv[1]));
    chatapp_launch(&app);
    handle_commands(&app);
    chatapp_shutdown(&app);
    return EXIT_SUCCESS;
}
