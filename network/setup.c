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
    unsigned long t[ITER], s[ITER];

    buf = malloc(BUF_SIZE);

    struct sockaddr_in local_addr, remote_addr;
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(PORT);
    //remote_addr.sin_addr.s_addr = inet_addr("137.110.222.242");
    remote_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(34567);
	local_addr.sin_addr.s_addr = INADDR_ANY;

    i = 0;
    while(i < ITER) {

		if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			perror("socket error");
			exit(EXIT_FAILURE);
		}
        
        MEASURE_START();
		if (connect(socketfd, (struct sockaddr *)&remote_addr, sizeof(remote_addr)) < 0) {
			perror("connect error");
			exit(EXIT_FAILURE);
		}

        MEASURE_END();
        t[i] = measure_time();

		
		MEASURE_START();
		if (close(socketfd) < 0) {
			perror("shut down");
			exit(EXIT_FAILURE);
		}
		MEASURE_END();
		s[i] = measure_time();
        i++;
    }

    for (i = 0; i < ITER; i++) {
        printf("%lu\n", t[i]);
    }

    for (i = 0; i < ITER; i++) {
        printf("%lu\n", s[i]);
    }
    return 0;
}