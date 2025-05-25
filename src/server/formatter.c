#include <stdio.h>
#include <string.h>

#include "server/formatter.h"
#include "server/server_macros.h"
#include "server/server_types.h"

void format_respone(accumulator* recv_accum, response* resp, clients_data* cli_data, client* cli) {
    if (recv_accum->accum[0] == '\0' || recv_accum->accum[1] != ' ') {
        strcpy(resp->mesg, USAGE_ERROR_NONE);
        resp->sock_fd = cli->sock_fd;
        return;
    }

    char cmd = recv_accum->accum[0];
    switch (cmd) {
        case 'm':
            format_mesg(recv_accum, resp, cli_data, cli);
            break;
        case 'b':
            format_brdcst(recv_accum, resp, cli);
            break;
        case 'l':
            format_list(recv_accum, resp, cli);
            break;
        case 'h':
            format_help(recv_accum, resp, cli);
            break;
        default:
            strcpy(resp->mesg, USAGE_ERROR_NONE);
            resp->sock_fd = cli->sock_fd;
            break;
    }
}

void format_mesg(accumulator* recv_accum, response* resp, clients_data* cli_data, client* cli) {
    char dummy;
    char name[32], mesg[96];
    if (sscanf(recv_accum->accum, "%c %31s %94s", &dummy, name, mesg) < 3) {
        strcpy(resp->mesg, USAGE_ERROR_MESG);
        resp->sock_fd = cli->sock_fd;
        return;
    }

    for (int i = 0; i <= cli_data->count - 1; i++) {
        if (strcmp(name, cli_data->clients[i]->name) == 0) {
            snprintf(resp->mesg, MAX_RESP, "%31s: %94s", cli->name, mesg);
            resp->sock_fd = cli_data->clients[i]->sock_fd;
            return;
        }
    }

    strcpy(resp->mesg, USAGE_ERROR_CNF);
    resp->sock_fd = cli->sock_fd;
}

void format_brdcst(accumulator* recv_accum, response* resp, client* cli) {
    char dummy;
    char mesg[MAX_MESG];
    if (sscanf(recv_accum->accum, "%c %94s", &dummy, mesg) < 2) {
        strcpy(resp->mesg, USAGE_ERROR_BRDCST);
        resp->sock_fd = cli->sock_fd;
        return;
    }

    snprintf(resp->mesg, MAX_RESP, "%31s: %94s", cli->name, mesg);
    resp->sock_fd = ALL;
}

void format_list(accumulator* recv_accum, response* resp, client* cli) {
    if (recv_accum->accum[1] != '\0') {
        strcpy(resp->mesg, USAGE_ERROR_LIST);
        resp->sock_fd = cli->sock_fd;
        return;
    }

    strcpy(resp->mesg, LIST);
    resp->sock_fd = cli->sock_fd;
}

void format_help(accumulator* recv_accum, response* resp, client* cli) {
    if (recv_accum->accum[1] != '\0') {
        strcpy(resp->mesg, USAGE_ERROR_HELP);
        resp->sock_fd = cli->sock_fd;
        return;
    }

    strcpy(resp->mesg, USAGE_ERROR_NONE);
    resp->sock_fd = cli->sock_fd;
}
