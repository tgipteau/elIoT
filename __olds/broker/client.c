#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>


#define HOST "192.168.1.190"
#define PORT 4242
#define BUF_SIZE 64



int main(){

	// create socket
	int sockfd;
	if (( sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Error on socket creation");
		exit(EXIT_FAILURE);
    }

	// address
	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));       // met à zéro tous les champs
	address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    if (inet_pton(AF_INET, HOST, &address.sin_addr) <= 0) {
	    perror("Error on inet_pton (defining address)");
	   	exit(EXIT_FAILURE);
    }

    if(connect(sockfd, (struct sockaddr *)&address, sizeof(address))){
    	perror("Error on connect");
     	exit(EXIT_FAILURE);
    }

	printf("Connected.\n");
	char buf[BUF_SIZE];

	for(;;){

		fgets(buf, BUF_SIZE, stdin);
		send(sockfd, buf, BUF_SIZE-1, 0);

	}

	// cleanup
	//freeaddrinfo(&hints);
	close(sockfd);


	return EXIT_SUCCESS;
}
