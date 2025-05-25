#ifndef SERVER_SETUP_H
#define SERVER_SETUP_H

#include <netdb.h>

void enable_reuse_addr(int sockfd);

void bind_and_listen(int sockfd, struct addrinfo* res);

#endif
