#include "user.h"
#include "../utils/error_handler.h"
#include "../entities/player.h"

dict_t user_table;
pthread_mutex_t user_mutex;

size_t hash_user(void* entry) {
    return (size_t)*(int*)entry;
}

void initialize_user_controller() {
    user_table = create_dict(10, hash_user);
    if (pthread_mutex_init(&command_mutex, NULL)) {
        error_at_line(ERR_MUTEX_INIT, ERR_MUTEX_INIT, "user.c", 13, "unable to initialize parsing mutex");
    }
}

player_t* user_add_connection(int fd, sockaddr_t* addr, socklen_t addr_len, char* username) {
    pthread_mutex_lock(&user_mutex);

    if (dict_contains(&user_table, &fd)) {
        return ((user_t*)dict_get(&user_table, &fd))->player;
    }

    player_t* returnValue = malloc(sizeof(player_t));
    handle_memory_error("user.c", 19, returnValue);

    user_t* user_entry = malloc(sizeof(user_t));
    handle_memory_error("user.c", 23, user_entry);
    user_entry->selected_deck = NULL;
    user_entry->fd = fd;
    user_entry->addr = addr;
    user_entry->room_name = NULL;

    returnValue* = retrieve_player(username);
    user_entry->player = returnValue;

    dict_put(&user_table, &fd, user_entry);

    pthread_mutex_unlock(&user_mutex);

    return returnValue;
}

void user_remove_connection(int fd) {
    if (!dict_contains(&user_table, &fd)) return;
    user_t* entry = (user_t*)dict_get(&user_table, &fd);
    
}
