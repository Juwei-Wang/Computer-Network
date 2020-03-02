/* 
 * echoserver.cpp
 * Created: Jan. 17, 2020
 * Author: Rachel Mclean
 *
 */

#include <unistd.h> 
#include <stdio.h>
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>  

using namespace std;

int main(int argc, char* argv[]){
	//address initialization	
	printf("Initializing address...\n");
	int PORT = 12345;
	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET; //specifies the address family
	server_address.sin_port = htons(PORT); //specificies the port number
	server_address.sin_addr.s_addr = htonl(INADDR_ANY); //specifies the IP address

	//socket creation
	printf("Creating socket...\n");
	int listen_socket;
	listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_socket == -1){
		printf("socket() call failed");
		exit(-1);
	}

	//binding
	printf("Binding...\n");
	int status;
	status = bind(listen_socket, (struct sockaddr *)&server_address, sizeof(struct sockaddr_in));
	if (status == -1){
		printf("bind() call failed\n");
		exit(-1);
	}

	//listening
	status = listen(listen_socket, 5);
	printf("Listening...\n");
	if (status == -1){
		printf("listen() call failed");
		exit(-1);
	}

	//accept connection
	int connect_socket;
	connect_socket = accept(listen_socket, NULL, NULL);
	printf("Accepting connection...\n");
	if (connect_socket == -1){
		printf("accept() call failed\n");
		exit(-1);
	}

	//send and receive information
	printf("Connection successful! Echoing information...\n");
	int count;
	char message[100];
	while (1) {
		count = recv(connect_socket, message, 100, 0);
		if (count == -1){
			printf("recv() call failed\n");
			exit(-1);
		} else {
			printf("received '%s' from client\n", message);
		}
		count = send(connect_socket, message, 100, 0);
		if (count == -1){
			printf("send() call failed");
			exit(-1);
		}

		if (strstr(message, "quit") != NULL){
			break;
		}
	}

	//close sockets
	printf("Closing socket...\n");
	close(connect_socket);
	close(listen_socket);
	return 0;
}
