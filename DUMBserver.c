#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

void error(char *msg)
{
	perror(msg);
	exit(1);
}

void* client(void* arg){
	int socket = *(int*)arg;

	printf("%d connected\n", socket);

	//Wait for data to come in from client
	char buffer[1024];
	while(1){
		int bits = read(socket, buffer, sizeof(buffer));	//Read data from socket
		if(bits == -1){
			printf("Error receiving data from client: %s\n", strerror(errno));	//Complain if something goes wrong
		}
		if(bits == 0){break;} //Needs a break clause. Temp for now.
		
		//Do something with data
		printf("%s\n",buffer);

		if(strncmp(buffer, "HELLO", 5) == 0){
			printf("sending response\n");
			char* response = "HELLO DUMBv0 ready!";
			send(socket, response, strlen(response), 0);
    	}

		//Reset buffer
		memset(buffer, 0, sizeof(buffer));
	}

	printf("%d disconnected\n", socket);

	return NULL;
}

int main(int argc, char *argv[])
{
	// file descriptor for our server socket:
	// file descriptor for a client socket:
	// server port to connect to:
	int sockfd, newsockfd, portno, clilen;

	struct sockaddr_in serv_addr;	// Super-special secret C struct that holds address info for building our server socket
	struct sockaddr_in cli_addr;	// Super-special secret C struct that holds address info about our client socket
	int addrlen;					// utility variable - size of clientAddressInfo below
	int val;						// utility variable - for monitoring reading/writing from/to the socket
									// char array to store data going to and coming from the socket



	if (argc < 2)					// If the user didn't enter enough arguments, complain and exit
	{
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}



	/** If the user gave enough arguments, try to use them to get a port number and address **/

	const char* portRaw = argv[1];
	portno = atoi(portRaw);						// convert the text representation of the port number given by the user to an int  


	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0)	// try to build a socket .. if it doesn't work, complain and exit
	{
		error("socket()");
		exit(EXIT_FAILURE);
	}

	/** We now have the port to build our server socket on .. time to set up the address struct **/

	memset(&serv_addr, 0, sizeof(serv_addr));	// zero out the socket address info struct .. always initialize!
	serv_addr.sin_port = htons(portno);			// set the remote port .. translate from a 'normal' int to a super-special 'network-port-int'
	serv_addr.sin_addr.s_addr = INADDR_ANY;		// set a flag to indicate the type of network address we'll be willing to accept connections from 
	serv_addr.sin_family = AF_INET;				// set a flag to indicate the type of network address we'll be using 


	/** We have an address struct and a socket .. time to build up the server socket **/

	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)	// bind the server socket to a specific local port, so the client has a target to connect to  
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(sockfd, 3) < 0)			// set up the server socket to listen for client connections
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	printf("Server listening on port: %d\n", portno);
	clilen = sizeof(cli_addr);			// determine the size of a clientAddressInfo struct
	

	pthread_t thread;
	while((newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t*)&clilen)))	// block until a client connects, when it does, create a client socket
	{
		pthread_create(&thread, NULL, client, (void*)&newsockfd);
		pthread_detach(thread);
	}

	/** If we're here, a client tried to connect **/

	// if the connection blew up for some reason, complain and exit

	// memset(buffer, 0, sizeof(buffer));							// zero out the char buffer to receive a client message
	// val = read(newsockfd, buffer, 1024);	// try to read from the client socket
	// if (val == -1) {						// if the read from the client blew up, complain and exit
	// 	perror("read");
	// 	exit(EXIT_FAILURE);
	// }
	// printf("%s\n", buffer);
	// for loop to read commands and text

// try to write to the client socket

// if the write to the client blow up, complain and exit


	return 0;
}