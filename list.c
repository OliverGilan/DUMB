#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sockets.h"

//Create new node
listNODE * createnode(char* name, Queue* msgBox) {
	listNODE * newNode = malloc(sizeof(listNODE));
	newNode->name = malloc(sizeof(name));
	strcpy(newNode->name, name);
	newNode->isOpen = (int*)calloc(1, sizeof(int));
	newNode->messageBox = msgBox;
	newNode->next = NULL;
	return newNode;
}

//Create list
List * makelist() {
	List * list = malloc(sizeof(List));
	if (!list) {
		return NULL;
	}
	list->head = NULL;
	return list;
}

//Add node to list
void add(char* name, Queue* msgBox, List * list) {
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

//Delete node from list
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

//Check if node exists in list
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

//Get specific node from list
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
