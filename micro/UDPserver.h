/* A2 UCID:30053278                                   */
/* Written by Juwei wang  2020.2.28                   */
#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 40

typedef int (*HANDLE)(int fd, const char* message, int len, struct sockaddr* client, int client_len);
int udp_server(uint16_t PORT, HANDLE handle);

#endif