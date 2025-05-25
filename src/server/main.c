#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "common/common_macros.h"
#include "common/common_setup.h"
#include "server/connection.h"
#include "server/server_macros.h"
#include "server/server_setup.h"
#include "server/server_types.h"

int main() {
    struct addrinfo hints, *res;
    setup_addr(&hints, &res, SERVER_IP, SERVER_PORT);

    int serv_fd = create_socket();
    enable_reuse_addr(serv_fd);
    bind_and_listen(serv_fd, res);

    freeaddrinfo(res);

    clients_data* cli_data = calloc(1, sizeof *cli_data);
    if (cli_data == NULL) {
        perror("calloc error");
        return 1;
    }

    pthread_mutex_init(&cli_data->lock, NULL);
    while (1) {
        struct sockaddr cli_addr;
        memset(&cli_addr, 0, sizeof cli_addr);

        int cli_fd;
        socklen_t addr_size = (socklen_t) sizeof cli_addr;
        if ((cli_fd = accept(serv_fd, &cli_addr, &addr_size)) == -1) {
            perror("accept error");
            continue;
        }

        client* cli = calloc(1, sizeof *cli);
        cli->sock_fd = cli_fd;
        cli->addr = cli_addr;

        thread_arg* arg = calloc(1, sizeof *arg);
        arg->cli_data = cli_data;
        arg->cli = cli;

        pthread_t tid;
        if (pthread_create(&tid, NULL, routine, (void*) arg) != 0) {
            perror("pthread_create error");
            return 1;
        }

        if (pthread_detach(tid) != 0) {
            perror("pthread_detach error");
            return 1;
        }
    }

    return 0;
}
