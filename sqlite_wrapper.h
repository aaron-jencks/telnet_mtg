#ifndef SQLITE_WRAPPER_H
#define SQLITE_WRAPPER_H

#include <pthread.h>
#include <sqlite3.h>

extern const char* const DB_PATH;

extern sqlite3 *db;
extern pthread_mutex_t db_lock;

/**
 * Executes a sql statement in the database
 * Use for statements where the result does not matter
*/
void db_exec(char* statement);

/**
 * Executes a sql statement in the database
 * Use for statements where the result does matter
*/
void db_exec_w_callback(char* statement, int callback (void*, int, char**, char**));

/**
 * Initializes the sql tables for the database
*/
void create_tables();

#endif