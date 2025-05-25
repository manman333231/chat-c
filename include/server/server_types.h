#ifndef TYPES_H
#define TYPES_H

#include <pthread.h>
#include <sys/socket.h>

#include "server/server_macros.h"

typedef struct {
    int sockfd;
    struct sockaddr addr;
    char name[MAX_NAME];
} client;

typedef struct {
    client* clients[MAX_CLIENTS];
    int count;
    pthread_mutex_t lock;
} clients_data;

typedef struct {
    client* cli;
    clients_data* cli_data;
} thread_arg;

typedef struct {
    char accum[MAX_RECV];
    int accum_len;
} accumulator;

typedef struct {
    char mesg[MAX_RESP];
    int sock_fd;
} response;

#endif
