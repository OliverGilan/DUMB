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
	// If the user didn't enter enough arguments, complain and exit
	if (argc < 3) {
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
	char helpMsg[] = "List of Commands:\n'hello' \t {HELLO} - Check if server is listening\n'quit' \t {GDBYE} - Disconnect from server\n'create' \t {CREAT} - Create new message box\n'delete' \t {DELBX} - Delete message box\n'open' \t {OPNBX} - Open message box\n'close' \t {CLSBX} - Close message box\n'next' \t {NXTMG} - Get next message\n'put' \t {PUTMG} - Add new message to box\n";
	// char array to store data going to and coming from the server
	char command[7];
	char buffer[1024];
	// Super-special secret C struct that holds address info for building our socket
	// Super-special secret C struct that holds info about a machine's address
	struct addrinfo* info = addStruct(hostname, port);


	// convert the text representation of the port number given by the user to an int

	// look up the IP address that matches up with the name given - the name given might
	//    BE an IP address, which is fine, and store it in the 'serverIPAddress' struct

	// try to build a socket .. if it doesn't work, complain and exit
	sock = socket(info->ai_family, SOCK_STREAM, 0);
	if (sock < 0) {
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
	for (int i = 0; i < 3; i++) {
		int res = connect(sock, info->ai_addr, info->ai_addrlen);
		if (res < 0) {
			error("connect()");
			continue;
		}
		break;
	}
	freeaddrinfo(info);

	/** If we're here, we're connected to the server .. w00t!  **/
	//Check if server is listening
	if (write(sock, "HELLO", 5) < 0) {
		printf("Error sending data to server: %s\n", strerror(errno)); //Complain if something goes wrong
		exit(1);
	}
	//Check for response
	int bits = read(sock, buffer, 1024);
	if (bits == -1) {
		printf("Error receiving data from server: %s\n", strerror(errno));	//Complain if something goes wrong
	}
	printf("%s\n", buffer);

	//Accept user input
	while (1) {
		printf("> ");
		// zero out the message buffer
		memset(buffer, 0, sizeof(buffer));
		memset(command, 0, sizeof(command));

		//Get a command from the client
		fscanf(stdin, "%s", command);

		//Converts to uppercase
		int j = 0;
		while (command[j]) {
			char ch = command[j];
			command[j] = (toupper(ch));
			j++;
		}
		// printf("command: %s\n", command);
		// command[5] = '\0';
		//Handle each command differently
		if (strcmp(command, "QUIT") == 0 || strcmp(command, "GDBYE") == 0) {
			if (write(sock, "GDBYE", 5) < 0) { //Send to server
				printf("Error sending data to server: %s\n", strerror(errno)); //Complain if something goes wrong
				continue;
			}

			//Check for success
			int bitsread = read(sock, buffer, sizeof(buffer));
			printf("%s\n", buffer);
			exit(0);
		}
		else if (strcmp(command, "CREATE") == 0 || strcmp(command, "CREAT") == 0) {
			printf("Okay, what should the message box name be?\ncreate:> ");
			fscanf(stdin, " %s", buffer);
			//Keep asking for input until it is well formed
			if (strlen(buffer) < 5 || strlen(buffer) > 25 || !isalpha(buffer[0])) {
				printf("Error: Message box name must be within 5 and 25 characters long and start with an alphabetic character!\n");
				// fgets(buffer, 26, stdin);
				continue;
			}

			//Create whole command
			char complete[32];
			memset(complete, 0, sizeof(complete));
			strncpy(complete, "CREAT ", 6);
			strncat(complete, buffer, 25);
			//Send command to server
			if (write(sock, complete, sizeof(complete)) < 0) {
				printf("Error sending data to server: %s\n", strerror(errno)); //Complain if something goes wrong
				continue;
			}

			//Expect response FIX
			memset(buffer, 0, sizeof(buffer));
			int bitsread = read(sock, buffer, sizeof(buffer));
			if (bitsread != 0) {
				// if(strncmp(buffer, "OK!", 3) == 0){
				printf("%s\n", buffer);
				// }
			}
			else if (bitsread == 0) {
				printf("Error receiving data from server!\n");
				exit(0);
			}
		}
		else if (strncmp(command, "DELETE", 6) == 0 || strncmp(command, "DELBX", 5) == 0) {
			printf("Okay, what's the name of the box you wish to delete?\ndelete:> ");
			memset(buffer, 0, sizeof(buffer));
			fscanf(stdin, " %s", buffer);

			//Check if message box name is well formed
			if (strlen(buffer) < 5 || strlen(buffer) > 25 || !isalpha(buffer[0])) {
				printf("Error: Command was unsuccessful - [poorly formatted box name] - please try again\n");
				continue;
			}

			//Create whole command
			char complete[32];
			memset(complete, 0, sizeof(complete));
			strncpy(complete, "DELBX ", 6);
			strncat(complete, buffer, 25);

			//Send command to server
			if (write(sock, complete, sizeof(complete)) < 0) {
				printf("Error sending data to server: %s\n", strerror(errno)); //Complain if something goes wrong
				continue;
			}
			//Expect response
			memset(buffer, 0, sizeof(buffer));
			int bitsread = read(sock, buffer, sizeof(buffer));
			if (bitsread != 0) {
				// if(strncmp(buffer, "OK!", 3) == 0){
				printf("%s\n", buffer);
				// }
			}
			else if (bitsread == 0) {
				printf("Error receiving data from server!\n");
				exit(0);
			}

		}
		else if (strncmp(command, "OPEN", 4) == 0 || strncmp(command, "OPNBX", 5) == 0) {
			printf("Okay, open which message box?\nopen:> ");
			fscanf(stdin, " %s", buffer);

			//Check if message box name is well formed
			if (strlen(buffer) < 5 || strlen(buffer) > 25 || !isalpha(buffer[0])) {
				printf("Error: Command was unsuccessful - [poorly formatted box name] - please try again\n");
				continue;
			}

			//Create whole command
			char complete[32];
			memset(complete, 0, 32);
			strncpy(complete, "OPNBX ", 6);
			strncat(complete, buffer, 25);

			//Send command to server
			if (write(sock, complete, sizeof(complete)) < 0) {
				printf("Error sending data to server: %s\n", strerror(errno)); //Complain if something goes wrong
				continue;
			}
			//Expect response
			memset(buffer, 0, sizeof(buffer));
			int bitsread = read(sock, buffer, sizeof(buffer));
			if (bitsread != 0) {
				// if(strncmp(buffer, "OK!", 3) == 0){
				printf("%s\n", buffer);
				// }
			}
			else if (bitsread == 0) {
				printf("Error receiving data from server!\n");
				exit(0);
			}

		}
		else if (strncmp(command, "CLOSE", 5) == 0 || strncmp(command, "CLSBX", 5) == 0) {
			printf("Okay, just confirm the name of the box you have open right now...\nclose:> ");
			fscanf(stdin, " %s", buffer);

			//Check if message box name is well formed
			if (strlen(buffer) < 5 || strlen(buffer) > 25 || !isalpha(buffer[0])) {
				printf("Error: Command was unsuccessful - [poorly formatted box name] - please try again\n");
				continue;
			}

			//Create whole command
			char complete[32];
			memset(complete, 0, 32);
			strncpy(complete, "CLSBX ", 6);
			strncat(complete, buffer, 25);

			//Send command to server
			if (write(sock, complete, sizeof(complete)) < 0) {
				printf("Error sending data to server: %s\n", strerror(errno)); //Complain if something goes wrong
				continue;
			}
			//Expect response
			memset(buffer, 0, sizeof(buffer));
			int bitsread = read(sock, buffer, sizeof(buffer));
			if (bitsread != 0) {
				// if(strncmp(buffer, "OK!", 3) == 0){
				printf("%s\n", buffer);
				// }
			}
			else if (bitsread == 0) {
				printf("Error receiving data from server!\n");
				exit(0);
			}


		}
		else if (strncmp(command, "NEXT", 4) == 0 || strncmp(command, "NXTMG", 5) == 0) {
			//Send command to server
			if (write(sock, "NXTMG", 5) < 0) {
				printf("Error sending data to server: %s\n", strerror(errno)); //Complain if something goes wrong
				continue;
			}

			//Expect response
			memset(buffer, 0, sizeof(buffer));
			int bitsread = read(sock, buffer, sizeof(buffer));
			if (bitsread != 0) {
				// if(strncmp(buffer, "OK!", 3) == 0){
				printf("%s\n", buffer);
				// }
			}
			else if (bitsread == 0) {
				printf("Error receiving data from server!\n");
				exit(0);
			}

		}
		else if (strncmp(command, "PUT", 3) == 0 || strncmp(command, "PUTMG", 5) == 0) {
			printf("Okay, how many bytes will your message be?\nput:> ");
			//Get bytes
			unsigned long bytes;
			fscanf(stdin, "%lu", &bytes);
			if (bytes == 0) { printf("No message will be created\n"); continue; }
			char length[8];
			sprintf(length, "%lu", bytes);
			//Get message
			printf("Sounds good. What message would you like to send?\nput:>");
			char* message = (char*)malloc(1024);
			unsigned long currentSize = 1024;
			char c = EOF;
			// scanf("%s", &message); //variable length input
			unsigned long i = 0;
			getchar(); //clear buffer
			int tooLong = 0;
			while ((fscanf(stdin, "%c", &c)) != '0' && c != '\n') {
				message[i++] = c;

				if (i == currentSize) {
					currentSize = i + currentSize;
					message = realloc(message, currentSize);
				}
				if (sizeof(char)*i > bytes) {
					printf("Message is longer than specified bytes.\n");
					tooLong = 1;
					break;
				}
			}
			if (tooLong) {
				fscanf(stdin, "%s", message);
				continue;
			}
			printf("size of message: %i", sizeof(message));
			message[i] = '\0';
			char data[7 + currentSize + sizeof(bytes) + 2];
			memset(data, 0, sizeof(data));
			strncpy(data, "PUTMG!", 6);
			strcat(data, length);
			strcat(data, "!");
			strcat(data, message);
			// printf("DATA: %s|\n", data);
			free(message);
			//Send to server
			if (write(sock, data, strlen(data)) < 0) {
				printf("Error sending data to server: %s\n", strerror(errno)); //Complain if something goes wrong
				continue;
			}
			memset(data, 0, sizeof(data));
			//Expect response
			memset(buffer, 0, sizeof(buffer));
			int bitsread = read(sock, buffer, sizeof(buffer));
			if (bitsread != 0) {
				// if(strncmp(buffer, "OK!", 3) == 0){
				printf("%s\n", buffer);
				// }
			}
			else if (bitsread == 0) {
				printf("Error receiving data from server!\n");
				exit(0);
			}


		}
		else if (strncmp(command, "HELLO", 5) == 0) {
			//Send to server
			if (write(sock, command, strlen(command)) < 0) {
				printf("Error sending data to server: %s\n", strerror(errno)); //Complain if something goes wrong
			}

			//Check for response
			int bits = read(sock, buffer, 1024);
			if (bits == -1) {
				printf("Error receiving data from server: %s\n", strerror(errno));	//Complain if something goes wrong
			}
			//printf("%s\n", buffer);

		}
		else if (strncmp(command, "HELP", 4) == 0) {
			//Print help message
			printf("%s\n", helpMsg);

		}
		else {
			printf("That is not a command, for a command list enter 'help.'\n");
		}
		// if we couldn't write to the server for some reason, complain and exit

		// sent message to the server, zero the buffer back out to read the server's response

		// read a message from the server into the buffer

		// if we couldn't read from the server for some reason, complain and exit

		// print out server's message
	}

	return 0;
}