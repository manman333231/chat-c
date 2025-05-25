#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "server/server.h"
#include "server/server_macros.h"
#include "server/server_types.h"

void serve_response(response* resp, clients_data* cli_data, client* cli) {
    if (strcmp(resp->mesg, LIST) == 0) {
        serve_list(resp, cli_data, cli);
    }

    if (resp->sock_fd == ALL) {
        serve_brdcst(resp, cli_data, cli);
    }

    for (int i = 0; i <= cli_data->count - 1; i++) {
        if (resp->sock_fd == cli_data->clients[i]->sock_fd) {
            pthread_mutex_lock(&cli_data->lock);
            if (send(cli_data->clients[i]->sock_fd, resp->mesg, MAX_RESP, 0) == -1) {
                perror("send error");
            }

            pthread_mutex_unlock(&cli_data->lock);
        }
    }
}

void serve_list(response* resp, clients_data* cli_data, client* cli) {
    for (int i = 0; i <= cli_data->count - 1; i++) {
        if (resp->sock_fd == cli_data->clients[i]->sock_fd) {
            for (int j = 0; j <= cli_data->count - 1; j++) {
                snprintf(resp->mesg, MAX_RESP, "[%d/%d] %31s", i + 1, cli_data->count, cli_data->clients[j]->name);
                pthread_mutex_lock(&cli_data->lock);
                if (send(cli_data->clients[i]->sock_fd, resp->mesg, MAX_RESP, 0) == -1) {
                    perror("send error");
                }

                pthread_mutex_unlock(&cli_data->lock);
            }
        }
    }
}

void serve_brdcst(response* resp, clients_data* cli_data, client* cli) {
    for (int i = 0; i <= cli_data->count - 1; i++) {
        if (cli_data->clients[i]->sock_fd != cli->sock_fd) {
            pthread_mutex_lock(&cli_data->lock);
            if (send(cli_data->clients[i]->sock_fd, resp->mesg, MAX_RESP, 0) == -1) {
                perror("send error");
            }

            pthread_mutex_unlock(&cli_data->lock);
        }
    }
}
