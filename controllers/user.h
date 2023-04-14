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
} user_t;

extern dict_t user_table;
extern pthread_mutex_t user_mutex;

#endif