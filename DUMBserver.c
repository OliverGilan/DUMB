#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <pthread.h>

// MESSAGE BOX
typedef struct Node_t { // message in queue
	char* message;
	struct Node_t *prev;
} NODE;

typedef struct Queue { // head of queue
	NODE *head;
	NODE *tail;
	int size;  // num. messages in box
} Queue;

Queue *ConstructQueue();
void freeQueue(Queue *queue);
int Enqueue(Queue *pQueue, NODE *item);
NODE *Dequeue(Queue *pQueue);
int isEmpty(Queue* pQueue);

Queue *ConstructQueue() {
	Queue *queue = (Queue*)malloc(sizeof(Queue));
	if (queue == NULL) {
		return NULL;
	}
	queue->size = 0;
	queue->head = NULL;
	queue->tail = NULL;

	return queue;
}

void freeQueue(Queue *queue) {
	NODE *pN;
	while (!isEmpty(queue)) {
		pN = Dequeue(queue);
		free(pN);
	}
	free(queue);
}

int Enqueue(Queue *pQueue, NODE *item) {
	if ((pQueue == NULL) || (item == NULL))
		return 0;

	item->prev = NULL;
	if (pQueue->size == 0) {
		pQueue->head = item;
		pQueue->tail = item;
	}
	else {
		pQueue->tail->prev = item;
		pQueue->tail = item;
	}
	pQueue->size++;
	return 1;
}

NODE * Dequeue(Queue *pQueue) {
	NODE *item;
	if (isEmpty(pQueue))
		return NULL;
	item = pQueue->head;
	pQueue->head = (pQueue->head)->prev;
	pQueue->size--;
	return item;
}

int isEmpty(Queue* pQueue) {
	if (pQueue == NULL) {
		return 0;
	}
	if (pQueue->size == 0) {
		return 1;
	}
	else {
		return 0;
	}
}

// MESSAGE BOX LIST

typedef struct node {
	char* name;
	Queue messageBox;
	struct node * next;
} listNODE;

typedef struct list {
	listNODE * head;
} List;

listNODE * createnode(char* name, Queue msgBox) {
	listNODE * newNode = malloc(sizeof(listNODE));
	newNode->name = malloc(sizeof(name));
	strcpy(newNode->name, name);
	newNode->messageBox = msgBox;
	newNode->next = NULL;
	return newNode;
}

List * makelist() {
	List * list = malloc(sizeof(List));
	if (!list) {
		return NULL;
	}
	list->head = NULL;
	return list;
}

void add(char* name, Queue msgBox, List * list) {
	listNODE * current = NULL;
	if (list->head == NULL) {
		list->head = createnode(name, msgBox);
	}
	else {
		current = list->head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = createnode(name, msgBox);
	}
}

void delete(char* name, List * list) {
	listNODE * current = list->head;
	listNODE * previous = current;
	while (current != NULL) {
		if (strcmp(current->name, name) == 0) {
			previous->next = current->next;
			if (current == list->head)
				list->head = current->next;
			free(current);
			return;
		}
		previous = current;
		current = current->next;
	}
}

int alreadyExists(char* name, List * list) {
	listNODE * current = list->head;
	while (current != NULL) {
		if (strcmp(current->name, name) == 0) {
			return 1;
		}
		current = current->next;
	}
	return 0;
}

listNODE* getBox(char* name, List* list) {
	listNODE * current = list->head;
	while (current != NULL) {
		if (strcmp(current->name, name) == 0) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}

// SERVER CODE

int portno;
List* list;

void error(char *msg)
{
	perror(msg);
	exit(1);
}

void* client(void* arg) {
	int socket = *(int*)arg;

	//Active box user is in
	listNODE* activeBox = NULL;
	// port number, date, ip address

	printf("%i connected\n", portno);

	//Wait for data to come in from client
	char buffer[1024];
	while (1) {
		for (int i = 0; i < 1024; i++) {
			buffer[i] = '\0';
		}
		int bits = read(socket, buffer, sizeof(buffer));	//Read data from socket
		if (bits == -1) {
			printf("Error receiving data from client: %s\n", strerror(errno));	//Complain if something goes wrong
		}
		if (bits == 0) { break; } //Needs a break clause. Temp for now.

		//Do something with data
		printf("%i %s\n", portno, buffer);

		if (strncmp(buffer, "HELLO", 5) == 0) {
			char* response = "HELLO DUMBv0 ready!";
			send(socket, response, strlen(response), 0);
		}
		else if (strncmp(buffer, "GDBYE", 5) == 0) {
			char* response = "GDBYE DUMBv0";
			send(socket, response, strlen(response), 0);
			break;
		}
		else if (strstr(buffer, "CREAT") != NULL) {
			char* response = "OK!";
			Queue *msgBox = ConstructQueue();
			char* name = strstr(buffer, " ");
			if (alreadyExists(name, list)) {
				response = "ER:EXIST";
			}
			else {
				add(name, *msgBox, list);
			}
			send(socket, response, strlen(response), 0);
		}
		else if (strstr(buffer, "DELBX") != NULL) {
			char* response = "OK!";
			char* name = strstr(buffer, " ");
			if (alreadyExists(name, list) == 1) {
				listNODE* box = getBox(name, list);
				if (!isEmpty(&(box->messageBox))) {
					response = "ER: NOTMT";
				}
				// else if(){} IF OPEN
				delete(name, list);
			}
			else if (alreadyExists(name, list) == 0) {
				response = "ER:NEXST";
			}

			send(socket, response, strlen(response), 0);
		}
		else if (strstr(buffer, "PUTMG") != NULL) {
			char* message = strchr(buffer, "!");
			char* command = strtok(buffer, "!");
			char* bytes = strtok(NULL, "!");
		}
		else {
			char* response = "ER:WHAT?";
			send(socket, response, strlen(response), 0);
		}

		//Reset buffer
		memset(buffer, 0, sizeof(buffer));
	}

	printf("%i disconnected\n", portno, socket);

	return NULL;
}

int main(int argc, char *argv[])
{
	// file descriptor for our server socket:
	// file descriptor for a client socket:
	// server port to connect to:
	list = makelist();
	int sockfd, newsockfd, clilen;

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
	while ((newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t*)&clilen)))	// block until a client connects, when it does, create a client socket
	{
		cli_addr.sin_addr.s_addr;
		char* s = malloc(INET_ADDRSTRLEN);
		inet_ntop(AF_INET, &(cli_addr.sin_addr), s, INET_ADDRSTRLEN);
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