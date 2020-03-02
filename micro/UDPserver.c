/* Simple program to demonstrate a UDP-based server.
 * Loops forever.
 * Receives a word from the client. 
 * Sends back the length of that word to the client.
 * 
 * Compile using "cc -o wordlen-UDPserver wordlen-UDPserver.c"
 */
/* A2 UCID:30053278                                   */
/* Written by Juwei wang  2020.2.28                   */
/* Include files */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "UDPserver.h"

/* Manifest constants */
//#define PORT 12345

/* Verbose debugging */
#define DEBUG 1


/* Service program */
int udp_server(uint16_t PORT, HANDLE handle)
  {
    struct sockaddr_in si_server, si_client;
    struct sockaddr *server, *client;
    int s, i, len=sizeof(si_server);
    socklen_t client_len = sizeof(si_server);
    char messagein[MAX_BUFFER_SIZE];
    char messageout[MAX_BUFFER_SIZE];
    int readBytes;

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
      {
	printf("Could not setup a socket!\n");
	return 1;
      }
    
    memset((char *) &si_server, 0, sizeof(si_server));
    si_server.sin_family = AF_INET;
    si_server.sin_port = htons(PORT);
    si_server.sin_addr.s_addr = htonl(INADDR_ANY);
    server = (struct sockaddr *) &si_server;
    client = (struct sockaddr *) &si_client;

    if (bind(s, server, sizeof(si_server))==-1)
      {
	printf("Could not bind to port %d!\n", PORT);
	return 1;
      }
    fprintf(stderr, "Welcome! I am the UDP version of the word length server!!\n");
    printf("server now listening on UDP port %d...\n", PORT);
	
    /* big loop, looking for incoming messages from clients */
    for( ; ; )
      {
	/* clear out message buffers to be safe */
	bzero(messagein, MAX_BUFFER_SIZE);
	bzero(messageout, MAX_BUFFER_SIZE);

	/* see what comes in from a client, if anything */
	if ((readBytes=recvfrom(s, messagein, MAX_BUFFER_SIZE, 0, client, &client_len)) < 0)
	  {
	    printf("Read error!\n");
	    return -1;
	  }
#ifdef DEBUG
	else printf("Server received %d bytes\n", readBytes);
#endif

	printf("  server received \"%s\" from IP %s port %d\n",
	       messagein, inet_ntoa(si_client.sin_addr), ntohs(si_client.sin_port));

#ifdef DEBUG
	printf("server thinks that word has %d characters...\n", strlen(messagein));
#endif
	/* create the outgoing message (as an ASCII string) */
	sprintf(messageout, "%d", strlen(messagein));

#ifdef DEBUG
	printf("Server sending back the message: \"%s\"\n", messageout);
#endif

  handle(s, messagein, readBytes, client, len);
	/* send the result message back to the client */
	//sendto(s, messageout, strlen(messageout), 0, client, len);		
      }

    close(s);
    return 0;
  }
