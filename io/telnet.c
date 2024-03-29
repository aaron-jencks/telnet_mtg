#include <netinet/in.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdbool.h>

#include "telnet.h"
#include "celnet.h"
#include "../utils/error_handler.h"
#include "../utils/net.h"
#include "../controllers/parsing.h"

size_t connection_hashing_function(void* key) {
    sockaddr_t* addr = (sockaddr_t*)key;
    return (addr->sin_addr.s_addr << 16) + addr->sin_port;
}

dict_t current_connections;
dict_t current_sockets;
pthread_mutex_t connection_lock;

telnet_client_t* create_client(size_t buff_size) {
    telnet_client_t* client = (telnet_client_t*)malloc(sizeof(telnet_client_t));
    handle_memory_error("telnet.c", 22, client);
    client->buffer = (char*)malloc(sizeof(char)*buff_size);
    handle_memory_error("telnet.c", 24, client->buffer);
    client->pointer = 0;
    client->capacity = buff_size;
    return client;
}

void delete_client(telnet_client_t client) {
    if(client.buffer) free(client.buffer);
}

void client_append_data(telnet_client_t* client, char* data, size_t data_len) {
    if (client->pointer + data_len > client->capacity) {
        client->buffer = (char*)realloc((void*)client->buffer, sizeof(char) * (client->capacity + data_len + 10));
        handle_memory_error("telnet.c", 33, client->buffer);
        client->capacity += data_len + 10;
    }
    memcpy((void*)(client->buffer + client->pointer), (void*)data, data_len);
    client->pointer += data_len;
}

char client_read_char(telnet_client_t* client) {
    if (!client->pointer) return 0;
    char c = client->buffer[0];
    for (size_t ci = 1; ci < client->pointer; ci++) {
        client->buffer[ci-1] = client->buffer[ci];
    }
    client->pointer--;
    return c;
}

void client_reput_char(telnet_client_t* client, char c) {
    if (client->pointer + 1 > client->capacity) {
        client->buffer = (char*)realloc((void*)client->buffer, sizeof(char) * (client->capacity + 11));
        handle_memory_error("telnet.c", 53, client->buffer);
        client->capacity += 11;
    }
    for (size_t ci = client->pointer++; ci > 0; ci--) {
        client->buffer[ci] = client->buffer[ci-1];
    }
    client->buffer[0] = c;
}

char client_peek_char(telnet_client_t* client) {
    if (!client->pointer) return 0;
    return client->buffer[0];
}

void client_pop_count(telnet_client_t* client, size_t num) {
    if (num) {
        if (num < client->pointer) {
            client->pointer -= num;
        } else {
            client->pointer = 0;
        }
    }
}

char* client_buffer_string(telnet_client_t* client) {
    char* result = (char*)malloc(sizeof(char)*(client->pointer+1));
    handle_memory_error("telnet.c", 79, result);
    result = memcpy(result, client->buffer, client->pointer);
    result[client->pointer] = 0;
    return result;
}

void telnet_connection_handler(int fd, sockaddr_t* addr, socklen_t len, char* data, size_t data_len) {
    pthread_mutex_lock(&connection_lock);
    telnet_client_t* client;
    if (!dict_contains(current_connections, (void*)addr)) {
        client = create_client(1024);
        client_append_data(client, data, data_len);
        dict_put(&current_connections, (void*)addr, (void*)client);
    } else {
        client = (telnet_client_t*)dict_get(current_connections, (void*)addr);
        client_append_data(client, data, data_len);
    }
    char* cbuff = client_buffer_string(client);
    size_t bused = parse_client_buffer(fd, addr, len, cbuff, strlen(cbuff));
    free(cbuff);
    client_pop_count(client, bused);
    pthread_mutex_unlock(&connection_lock);
}

typedef struct {
    pthread_t cthread;
    int fd;
    sockaddr_t* addr;
    socklen_t addrlen;
} disconnection_args_t;

void* client_disconnection_listener(void* args) {
    disconnection_args_t* dargs = (disconnection_args_t*)args;
    int dret = pthread_join(dargs->cthread, NULL);
    if (dret) {
        error_at_line(ERR_PTHREAD, dret, "telnet.c", 113, "Failed to join client thread, failed with code: %d", dret);
    }
    char* daddrstring = display_ip(dargs->addr, dargs->addrlen);
    printf("Client %s disconnected...\n", daddrstring);
    pthread_mutex_lock(&connection_lock);
    telnet_client_t* conn = (telnet_client_t*)dict_remove(&current_connections, dargs->addr);
    if(conn) {
        delete_client(*conn);
        free(conn);
    }
    int* fdp = (int*)dict_remove(&current_sockets, dargs->addr);
    shutdown(*fdp, SHUT_RDWR);
    if(fdp) free(fdp);
    pthread_mutex_unlock(&connection_lock);
    free(daddrstring);
    free(dargs->addr);\
    printf("Freed caddr\n");
    free(dargs);
    return NULL;
}

void client_cleanup(pthread_t cthread, int fd, sockaddr_t* addr, socklen_t addrlen) {
    char* daddrstring = display_ip(addr, addrlen);
    printf("Client %s connected...\n", daddrstring);
    free(daddrstring);

    int* fdp = malloc(sizeof(int));
    handle_memory_error("telnet.c", 132, fdp);
    *fdp = fd;

    pthread_mutex_lock(&connection_lock);
    dict_put(&current_sockets, addr, fdp);
    pthread_mutex_unlock(&connection_lock);

    disconnection_args_t* args = (disconnection_args_t*)malloc(sizeof(disconnection_args_t));
    handle_memory_error("telnet.c", 128, args);
    args->cthread = cthread;
    args->fd = fd;
    args->addr = addr;
    args->addrlen = addrlen;

    pthread_t dthread;
    int cret;

    cret = pthread_create(&dthread, NULL, client_disconnection_listener, (void*)args);
    if (cret) {
        error_at_line(ERR_PTHREAD, cret, "telnet.c", 138, "Failed to launch pthread, failed with code: %d", cret);
    }
}

void telnet_set_socket_options(int fd) {
    int yes = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
}

void launch_telnet_server(uint16_t port) {
    current_connections = create_dict(10, connection_hashing_function);
    current_sockets = create_dict(10, connection_hashing_function);
    if (pthread_mutex_init(&connection_lock, NULL)) {
        error_at_line(ERR_MUTEX_INIT, 0, "telnet.c", 146, "Failed to initialize mutex for connection lock");
    }

    server_def_t definition = create_server_defaults();
    definition.address.sin_port = htons(port);
    definition.connection_handler = telnet_connection_handler;
    definition.thread_handler = client_cleanup;
    definition.options_callback = telnet_set_socket_options;

    server_listen_and_serve(definition);

    pthread_mutex_destroy(&connection_lock);
    destroy_dict(current_connections);
    destroy_dict(current_sockets);
}

void shutdown_telnet_server() {
    pthread_mutex_lock(&connection_lock);
    arraylist_t conns = dict_to_list(current_sockets, false);
    for(size_t ci = 0; ci < conns.count; ci++) {
        shutdown(*(int*)(((key_value_pair_t*)arraylist_index(conns, ci))->value), SHUT_RDWR);
    }
    pthread_mutex_unlock(&connection_lock);
    shutdown(celnet_socketfd, SHUT_RDWR);
}
