#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(void)
{
    int status = 0;
    struct addrinfo hints;
    struct addrinfo *result = NULL, *rp = NULL;
    char *ai_family_desc = NULL;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    status = getaddrinfo("localhost", NULL, &hints, &result);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return EXIT_FAILURE;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        ai_family_desc = NULL;
        switch (rp->ai_family) {
        case AF_INET:
            ai_family_desc = "AF_INET";
            break;
        case AF_INET6:
            ai_family_desc = "AF_INET6";
            break;
        case AF_UNSPEC:
            ai_family_desc = "AF_UNSPEC";
            break;
        default:
            ai_family_desc = "UNKNOWN";
            break;
        }
        printf("addrinfo result %s\n", ai_family_desc);
    }

    return EXIT_SUCCESS;
}
