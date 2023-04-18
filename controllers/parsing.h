#ifndef PARSING_H
#define PARSING_H

#include <stddef.h>
#include <pthread.h>

#include "../utils/dict.h"
#include "../io/celnet.h"

/**
 * @brief Represents a command handler,
 * handles a command for a given client
 * 
 * the call is given:
 * The socket pointer
 * The remote address of the client
 * The size of the remote address of the client
 * The string containing the arguments supplied to the command
 * The length of the argument string
 * 
 */
typedef void (*command_handler)(int, sockaddr_t*, socklen_t, char*, size_t);

extern dict_t command_handler_map;
extern pthread_mutex_t command_mutex;
extern char* command_delimiter;
extern char* command_arg_delimiter;

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
 * @return size_t Returns the number of bytes used by the command
 */
size_t parse_client_buffer(int fd, sockaddr_t* addr, socklen_t len, char* data, size_t data_len);

#endif