#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "common/common_macros.h"
#include "common/common_setup.h"

void* routine(void* arg) {
    int servfd = *(int*) arg;
    free(arg);

    char send_buffer[96];

    while (1) {
        memset(send_buffer, 0, sizeof send_buffer);
        fgets(send_buffer, sizeof send_buffer, stdin);
        if (send(servfd, send_buffer, strlen(send_buffer), 0) == -1) {
            perror("send error");
        }
    }
}

int main() {
    struct addrinfo hints, *res;
    setup_addr(&hints, &res, SERVER_IP, SERVER_PORT);

    int servfd = create_socket();

    if (connect(servfd, res->ai_addr, res->ai_addrlen) == -1) {
        close(servfd);
        return 1;
    }

    pthread_t tid;
    int* servfd_ptr = calloc(1, sizeof(int));
    *servfd_ptr = servfd;
    if (pthread_create(&tid, NULL, routine, (void*) servfd_ptr) != 0) {
        perror("pthread create error");
        return 1;
    }

    if (pthread_detach(tid) != 0) {
        perror("pthread detach error");
        return 1;
    }

    char recv_buffer[129];
    memset(recv_buffer, 0, sizeof recv_buffer);

    int bytes_recv;
    while ((bytes_recv = recv(servfd, recv_buffer, sizeof recv_buffer, 0)) > 0) {
        printf("%s\n", recv_buffer);
        memset(recv_buffer, 0, sizeof recv_buffer);
    }

    return 0;
}
