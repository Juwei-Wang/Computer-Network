/* A2 UCID:30053278                                   */
/* Written by Juwei wang  2020.2.28                   */
#ifndef SERVICES_H
#define SERVICES_H

#include <unistd.h>
#include<sys/types.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#include "UDPserver.h"



int Identity(int fd, const char* message, int len, struct sockaddr*client, int client_len);

int Reverse(int fd, const char* message, int len, struct sockaddr*client, int client_len);

int Upper(int fd, const char* message, int len, struct sockaddr* client, int client_len);

int Caesar(int fd, const char* message, int len, struct sockaddr* client, int client_len);

int Lower(int fd, const char* message, int len, struct sockaddr* client, int client_len);

int Yours(int fd, const char* message, int len, struct sockaddr* client, int client_len);


typedef struct _Service {
    char* host;
    int port;
    HANDLE handle;
} Service;

typedef struct _Manager {
    Service* services;
    int count;
} Manager;



Manager* get_all_service();








#endif