#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "server/server_macros.h"
#include "server/server_setup.h"

void enable_reuse_addr(int sockfd) {
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt) == -1) {
        perror("setsockopt error");
        exit(1);
    }
}

void bind_and_listen(int sockfd, struct addrinfo* res) {
    if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind error");
        exit(1);
    }

    if (listen(sockfd, MAX_CLIENTS) == -1) {
        perror("listen error");
        exit(1);
    }
}
