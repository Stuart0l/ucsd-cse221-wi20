#include <measure.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <arpa/inet.h>
#include <stdio.h>

#define PORT 23456
#define K 1024
#define M (1024 * K)
#define BLOCK (4 * M)
#define ITER 20

int main() {
	set_affinity();	

    int socketfd, filefd;
    int ret, i;
    unsigned long t[ITER];

    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in local_addr, remote_addr;
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(PORT);
    remote_addr.sin_addr.s_addr = inet_addr("137.110.222.242");
    //remote_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (connect(socketfd, (struct sockaddr *)&remote_addr, sizeof(remote_addr)) < 0) {
        perror("connect error");
        exit(EXIT_FAILURE);
    }

	if ((filefd = open("1.dat", O_RDONLY)) < 0) {
		perror("open file");
		exit(EXIT_FAILURE);
	}
	
	off_t offset = 0;

    for (i = 0; i < ITER; i++) {
        offset = 0;
        printf("iteraton: %d\n", i);
        MEASURE_START();
        ret = sendfile(socketfd, filefd, &offset, 50 * M);
        MEASURE_END();
        t[i] = measure_time();
    }

    for (i = 0; i < ITER; i++)
	    printf("%lu\n", t[i]);

	close(socketfd);
	close(filefd);

	return 0;
}