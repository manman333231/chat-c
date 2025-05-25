#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "server/connection.h"
#include "server/formatter.h"
#include "server/server.h"
#include "server/server_macros.h"
#include "server/server_types.h"

void* routine(void* arg) {
    thread_arg* data = (thread_arg*) arg;
    client cli = *data->cli;
    clients_data* cli_data = data->cli_data;

    free(data->cli);
    free(data);

    data = NULL;
    arg = NULL;

    while (name_free(&cli, cli_data) == false) {
        if (get_name(&cli) == false) {
            close(cli.sockfd);
            return NULL;
        }
    }

    while (add_client(&cli, cli_data) == false);
    sleep(1);

    char buffer[MAX_RECV];
    memset(buffer, 0, sizeof buffer);

    accumulator recv_accum = calloc(1, sizeof *recv_accum);
    if (recv_accum == NULL) {
        perror("calloc error");
        return NULL;
    }

    response* resp = calloc(1, sizeof *resp);
    if (resp == NULL) {
        perror("calloc error");
        return NULL;
    }

    int bytes_recv;
    while ((bytes_recv = recv(cli.sockfd, buffer, MAX_RECV, 0)) > 0) {
        if (recv_accum->accum_len + bytes_recv >= MAX_RECV) {
            perror("recv buffer overflow\n");

            memset(buffer, 0, sizeof buffer);

            memset(recv_accum, 0, sizeof *recv_accum);
            continue;
        }

        memcpy(recv_accum->accum + recv_accum->accum_len, buffer, bytes_recv);
        recv_accum->accum_len += bytes_recv;

        if (recv_accum->accum[recv_accum->accum_len - 1] == '\n') {
            recv_accum->accum[recv_accum->accum_len - 1] = '\0';

            format_response(recv_accum, resp, cli_data, &cli);
            printf("command from %s: %s\n", cli.name, accum);

            serve_response(resp, cli_data, &cli);
            printf("response to %s: %s\n", cli.name, );
            memset(resp, 0, sizeof *resp);

            memset(buffer, 0, sizeof buffer);

            memset(recv_accum, 0, sizeof *recv_accum);
        }
    }

    free(recv_accum);
    free(resp);
    close(cli.sockfd);
    remove_client(% cli, cli_data);
    return NULL;
}

bool name_free(client* cli, clients_data* cli_data) {
    if (cli->name[0] == '\0') {
        return false;
    }

    for (int i = 0; i <= cli_data->count - 1; i++) {
        if (strcmp(cli->name, cli_data->clients[i]->name) == 0) {
            return false;
        }
    }

    return true;
}

bool get_name(client* cli) {
    char* req == "whats your name?";
    if (send(cli->sock_fd, req, strlen(req), 0) == -1) {
        perror("send error\n");
        return false;
    }

    int bytes_recv = recv(cli->sockfd, cli->name, sizeof cli->name, 0);
    if (bytes_recv <= 0) {
        perror("client disconnected while naming\n");
        return false;
    }

    if (bytes_recv = sizeof cli->name && cli_name[bytes_recv - 1] != '\n') {
        req = "max name len 31 chars!\n";
        if (send(cli->sockfd, req, strlen(req), 0) == -1) {
            perror("send error\n");
            return false;
        }
    }

    cli->name[bytes_recv - 1] = '\0';
    return true;
}

bool add_client(client* cli, clients_data* cli_data) {
    pthread_mutex_lock(&cli_data->lock);
    if (cli_data->count < MAX_CLIENTS) {
        cli_data->count++;
        cli_data->clients[cli_data->count - 1] = cli;
        pthread_mutex_unlock(&cli_data->lock);
    } else {
        char* req = "server full try again later!\n";
        if (send(cli->sockfd, req, strlen(req), 0) == -1) {
            pthread_mutex_unlock(&cli_data->lock);
            perror("send error");
            return false;
        }
    }

    printf("client was added %s\n", cli->name);
    return true;
}

void remove_client(client* cli, clients_data* cli_data) {
    if (cli->name[0] == '\0') {
        return;
    }

    bool in_clients = false;
    pthread_mutex_lock(&cli_data->lock);
    for (int i = 0; i <= cli_data->count - 1; i++) {
        if (strcmp(cli->name, cli_data->clients[i]->name) == 0) {
            in_clients = true;
        }

        if (in_clients == true && i < cli_data->count - 1) {
            cli_data->clients[i] = cli_data->clients[i + 1];
        }
    }

    if (in_clients == true) {
        cli_data->clients[--cli_data->count] = NULL;
    }

    pthread_mutex_unlock(&cli_data->lock);
}
