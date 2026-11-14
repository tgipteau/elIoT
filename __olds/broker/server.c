#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

#define PORT 4242
#define HOST "192.168.1.190" // Macbook 192.168.1.190

#define BUF_SIZE 64
#define BACKLOG_SIZE 6
#define MAX_CONN 32


int main(void){

	/* Creating the listening socket :
	 * create IP4 stream socket, define an
	 * address for the socket, bind them and start listening. */

	int sockfd_listen;
	if ((sockfd_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Error on socket creation");
		exit(EXIT_FAILURE);
    }
	setsockopt(sockfd_listen, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    if (inet_pton(AF_INET, HOST, &address.sin_addr) <= 0) {
	    perror("Error on inet_pton (defining address)");
	   	exit(EXIT_FAILURE);
    }

	int status = bind(sockfd_listen, (struct sockaddr *)&address, sizeof(address));
	if (status!=0) {
		perror("Error on binding listening socket\n");
		exit(EXIT_FAILURE);
	}

	status = listen(sockfd_listen, BACKLOG_SIZE);
	if (status!=0) perror("Error on listen call\n");
	printf("Listening started.\n");



	/* Core of the server. We define a pollfd struct to hold IO sockfds
 	* and listening socket as pfds[0]. Iterating over the struct means checking
	* new accepts waiting on pfds[0], and checking new POLLINS on others.
	* MAX_CO defines struct size, nfds stores the effective of connections.
	*
	* Two labels
	* -> MAIN_LOOP breaks the iteration in case of adding or removing of
	* clients which affects iteration parameters.
	* -> REMOVE_CLIENT is jumped to in case of various errors and terminations,
	* and jumps back to MAIN_LOOP after removal.
	*/

	struct pollfd pfds[MAX_CONN];
	int nfds = 1;
	pfds[0].fd = sockfd_listen;
	pfds[0].events = POLLIN;

	MAIN_LOOP :
	for(;;){

		/* Main loop. 100ms timeout on polls. */
		if (poll(pfds, nfds, 100)>0){

			/* Handle accepts here. */
			if (pfds[0].revents & POLLIN){

				if (nfds<MAX_CONN){
					printf("Accepting a connection.\n");
					pfds[nfds].fd = accept(sockfd_listen, NULL, 0);
					pfds[nfds].events = POLLIN;
					nfds++;
					goto MAIN_LOOP;

				} else {
					/* Refuse connection if the MAX_CONN number is attained. */
					printf("Refusing a connection.\n");
				    int refused = accept(sockfd_listen, NULL, NULL);
				    if (refused != -1) {
				        const char *msg = "ERR:server full.\n";
				        send(refused, msg, strlen(msg), 0);   // optionnel
				        close(refused);
					}
				}

			}

			/* Handle IO sockets */
			for (int i=1 ; i<nfds ; i++){


				if (pfds[i].revents & POLLHUP){
					/* Disconnected by client. */

					REMOVE_CLIENT :
					close(pfds[i].fd);
					pfds[i] = pfds[nfds-1];
					nfds--;
					goto MAIN_LOOP;
				}


				if (pfds[i].revents & POLLERR){
					//printf("POLLER\n");
					goto REMOVE_CLIENT;
					exit(EXIT_FAILURE);
				}


				if (pfds[i].revents & POLLIN){
					//printf("Received on pfds[%d].\n", i);

					char buf[BUF_SIZE];
					if (read(pfds[i].fd, buf, BUF_SIZE)>0){
						buf[strcspn(buf, "\n")]='\0';
						printf("Rcv : %s\n", buf);

					} else {
						goto REMOVE_CLIENT;
					}

				} /*if POLLIN */

			} /*for pfds */

		} /*if poll>0 */

	}

	/* Cleanup */
	for (int i=0 ; i<nfds ; i++) close(pfds[i].fd);
	close(sockfd_listen);

	return EXIT_SUCCESS;
}
