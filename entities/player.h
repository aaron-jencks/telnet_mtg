#ifndef PLAYER_H
#define PLAYER_H

#include "entities.h"

player_t create_player(char* username, char* password);
player_t retrieve_player(char* username);
char player_exists(char* username);

#endif