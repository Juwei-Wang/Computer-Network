/* TCP-based server example of socket programming.    */
/* The server receives an input word (e.g., "dog")    */
/* and returns the length of the word (e.g., "3").    */
/*                                                    */
/* Usage: cc -o wordlen-TCPserver wordlen-TCPserver.c */
/*        ./wordlen-TCPserver                         */
/*                                                    */
/* Written by Carey Williamson       January 13, 2012 */

/* Include files for C socket programming and stuff */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include "Service.h"
#include "UDPserver.h"
#include "UDPclient.h"

/* Global manifest constants */
#define MAX_MESSAGE_LENGTH 100
#define MYPORTNUM 12345

/* Optional verbose debugging output */
#define DEBUG 1
/* A2 UCID:30053278                                   */
/* Written by Juwei wang  2020.2.28                   */
/* Global variable */
int childsockfd;

/* This is a signal handler to do graceful exit if needed */
void catcher( int sig )
  {
    close(childsockfd);
    exit(0);
  }

/* Main program for server */
int main(int argc, char* argv[])
  {
    struct sockaddr_in server;
    static struct sigaction act;
    char messagein[MAX_MESSAGE_LENGTH];
    char messageout[MAX_MESSAGE_LENGTH];
	char data_source[MAX_MESSAGE_LENGTH];
    int parentsockfd;
	int port;
    int i, j;
    int pid;
    char c;
	int idx =0;
	int rec_len = 0;
	Manager* manager = get_all_service();
	int cmd_idx = 0;
	port = atoi(argv[1]);

	for (idx = 0 ; idx < manager->count; idx++) {

		pid = fork();	
		if (pid == 0) {

			udp_server(manager->services[idx].port, manager->services[idx].handle);

			exit(0);
		}

	}

	

    /* Set up a signal handler to catch some weird termination conditions. */
    act.sa_handler = catcher;
    sigfillset(&(act.sa_mask));
    sigaction(SIGPIPE, &act, NULL);

    /* Initialize server sockaddr structure */
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    //server.sin_port = htons(MYPORTNUM);
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    /* set up the transport-level end point to use TCP */
    if( (parentsockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1 )
      {
	fprintf(stderr, "wordlen-TCPserver: socket() call failed!\n");
	exit(1);
      }

    /* bind a specific address and port to the end point */
    if( bind(parentsockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in) ) == -1 )
      {
	fprintf(stderr, "wordlen-TCPserver: bind() call failed!\n");
	exit(1);
      }

    /* start listening for incoming connections from clients */
    if( listen(parentsockfd, 5) == -1 )
      {
	fprintf(stderr, "wordlen-TCPserver: listen() call failed!\n");
	exit(1);
      }

    /* initialize message strings just to be safe (null-terminated) */
    bzero(messagein, MAX_MESSAGE_LENGTH);
    bzero(messageout, MAX_MESSAGE_LENGTH);
    bzero(data_source, MAX_MESSAGE_LENGTH);

    fprintf(stderr, "Welcome! I am the TCP version of the word length server!!\n");
    //fprintf(stderr, "server listening on TCP port %d...\n\n", MYPORTNUM);
    fprintf(stderr, "server listening on TCP port %d...\n\n", port);

    /* Main loop: server loops forever listening for requests */
    for( ; ; )
      {
	/* accept a connection */
	if( (childsockfd = accept(parentsockfd, NULL, NULL)) == -1 )
	  {
	    fprintf(stderr, "wordlen-TCPserver: accept() call failed!\n");
	    exit(1);
	  }

	/* try to create a child process to deal with this new client */
	pid = fork();

	/* use process id (pid) returned by fork to decide what to do next */
	if( pid < 0 )
	  {
	    fprintf(stderr, "wordlen-TCPserver: fork() call failed!\n");
	    exit(1);
	  }
	else if( pid == 0 )
	  {
	    /* the child process is the one doing the "then" part */
		printf("entering child\n");
	    /* don't need the parent listener socket that was inherited */
	    close(parentsockfd);
	    if ( recv(childsockfd, data_source, MAX_MESSAGE_LENGTH, 0) <= 0 ) {
			printf("child recv error\n");
	    	close(childsockfd);
	    	exit(0);
		}

	    /* obtain the message from this client */
	    while( (rec_len= recv(childsockfd, messagein, MAX_MESSAGE_LENGTH, 0)) > 0 )
	      {
		/* print out the received message */
		printf("Child process received word: %s\n", messagein);
		printf("That word has %d characters!\n", strlen(messagein));
		sprintf(messageout, "%s", data_source);
		cmd_idx = 0;	
		for (cmd_idx = 0; cmd_idx < rec_len; cmd_idx++) {
			int cmd = messagein[cmd_idx] - '1';
			if ((cmd< 0 || cmd >= manager->count)) {
				sprintf(messageout, "%s\n", "error transform");
				break;
			} else {
				if (udp_call(manager->services[cmd].host,manager->services[cmd].port, messageout, strlen(messageout), messageout, strlen(messageout)) != 0) {

					sprintf(messageout, "%s\n", "error udp call");
					break;
				}
			}
		}


#ifdef DEBUG
		printf("Child about to send message: %s\n", messageout);
#endif

		/* send the result message back to the client */
		send(childsockfd, messageout, strlen(messageout), 0);

		/* clear out message strings again to be safe */
		bzero(messagein, MAX_MESSAGE_LENGTH);
		bzero(messageout, MAX_MESSAGE_LENGTH);
	      }

	    /* when client is no longer sending information to us, */
	    /* the socket can be closed and the child process terminated */
	    close(childsockfd);
	    exit(0);
	  } /* end of then part for child */
	else
	  {
	    /* the parent process is the one doing the "else" part */
	    fprintf(stderr, "Created child process %d to handle that client\n", pid);
	    fprintf(stderr, "Parent going back to job of listening...\n\n");

	    /* parent doesn't need the childsockfd */
	    close(childsockfd);
	  }
      }
  }
