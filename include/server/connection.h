#ifndef CONNECTION_H
#define CONNECTION_H

#include <stdbool.h>

#include "server/server_types.h"

void* routine(void* arg);

bool name_is_free(client* cli, clients_data* cli_data);

bool get_user_name(client* cli);

bool add_client(client* cli, clients_data* cli_data);

void remove_client(client* cli, clients_data* cli_data);

#endif
