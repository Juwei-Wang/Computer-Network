/* Client end of a simple demo of socket programming. */
/* This client interacts with the word length server, */
/* which needs to be running first.                   */
/*                                                    */
/* Usage:   cc -o wordlengthclient wordlengthclient.c */
/*          ./wordlengthclient                        */
/* On some machines, you might need -lsocket or -lnsl */  
/*                                                    */
/* A2 UCID:30053278                                   */
/* Written by Juwei wang  2020.2.28*/

/* Include files for C socket programming and stuff */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>

/* Some generic error handling stuff */
extern int errno;
void perror(const char *s);

/* Manifest constants used by client program */
#define MAX_HOSTNAME_LENGTH 64
#define MAX_WORD_LENGTH 100
#define BYNAME 1
#define MYPORTNUM 12345   /* must match the server's port! */

/* Menu selections */
#define ALLDONE 0
#define ENTER 1

/* Prompt the user to enter a word */
printmenu()
  {
    printf("\n");
    printf("Please choose from the following selections:\n");
    printf("  1 - Enter a word\n");
    printf("  0 - Exit program\n");
    printf("Your desired menu selection? ");
  }

/* Main program of client */
int main()
  {
    int sockfd, sockfd2;
    char c;
    struct sockaddr_in server;
    struct hostent *hp;
    char hostname[MAX_HOSTNAME_LENGTH];
    char message[MAX_WORD_LENGTH];
    char messageback[MAX_WORD_LENGTH];
    int choice, len, bytes;

    /* Initialization of server sockaddr data structure */
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(MYPORTNUM);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

#ifdef BYNAME
    /* use a resolver to get the IP address for a domain name */
    /* I did my testing using csg (136.159.5.20)    Carey */
    strcpy(hostname, "csg.cpsc.ucalgary.ca");
    hp = gethostbyname(hostname);
    if (hp == NULL)
      {
	fprintf(stderr, "%s: unknown host\n", hostname);
	exit(1);
      }
    /* copy the IP address into the sockaddr structure */
    bcopy(hp->h_addr, &server.sin_addr, hp->h_length);
#else
    /* hard code the IP address so you don't need hostname resolver */
    server.sin_addr.s_addr = inet_addr("136.159.5.20");
#endif

    /* create the client socket for its transport-level end point */
    if( (sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1 )
      {
	fprintf(stderr, "wordlengthclient: socket() call failed!\n");
	exit(1);
      }

    /* connect the socket to the server's address */
    if( connect(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) == -1 )
      {
	fprintf(stderr, "wordlengthclient: connect() call failed!\n");
	perror(NULL);
	exit(1);
      }

    /* Print welcome banner */
    printf("Welcome! I am the client end of the client-server demo!!\n\n");
    printmenu();
    scanf("%d", &choice);

    /* main loop: read a word, send to server, and print answer received */
    while( choice != ALLDONE )
      {
	if( choice == ENTER )
	  {
	    /* get rid of newline after the (integer) menu choice given */
	    c = getchar();

	    /* prompt user for the input */
	    printf("Enter your word: ");
	    len = 0;
	    while( (c = getchar()) != '\n' )
	      {
		message[len] = c;
		len++;
	      }
	    /* make sure the message is null-terminated in C */
	    message[len] = '\0';

	    /* send it to the server via the socket */
	    send(sockfd, message, len, 0);

	    /* see what the server sends back */
	    if( (bytes = recv(sockfd, messageback, len, 0)) > 0 )
	      {
		/* make sure the message is null-terminated in C */
		messageback[bytes] = '\0';
		printf("Answer received from server: ");
		printf("%s\n", messageback);
	      }
	    else
	      {
		/* an error condition if the server dies unexpectedly */
		printf("Sorry, dude. Server failed!\n");
		close(sockfd);
		exit(1);
	      }
	  }
	else printf("Invalid menu selection, you moron! Please try again.\n");

	printmenu();
	scanf("%d", &choice);
      }

    /* Program all done, so clean up and exit the client */
    close(sockfd);
    exit(0);
  }
