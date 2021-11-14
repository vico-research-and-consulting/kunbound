/* test dns resolving on client system
   must return within 5 seconds

   e.g. test it with "time <cmd>"
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    struct addrinfo *res, *tmp;
    struct addrinfo hints;
    char host[256];

    if (argc < 2) {
       fprintf(stderr, "Usage: %s <host_to_lookup>\n", argv[0]);
       exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    // hints.ai_family = AF_INET6;    /* Allow IPv4 or IPv6 */
    // hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    // hints.ai_protocol = 0;          /* Any protocol */

    int rc = getaddrinfo(argv[1], "http", &hints, &res);

    if (rc == 0) {	
        printf("OK: getaddrinfo returned no error for %s\n", argv[1]);
        for (tmp = res; tmp != NULL; tmp = tmp->ai_next) {
          getnameinfo(tmp->ai_addr, tmp->ai_addrlen, host, sizeof(host), NULL, 0, NI_NUMERICHOST);
          puts(host);
        }
        freeaddrinfo(res);
    } else {
        printf("ERROR: getaddrinfo returned \"%s\" for %s\n", gai_strerror(rc), argv[1]);
        exit(EXIT_FAILURE);
    }
}
