/* A2 UCID:30053278                                   */
/* Written by Juwei wang  2020.2.28                   */

#include "Service.h"
int Identity(int fd, const char* message, int len, struct sockaddr*client, int client_len) {

    sendto(fd, message, len, 0, client, client_len);    

    return 0;
}