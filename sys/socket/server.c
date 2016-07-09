#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h> /* struct sockaddr_in */

#define PORT 111
#define IP "127.0.0.1"
/*
 * socket(AF_LOCAL, SOCK_STREAM, 0);本地socket TODO
 */

int main(int argc, char **argv)
{
    int s;
    int domain = 1;
    int type = SOCK_STREAM;
    int protocol = 0;
    int err;
    printf("server test ...\n");
    
    s = socket(domain, type, protocol);
    if (s < 0) {
        printf("s = %d\n", s);
    } else {
        struct sockaddr_in addr_in;
        addr_in.sin_family = 1;
        addr_in.sin_port = PORT;
        addr_in.sin_addr = ;
        const struct sockaddr address = {1, IP};
        socklen_t address_len = sizeof(struct sockaddr);
        err = bind(s, &address, address_len);
        printf("sizeof(struct sockaddr) = %ld\n", sizeof(struct sockaddr));
        printf("sizeof(struct sockaddr_in) = %ld\n", sizeof(struct sockaddr_in));
        if (err) {
            printf("err = %d error: %s\n", err, strerror(errno));
        }
    }
    return 0;
}
