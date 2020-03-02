/* A2 UCID:30053278                                   */
/* Written by Juwei wang  2020.2.28                   */
#include "Service.h"
int Reverse(int fd, const char* message, int len, struct sockaddr*client, int client_len) {
    char buf[MAX_BUFFER_SIZE];
    int idx = 0;
    while (idx < len) {
        buf[idx] = message[len - idx - 1];

        idx++;
    }

    sendto(fd, buf, len, 0, client, client_len);    

    return 0;
}