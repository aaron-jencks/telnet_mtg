#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>

#include "utils/urlencode.h"
#include "io/sqlite_wrapper.h"
#include "io/telnet.h"
#include "io/celnet.h"
#include "entities/entities.h"
#include "entities/player.h"
#include "commands/commands.h"
#include "controllers/parsing.h"

void catchkill(int signo) {
    if (signo == SIGINT) {
        shutdown(celnet_socketfd, SHUT_RDWR);
    }
}

int main(int argc, char *argv[]) {
    // Setup the database tables
    create_tables();
    
    // setup encoder
    url_encoder_rfc_tables_init();

    // setup command handlers
    setup_handler_map();
    setup_commands();

    // Create a seed player
    player_t p;
    if (!player_exists("iggy12345")) {
        printf("The player did not exist!\n");
    } else {
        printf("The player existed!\n");
        p = retrieve_player("iggy12345");
        printf("Player:\n\tName: %s\n\tPassword: %s\n", p.username, p.password);
    }

    // Start the telnet listener
    launch_telnet_server(23);
    return 0;
}
