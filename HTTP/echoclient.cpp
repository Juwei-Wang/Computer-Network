/* 
 * echoclient.cpp
 * Created: Jan. 17, 2020
 * Author: Rachel Mclean
 *
 */

#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h> 


#include <string.h> 

#include <sys/socket.h> 
#include <netinet/in.h> 

using namespace std;

int main(int argc, char* argv[]){
	//address initialization
	printf("Initializing address...\n");
	int PORT = 12345;
	struct sockaddr_in client_address;
	memset(&client_address, 0, sizeof(client_address));
	client_address.sin_family = AF_INET; //specifies the address family
	client_address.sin_port = htons(PORT); //specificies the port number
	client_address.sin_addr.s_addr = htonl(INADDR_ANY); //specifies the IP address

	//socket creation
	printf("Creating socket...\n");
	int connect_socket;
	connect_socket = socket(AF_INET, SOCK_STREAM, 0);
	//check if socket creation was successful
	if (connect_socket == -1){
		printf("socket() call failed");
		exit(-1);
	}

	//connection request
	printf("Sending connection request...\n");
	int status;
	status = connect(connect_socket, (struct sockaddr*)&client_address, sizeof(struct sockaddr_in));
	//check if connection was successful
	if (status == -1){
		printf("connect() call failed");
		exit(-1);
	}

	//send and receive information
	printf("Connection successful! Enter 'quit' to end program.\n");
	int count;
	char send_message[100] = {""};
	char receive_message[100];
	while (1) {

		printf("Enter message to send: ");
		gets(send_message);
		count = send(connect_socket, send_message, 100, 0);
		if (count == -1){
			printf("send() call failed");
			exit(-1);
		}
		count = recv(connect_socket, receive_message, 100, 0);
		if (count == -1){
			printf("recv() call failed\n");
			exit(-1);
		} else {
			printf("received '%s' from server\n", receive_message);
		}
		if (strstr("quit", receive_message) != NULL || strstr(send_message, "quit") != NULL){
			break;
		}
	}

	//close sockets
	printf("Closing socket...\n");
	close(connect_socket);
	return 0;

}