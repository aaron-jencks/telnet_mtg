#include "sqlite_wrapper.h"
#include "error_handler.h"
#include "player.h"
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>

size_t sqlresponse_count = 0, sqlresponse_capacity = 0;
player_t* sql_responses = 0;
pthread_mutex_t player_lock = PTHREAD_MUTEX_INITIALIZER;

static int handle_player_sql_response(void* not_used, int argc, char** argv, char** col_names) {
    player_t* temp = malloc(sizeof(player_t));
    handle_memory_error("player.c", 15, temp);

    for (size_t i = 0; i < argc; i++) {
        if (!strcmp(col_names[i], "pid")) {
            sscanf(argv[i], "%d", &temp->pid);
        } else if (!strcmp(col_names[i], "username")) {
            temp->username = strdup(argv[i]);
        } else if (!strcmp(col_names[i], "password")) {
            temp->password = strdup(argv[i]);
        } else {
            error_at_line(4, ERR_DB, "player.c", 26, "unknown column: %s", col_names[i]);
        }
    }

    pthread_mutex_lock(&player_lock);
    if (sqlresponse_count == sqlresponse_capacity) {
        sqlresponse_capacity += 10;
        sql_responses = realloc(sql_responses, sizeof(player_t) * sqlresponse_capacity);
        handle_memory_error("player.c", 33, sql_responses);
    }
    sql_responses[sqlresponse_count++] = *temp;
    pthread_mutex_unlock(&player_lock);
    free(temp);
}

void reset_player_responses() {
    pthread_mutex_lock(&player_lock);
    sqlresponse_count = 0;
    sqlresponse_capacity = 0;
    free(sql_responses);
    sql_responses = 0;
    pthread_mutex_unlock(&player_lock);
}

player_t create_player(char* username, char* password) {
    char* insertion_template = "insert into players (username, password) values (\"%s\", \"%s\");";

    size_t exp_len = strlen(insertion_template) - 4 + strlen(username) + strlen(password);
    char* exp = malloc(sizeof(char) * (exp_len + 1));
    exp[exp_len] = 0;
    sprintf(exp, insertion_template, username, password);

    db_exec(exp);
    free(exp);

    return retrieve_player(username);
}

player_t retrieve_player(char* username) {
    char* selection_template = "select * from players where username=\"%s\";";

    size_t exp_len = strlen(selection_template) - 2 + strlen(username);
    char* exp = malloc(sizeof(char) * (exp_len + 1));
    exp[exp_len] = 0;
    sprintf(exp, selection_template, username);

    reset_player_responses();
    db_exec_w_callback(exp, handle_player_sql_response);
    free(exp);

    if (sqlresponse_count)
        return sql_responses[0];

    player_t not_found;
    not_found.pid = 0;
    not_found.username = 0;
    not_found.password = 0;
    return not_found;
}

char player_exists(char* username) {
    char* selection_template = "select pid from players where username=\"%s\";";

    size_t exp_len = strlen(selection_template) - 2 + strlen(username);
    char* exp = malloc(sizeof(char) * (exp_len + 1));
    exp[exp_len] = 0;
    sprintf(exp, selection_template, username);

    reset_player_responses();
    db_exec_w_callback(exp, handle_player_sql_response);
    free(exp);

    return sqlresponse_count > 0;
}