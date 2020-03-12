#include <measure.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>

#define PORT 23456
#define BUF_SIZE 64
#define ITER 20

int main() {
    set_affinity();

    int socketfd;
    int ret, i;
    char *buf;
    unsigned long t[ITER];

    buf = malloc(BUF_SIZE);

    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in local_addr, remote_addr;
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(PORT);
    remote_addr.sin_addr.s_addr = inet_addr("137.110.222.242");
    remote_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (connect(socketfd, (struct sockaddr *)&remote_addr, sizeof(remote_addr)) < 0) {
        perror("connect error");
        exit(EXIT_FAILURE);
    }

    i = 0;
    while(i < ITER) {
        
        MEASURE_START();
        if ((ret = send(socketfd, buf, BUF_SIZE, 0)) < 0) {
            perror("send error");
            exit(EXIT_FAILURE);
        }

        ret = recv(socketfd, buf, BUF_SIZE, 0);
        MEASURE_END();
        t[i] = measure_time();
        i++;
    }

    for (i = 0; i < ITER; i++) {
        printf("%lu\n", t[i]);
    }

    close(socketfd);

    return 0;
}