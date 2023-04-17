#ifndef COMMANDS_LIST_H
#define COMMANDS_LIST_H

#include <stddef.h>
#include <pthread.h>

#include "../io/celnet.h"

// /**
//  * @brief Represents a command handler,
//  * handles a command for a given client
//  * 
//  * the call is given:
//  * The socket pointer
//  * The remote address of the client
//  * The size of the remote address of the client
//  * The string containing the arguments supplied to the command
//  * The length of the argument string
//  * 
//  */
// typedef void (*command_handler)(int, sockaddr_t*, socklen_t, char*, size_t);

void setup_commands();

void command_search_card(int fd, sockaddr_t* addr, socklen_t addr_len, char* data, size_t data_len);

#endif