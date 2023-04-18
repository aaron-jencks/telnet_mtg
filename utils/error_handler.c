#include <error.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "error_handler.h"

const int ERR_MEMORY = 0;
const int ERR_DB = 1;
const int ERR_SCRYFALL = 2;
const int ERR_DICT_SIZE = 3;
const int ERR_MUTEX_INIT = 4;
const int ERR_PTHREAD = 5;

void handle_memory_error(const char* filename, unsigned int linenum, const void* const ptr) {
    if (!ptr) error_at_line(1, ERR_MEMORY, filename, linenum, "Out of Memory!");
}

void handle_socket_write_error(const char* filename, unsigned int linenum) {
    char* err;
    switch(errno) {
        case EACCES:
            err = "EACCES";
            break;
        case EAGAIN:
            err = "EAGAIN";
            break;
        case EALREADY:
            err = "EALREADY";
            break;
        case EBADF:
            err = "EBADF";
            break;
        case ECONNRESET:
            err = "ECONNRESET";
            break;
        case EDESTADDRREQ:
            err = "EDESTADDRREQ";
            break;
        case EFAULT:
            err = "EFAULT";
            break;
        case EINTR:
            err = "EINTR";
            break;
        case EINVAL:
            err = "EINVAL";
            break;
        case EISCONN:
            err = "EISCONN";
            break;
        case EMSGSIZE:
            err = "EMSGSIZE";
            break;
        case ENOBUFS:
            err = "ENOBUFS";
            break;
        case ENOMEM:
            err = "ENOMEM";
            break;
        case ENOTCONN:
            err = "ENOTCONN";
            break;
        case ENOTSOCK:
            err = "ENOTSOCK";
            break;
        case EOPNOTSUPP:
            err = "EOPNOTSUPP";
            break;
        case EPIPE:
            err = "EPIPE";
            break;
        default:
            err = "UNKNOWN";
            break;
    }
    fprintf(stderr, "WARN %s:%d Failed to send message to client: %d (%s)\n", filename, linenum, errno, err);
}