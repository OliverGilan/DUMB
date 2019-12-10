#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sockets.h"

// Create Queue
Queue *ConstructQueue() {
	Queue *queue = (Queue*)malloc(sizeof(Queue));
	if (queue == NULL) {
		return NULL;
	}
	queue->size = (int*)calloc(1, sizeof(int));
	queue->head = NULL;
	queue->tail = NULL;

	return queue;
}

// Free the memory  of Queue
void freeQueue(Queue *queue) {
	NODE *pN;
	while (!isEmpty(queue)) {
		pN = Dequeue(queue);
		free(pN);
	}
	free(queue);
}

// Add node to queue
int Enqueue(Queue *pQueue, NODE *item) {
	if ((pQueue == NULL) || (item == NULL))
		return 0;

	item->prev = NULL;
	if (*(pQueue->size) == 0) {
		pQueue->head = item;
		pQueue->tail = item;
	}
	else {
		pQueue->tail->prev = item;
		pQueue->tail = item;
	}
	*(pQueue->size) = *(pQueue->size) + 1;
	return 1;
}

// Remove node from head of queue
NODE * Dequeue(Queue *pQueue) {
	if (isEmpty(pQueue))
		return NULL;
	NODE* item = pQueue->head;
	pQueue->head = (pQueue->head)->prev;
	*(pQueue->size) = *(pQueue->size) - 1;
	return item;
}

// Check if Queue is empty
int isEmpty(Queue* pQueue) {
	if (pQueue == NULL) {
		return 0;
	}
	if (*(pQueue->size) == 0) {
		return 1;
	}
	else {
		return 0;
	}
}

// Get character count in message
int countChars(char* message) {
	if (message == NULL) {
		return 0;
	}
	int i = 0;
	while (message[i] != '\0' && i < strlen(message)) {
		i++;
	}
	return i;
}