#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "net.h"
#include "error_handler.h"

size_t determine_char_slength(uint8_t i) {
    if (i >= 100) return 3;
    if (i >= 10) return 2;
    return 1;
}

size_t determine_short_slength(uint16_t i) {
    if (i >= 10000) return 5;
    if (i >= 1000) return 4;
    if (i >= 100) return 3;
    if (i >= 10) return 2;
    return 1;
}

char* display_ip(struct sockaddr_in* addr, socklen_t addrlen) {
    uint32_t ip = ntohl(addr->sin_addr.s_addr);
    uint8_t b1 = (uint8_t)(ip >> 24), b2 = (uint8_t)(ip >> 16), b3 = (uint8_t)(ip >> 8), b4 = (uint8_t)ip;
    uint16_t port = ntohs(addr->sin_port);
    size_t slen = 5;

    // Determine length of string
    slen += determine_char_slength(b1);
    slen += determine_char_slength(b2);
    slen += determine_char_slength(b3);
    slen += determine_char_slength(b4);
    slen += determine_short_slength(port);

    char* return_value = malloc(sizeof(char)*slen);
    handle_memory_error("net.c", 36, return_value);

    return_value[--slen] = 0;
    snprintf(return_value, sizeof(char)*slen, "%hhd.%hhd.%hhd.%hhd:%hd", b1, b2, b3, b4, port);

    return return_value;
}
