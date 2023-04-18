#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>

#include "comm.h"
#include "../utils/error_handler.h"

void comm_write_message(int fd, char* message) {
    if(send(fd, message, strlen(message), 0) < 0) {
        handle_socket_write_error("comm.c", 10);
    }
}
