#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "sockets.h"

struct addrinfo* addStruct(const char* host, const char* port){
    struct addrinfo* info;
    struct addrinfo args = {
        .ai_flags = AI_ADDRCONFIG | AI_ALL,
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM
    };

    int res = getaddrinfo(host, port, &args, &info);
    if(res < 0)
        printf("getaddrinfo(): %s\n", gai_strerror(res));
    
    return info;
}