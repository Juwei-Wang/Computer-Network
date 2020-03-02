/* A2 UCID:30053278                                   */
/* Written by Juwei wang  2020.2.28                   */
#include "Service.h"

int Yours(int fd, const char* message, int len, struct sockaddr* client, int client_len) {
    char buf[MAX_BUFFER_SIZE];
    int idx = 0;
    char c;

    while (idx < len) {
        c = message[idx]; 
        if (c == ' ') {
            c = '_';
        }

        buf[idx] = c;
        idx++;
    }

    sendto(fd, buf, len, 0, client, client_len);    
    return 0;
}
