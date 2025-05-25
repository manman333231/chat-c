#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "common/common_setup.h"

void setup_addr(struct addrinfo* hints, struct addrinfo** res, char* IP, char* PORT) {
    memset(hints, 0, sizeof *hints);

    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM;

    if (getaddrinfo(IP, PORT, hints, res) == -1) {
        perror("getaddrinfo error!");
        exit(1);
    }
}

int create_socket() {
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(1);
    }

    return sockfd;
}
