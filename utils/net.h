#ifndef NET_UTILS_H
#define NET_UTILS_H

#include <netinet/in.h>

/**
 * @brief Takes in a sockaddr_t* addr and converts it into a network://ip:port string
 * 
 * @param addr The address to convert
 * @param addrlen the size of the address in bytes
 * @return char* A string of the form network://ip:port, must be free'd by the caller
 */
char* display_ip(struct sockaddr_in* addr, socklen_t addrlen);

#endif