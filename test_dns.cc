/* test dns resolving on client system
   must return within 5 seconds

   e.g. test it with "time <cmd>"
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    struct addrinfo *res;
    int rc=getaddrinfo(argv[1], "http", NULL, &res);

    if (rc == 0) {	
        printf("OK: getaddrinfo returned %d for %s\n", rc, argv[1]);
        freeaddrinfo(res);
    } else {
        printf("ERROR: getaddrinfo returned %d for %s\n", rc, argv[1]);
    }
}
