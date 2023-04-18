#include "commands.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../io/scryfall.h"
#include "../controllers/comm.h"
#include "../controllers/parsing.h"
#include "../utils/dict.h"
#include "../utils/error_handler.h"

void setup_commands() {
    pthread_mutex_lock(&command_mutex);

    dict_put(&command_handler_map, "search", command_search_card);
    dict_put(&command_handler_map, "info", command_info_card);

    pthread_mutex_unlock(&command_mutex);
}

void command_search_card(int fd, sockaddr_t* addr, socklen_t addr_len, char* data, size_t data_len) {
    // TODO figure out why it's not working
    printf("Searching for %s\n", data);
    card_search_result_t sr = scryfall_search(data);
    if (sr.len) {
        for (size_t ci = 0; ci < sr.len; ci++) {
            char* name = sr.cards[ci].name;
            char* cmc = sr.cards[ci].manacost;
            char* print_buff = malloc(sizeof(char) * (7 + strlen(name) + strlen(cmc)));
            handle_memory_error("commands.c", 29, print_buff);
            sprintf(print_buff, "%ld: %s %s\r\n", ci, name, cmc);
            comm_write_message(fd, print_buff);
            free(print_buff);
            delete_card(sr.cards[ci]);
        }
    } else {
        comm_write_message(fd, "No cards matched your search :(\r\n");
    }
}

void command_info_card(int fd, sockaddr_t* addr, socklen_t addr_len, char* data, size_t data_len) {
    // TODO figure out why it's not working
    printf("Searching for %s\n", data);
    card_t sr = scryfall_find(data);
    char* line = display_card(&sr);
    char* pbuff = malloc(sizeof(char) * (strlen(line) + 2));
    handle_memory_error("commands.c", 46, pbuff);
    sprintf(pbuff, "%s\r\n", line);
    comm_write_message(fd, pbuff);
    free(pbuff);
    free(line);
    delete_card(sr);
}
