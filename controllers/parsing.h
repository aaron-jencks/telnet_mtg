#ifndef PARSING_H
#define PARSING_H

#include <netinet/in.h>
#include <pthread.h>

#include "../utils/dict.h"

extern dict_t command_handler_map;
extern pthread_mutex_t command_mutex;

/**
 * @brief Set the up handler map object and the corresponding mutex
 * 
 */
void setup_handler_map();

/**
 * @brief Parses a client's buffer for commands
 * 
 * @param fd the socket pointer
 * @param addr the remote address of the client
 * @param len the size of the remote address of the client
 * @param data the buffer for the client
 * @param data_len the length of the buffer for the client
 * @return size_t 
 */
size_t parse_client_buffer(int fd, sockaddr_t* addr, socklen_t len, char* data, size_t data_len);

#endif