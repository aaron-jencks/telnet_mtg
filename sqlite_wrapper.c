#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include "error_handler.h"
#include "entities.h"
#include "sqlite_wrapper.h"

const char* const DB_PATH = "./db.db";

sqlite3 *db;
pthread_mutex_t db_lock = PTHREAD_MUTEX_INITIALIZER;

void db_exec(char* statement) {
    pthread_mutex_lock(&db_lock);
    int rc = sqlite3_open(DB_PATH, &db);
    if (rc) {
        error_at_line(1, ERR_DB, "sqlite_wrapper.c", 12, "Error occured while opening database: %s", sqlite3_errmsg(db));
    } else {
        printf("Opened database successfully!\n");
    }

    char* zErrMsg = 0;
    rc = sqlite3_exec(db, statement, NULL, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        error_at_line(1, ERR_DB, "sqlite_wrapper.c", 20, "SQL error: %s\nIn statement: %s", zErrMsg, statement);
    } else {
        printf("Statement run successfully!\n");
    }

    sqlite3_close(db);
    pthread_mutex_unlock(&db_lock);
}

void db_exec_w_callback(char* statement, int callback (void*, int, char**, char**)) {
    pthread_mutex_lock(&db_lock);
    int rc = sqlite3_open(DB_PATH, &db);
    if (rc) {
        error_at_line(1, ERR_DB, "sqlite_wrapper.c", 38, "Error occured while opening database: %s", sqlite3_errmsg(db));
    } else {
        printf("Opened database successfully!\n");
    }

    char* zErrMsg = 0;
    rc = sqlite3_exec(db, statement, callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        error_at_line(1, ERR_DB, "sqlite_wrapper.c", 45, "SQL error: %s\nIn statement: %s", zErrMsg, statement);
    } else {
        printf("Statement run successfully!\n");
    }

    sqlite3_close(db);
    pthread_mutex_unlock(&db_lock);
}

void create_tables() {
    db_exec("create table if not exists cards (id text primary key, name text unique not null, manacost text not null, type_string text not null, oracle_text text, power text, toughness text, loyalty text)");
    db_exec("create table if not exists players (pid integer primary key autoincrement, username text not null unique, password text not null)");
    db_exec("create table if not exists deck_names (deck_id integer primary key autoincrement, player_id integer references players (pid), name text not null)");
    db_exec("create table if not exists decks (deck_id integer not null references deck_names (deck_id) on delete cascade, card_id text not null references cards (name) on delete cascade, quantity integer not null default 1)");
}