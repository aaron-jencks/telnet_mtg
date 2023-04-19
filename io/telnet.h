#ifndef TELNET_H
#define TELNET_H

#include <stdint.h>
#include <stddef.h>
#include <pthread.h>

#include "../utils/dict.h"

extern dict_t current_connections;
extern pthread_mutex_t connection_lock;

typedef struct {
    char* buffer;
    size_t pointer;
    size_t capacity;
} telnet_client_t;

/**
 * Launches the telnet server and listens for connections
*/
void launch_telnet_server(uint16_t port);

/**
 * @brief shuts down all sockets associated with the telnet server
 * This causes the server to exit
 * 
 */
void shutdown_telnet_server();

#endif