#ifndef FORMATTER_H
#define FORMATTER_H

#include "server/server_types.h"

void format_response(accumulator *recv_accum, response *resp, clients_data *cli_data, client *cli);

void format_mesg(accumulator *recv_accum, response *resp, clients_data *cli_data, client *cli);

void format_brdcst(accumulator *recv_accum, response *resp, client *cli);

void format_list(accumulator *accum, response *resp, client *cli);

void format_help(accumulator *accum, response *resp, client *cli);

#endif
