#ifndef USER_CONT_H
#define USER_CONT_H

#include <pthread.h>

#include "../entities/entities.h"
#include "../utils/dict.h"
#include "../io/celnet.h"

typedef struct {
    player_t* player;

    int fd;
    sockaddr_t* addr;
    socklen_t addr_len;

    char* room_name;
    deck_t* selected_deck;
} user_t;

extern dict_t user_table;
extern pthread_mutex_t user_mutex;

void initialize_user_controller();

player_t* user_add_connection(int fd, sockaddr_t* addr, socklen_t addr_len, char* username);
void user_remove_connection(int fd);

#endif