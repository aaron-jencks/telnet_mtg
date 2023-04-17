#ifndef UI_CONT_H
#define UI_CONT_H

#include <pthread.h>

#include "../entities/entities.h"
#include "../utils/dict.h"
#include "../ui.h"

typedef struct {
    player_t* player;
    window_controller_t ui_stack;
} ui_stack_t;

extern dict_t ui_table;
extern pthread_mutex_t ui_mutex;

void initialize_ui_controller();

char* ui_interact(player_t* player, char* entry);
char* ui_add_player(player_t* player, window_t* initial_window);
void ui_remove_player(player_t* player);

#endif