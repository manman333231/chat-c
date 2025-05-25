#ifndef COMMON_SETUP_H
#define COMMON_SETUP_H

#include <netdb.h>

void setup_addr(struct addrinfo* hints, struct addrinfo** res, char* IP, char* PORT);

int create_socket();

#endif
