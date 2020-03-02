/* A2 UCID:30053278                                   */
/* Written by Juwei wang  2020.2.28                   */
#ifndef UDPCLIENT_H

#define UDPCLIENT_H
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int udp_call(const char* SERVER_IP, uint16_t SERVER_PORT, char* buf, int buf_len, char* out, int out_len);

#endif