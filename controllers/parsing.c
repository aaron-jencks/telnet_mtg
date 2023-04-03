#include <error.h>

#include "parsing.h"

dict_t command_handler_map;
pthread_mutex_t command_mutex;

void setup_handler_map() {
    create_dict(10, );
    if (pthread_mutex_init(&command_mutex, NULL);) {

    }
}

size_t parse_client_buffer(int fd, sockaddr_t* addr, socklen_t len, char* data, size_t data_len) {
    size_t returnValue;
    return returnValue;
}
