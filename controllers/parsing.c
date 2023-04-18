#include <error.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "parsing.h"
#include "../utils/error_handler.h"
#include "../utils/hashing.h"

dict_t command_handler_map;
pthread_mutex_t command_mutex;
char* command_delimiter = "\r\n";
char* command_arg_delimiter = " ";

int strcmparator(void* a, void* b) {
    return strcmp((char*)a, (char*)b);
}

void setup_handler_map() {
    command_handler_map = create_dict(10, hash_string_prime);
    command_handler_map.key_comparing_func = strcmparator;
    if (pthread_mutex_init(&command_mutex, NULL)) {
        error_at_line(ERR_MUTEX_INIT, ERR_MUTEX_INIT, "parsing.c", 22, "unable to initialize parsing mutex");
    }
}

size_t parse_client_buffer(int fd, sockaddr_t* addr, socklen_t len, char* data, size_t data_len) {
    size_t offset;
    for (offset = 0; offset < data_len;) {
        size_t line_len = 0;
        char* tdata = data + offset;
        bool found = false;

        for(size_t i = 0; i < (data_len - offset) && !found; i++) {
            for(char* di = command_delimiter; *di; di++) {
                if (tdata[i] == *di) {
                    line_len = i;
                    found = true;
                    break;
                }
            }
        }

        if (found && line_len) {
            // We actually found a command
            tdata[line_len] = 0; // resize the string
            printf("executing command '%s'\n", tdata);

            // extract the command;
            size_t command_len = strcspn(tdata, command_arg_delimiter);
            if (command_len < (data_len - offset)) tdata[command_len++] = 0; // resize the string temporarily

            // fetch the handler
            pthread_mutex_lock(&command_mutex);
            command_handler handler = dict_get(command_handler_map, tdata);
            pthread_mutex_unlock(&command_mutex);
            if (handler) {
                handler(fd, addr, len, tdata + command_len, line_len - (command_len + 2));
            }

            // move the offset
            offset += line_len + 1; // move past the line delimiter
            
            // TODO send a message to the client that the command is unrecognized
        } else if (!found) {
            return offset;
        } else {
            offset++;
        }
    }
}
