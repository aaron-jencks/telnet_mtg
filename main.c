#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scryfall.h"
#include "urlencode.h"
#include "sqlite_wrapper.h"
#include "entities.h"
#include "player.h"


int main(int argc, char *argv[]) {
    create_tables();
    url_encoder_rfc_tables_init();
    player_t p;
    if (!player_exists("iggy12345")) {
        printf("The player did not exist!\n");
    } else {
        printf("The player existed!\n");
        p = retrieve_player("iggy12345");
        printf("Player:\n\tName: %s\n\tPassword: %s\n", p.username, p.password);
    }
    return 0;
}
