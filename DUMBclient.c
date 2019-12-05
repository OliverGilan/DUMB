#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "sockets.h"


void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char **argv)
{

    if(argc < 3){
        printf("Need a server IP and Port! \n");
        exit(1);
    }
	// Declare initial vars
            // file descriptor for our socket
            int sock;
            // server port to connect to
            const char* hostname = argv[1];
            const char* port = argv[2];
            // utility variable - for monitoring reading/writing from/to the socket
            // char array to store data going to and coming from the server
            char command[6];
            char buffer[1024];
            // Super-special secret C struct that holds address info for building our socket
            // Super-special secret C struct that holds info about a machine's address
            struct addrinfo* info = addStruct(hostname, port);
	
	
	// If the user didn't enter enough arguments, complain and exit
    
	
	
	/** If the user gave enough arguments, try to use them to get a port number and address **/

	// convert the text representation of the port number given by the user to an int
	
	// look up the IP address that matches up with the name given - the name given might
	//    BE an IP address, which is fine, and store it in the 'serverIPAddress' struct
    				
	// try to build a socket .. if it doesn't work, complain and exit
    sock = socket(info->ai_family, SOCK_STREAM, 0);
    if(sock < 0){
        error("socket()");
    }

	
	/** We now have the IP address and port to connect to on the server, we have to get    **/
	/**   that information into C's special address struct for connecting sockets                     **/

	// zero out the socket address info struct .. always initialize!

	// set a flag to indicate the type of network address we'll be using 
	
	// set the remote port .. translate from a 'normal' int to a super-special 'network-port-int'

	// do a raw copy of the bytes that represent the server's IP address in 
	//   the 'serverIPAddress' struct into our serverIPAddressInfo struct



	/** We now have a blank socket and a fully parameterized address info struct .. time to connect **/
	
	// try to connect to the server 3 times using our blank socket and the address info struct 
	//   if it doesn't work, complain and exit
    for(int i = 0; i < 3; i++){
        int res = connect(sock, info->ai_addr, info->ai_addrlen);
        if(res < 0){
            error("connect()");
            continue;
        }
        break;
    }
	freeaddrinfo(info);
	
	/** If we're here, we're connected to the server .. w00t!  **/
		
	// zero out the message buffer

	// get a message from the client
    fgets(command, 6, stdin);
    int j = 0;
    while(command[j]){
        char ch = command[j];
        command[j] = (toupper(ch));
        j++;
    }
	// try to write it out to the server
	if(strncmp(command, "HELLO", 5) == 0){
        printf("sending %s\n", command);
        send(sock, command, strlen(command), 0);
        int bits = read(sock, buffer, 1024);
        if(bits == -1){
			printf("Error receiving data from client: %s\n", strerror(errno));	//Complain if something goes wrong
		}
        printf("%s\n", buffer);
    }
	// if we couldn't write to the server for some reason, complain and exit
	
	// sent message to the server, zero the buffer back out to read the server's response

	// read a message from the server into the buffer
	
	// if we couldn't read from the server for some reason, complain and exit

	// print out server's message


    return 0;
}
