#ifndef ENTITIES_H
#define ENTITIES_H

typedef struct {
    int pid;
    char* username;
    char* password;
} player_t;

typedef struct {
    char* card_name;
    int quantity;
} deck_entry_t;

typedef struct {
    int player;
    char* name;
    deck_entry_t* entries;
} deck_t;

#endif