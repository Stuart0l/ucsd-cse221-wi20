#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sched.h>
#include <time.h>

#define PORT 23456
#define MTU 1500
#define BUF_SIZE (MTU-20-20-14)
//#define BUF_SIZE 64
#define ITER 20

int main() {
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(19, &mask);

	if (sched_setaffinity(0, sizeof(mask), &mask) < 0) {
		perror("set affinity");
		exit(1);
	}

	int sockfd, clifd;
	int ret, i;
	char buf[BUF_SIZE];
	socklen_t len;
	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in local_addr, remote_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(PORT);
	local_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
		perror("bind error");
		exit(EXIT_FAILURE);
	}

	if (listen(sockfd, 1) < 0) {
		perror("listen error");
		exit(EXIT_FAILURE);
	}

	while (1) {
	len = sizeof(remote_addr);
	if ((clifd = accept(sockfd, (struct sockaddr *)&remote_addr, &len)) < 0) {
		perror("accept error");
		exit(EXIT_FAILURE);
	}
	printf("accept connection\n");
	}

	while(1) {
		if ((ret = recv(clifd, buf, BUF_SIZE, 0)) < 0) {
			perror("recv error");
			exit(EXIT_FAILURE);
		} else {
			//printf("recv %d\n", ret);
		}
		//send(clifd, buf, BUF_SIZE, 0);
	}

	return 0;
}
