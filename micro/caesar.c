/* A2 UCID:30053278                                   */
/* Written by Juwei wang  2020.2.28*/
#include "Service.h"

int Caesar(int fd, const char* message, int len, struct sockaddr* client, int client_len) {
    char buf[MAX_BUFFER_SIZE]; 
    int idx = 0;
    char c;
    while (idx < len) {
        c = message[idx]; 
        if (c >='A' && c <= 'Z') {
            if ((c+13) > 'Z') {
                c = c + 13 - 'Z' + 'A' - 1;
            } else {
                c = c + 13;
            }
        }
        else if (c >='a' && c <= 'z') {
            if ((c+13) > 'z') {
                c = c + 13 - 'z' + 'a' - 1;
            } else {
                c = c + 13;
            }
        }

        buf[idx] = c;
        idx++;
    }
    sendto(fd, buf, len, 0, client, client_len);    

    return 0;
}
