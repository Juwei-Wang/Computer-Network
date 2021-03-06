/* Simple program to demonstrate a basic UDP client.
 * Repeatedly asks for words from stdin, and sends each word to the server.
 * If a response is received, then print out the length of the word.
 * 
 * Compile using "cc -o wordlen-UDPclient wordlen-UDPclient.c"
 */
/* A2 UCID:30053278                                   */
/* Written by Juwei wang  2020.2.28                   */
 
/* Include files */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

/* Manifest constants */
#define MAX_BUFFER_SIZE 40
#define RETRY_TIMES 3

// Configure IP address and port of server here
/* #define SERVER_IP "127.0.0.1"   /* loopback interface */
//#define SERVER_IP "136.159.5.25"  /* csx.cpsc.ucalgary.ca */
//#define SERVER_PORT 12345   /* must match the server's actual port!! */
    
int udp_call(const char* SERVER_IP, uint16_t SERVER_PORT, char* buf, int buf_len, char* out, int out_len)
  {
    struct sockaddr_in si_server;
    struct sockaddr *server;
    int s, i, len = sizeof(si_server);
    socklen_t server_len = sizeof(si_server);
    //char buf[MAX_BUFFER_SIZE];
    int readBytes = -1;

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
      {
	printf("Could not set up a socket!\n");
	return 1;
      }
      
    memset((char *) &si_server, 0, sizeof(si_server));
    si_server.sin_family = AF_INET;
    si_server.sin_port = htons(SERVER_PORT);
    server = (struct sockaddr *) &si_server;

    if (inet_pton(AF_INET, SERVER_IP, &si_server.sin_addr)==0)
      {
	printf("inet_pton() failed\n");
	return 1;
      }

    fprintf(stderr, "Welcome! I am the UDP version of the word length client!!\n");

    /* loop until the user enters "bye" */
	//bzero(buf, MAX_BUFFER_SIZE);

 	//printf("Enter a word to send to the server (or \"bye\" to exit): ");
	//scanf("%s", buf);

	//if(strncmp(buf, "bye", 3) == 0)
	//  break;
  int try_count = 0;
  while (try_count < RETRY_TIMES && readBytes < 0) {
		
	if (sendto(s, buf, buf_len, 0, server, sizeof(si_server)) == -1)
	  {
	    printf("sendto failed\n");
	    return 1;
	  }

	  if ((readBytes=recvfrom(s, out, out_len, 0, server, &server_len))<0)
	  {
      if (readBytes == EAGAIN || readBytes == EWOULDBLOCK) {
        try_count++;
        continue;
      } else {
        break;
      }
	  }
    
	  out[readBytes] = '\0';    // proper null-termination of string
  }

  if (readBytes < 0) {
	    printf("Read error!\n");
	    return -1;
  }

	printf("Answer: That word has %s letters!\n", out);
    close(s);
    return 0;
  }
