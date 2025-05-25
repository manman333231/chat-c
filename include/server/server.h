#ifndef SERVER_H
#define SERVER_H

#include "server/server_types.h"

void serve_response(response *resp, clients_data *cli_data, client *cli);

void serve_list(response *resp, clients_data *cli_data, client *cli);

void serve_brdcst(response *resp, clients_data *cli_data, client *cli);

#endif
