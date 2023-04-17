#include "commands.h"

#include <stdlib.h>

#include "../io/scryfall.h"
#include "../controllers/comm.h"
#include "../controllers/parsing.h"
#include "../utils/dict.h"

void setup_commands() {
    pthread_mutex_lock(&command_mutex);

    dict_put(&command_handler_map, "search", command_search_card);

    pthread_mutex_unlock(&command_mutex);
}

void command_search_card(int fd, sockaddr_t* addr, socklen_t addr_len, char* data, size_t data_len) {
    // TODO figure out why it's not working
    card_search_result_t sr = scryfall_search(data);
    for (size_t ci = 0; ci < sr.len; ci++) {
        char* line = display_card(&sr.cards[ci]);
        comm_write_message(fd, line);
        free(line);
    }
}
