#ifndef _SOCKETS_
#define _SOCKETS_

//Queue struct definitions
// Message in Queue
typedef struct Node_t {
	char* message;
	struct Node_t *prev;
} NODE;

// Queue 
typedef struct Queue {
	NODE *head;
	NODE *tail;
	int* size; 
} Queue;

//Queue function definitions
Queue *ConstructQueue();
void freeQueue(Queue *);
int Enqueue(Queue *, NODE *);
NODE *Dequeue(Queue *);
int isEmpty(Queue* );
int countChars(char* );


//List struct definitions
//List node struct
typedef struct node {
	char* name;
	Queue* messageBox;
	struct node * next;
	int* isOpen;
} listNODE;

//List struct
typedef struct list {
	listNODE * head;
} List;


//List function definitions
listNODE * createnode(char*, Queue*);
List * makelist();
void add(char*, Queue* , List *);
void delete(char*, List *);
int alreadyExists(char*, List *);
listNODE* getBox(char*, List*);


//Socket struct creator definition
struct addrinfo* addStruct(const char*, const char*);


#endif