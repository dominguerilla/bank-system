#include <stdlib.h>
#include <stdio.h>
#include "accounts.h"


/*RETURNS POINTER TO NEWLY CREATED ACCOUNT*/
Node * createAccount(char* name, float balance){
	/*CREATING AN ACCOUNT*/
	Account *acc = malloc(sizeof(Account));
	if(acc == NULL){
		printf("Not enough memory to create a new account.\n");
		free(acc);
		return NULL;
	}
	acc->name = name;
	acc->balance = balance;

	/*CREATING A NODE*/
	Node *node = malloc(sizeof(Node));
	if(node == NULL){
		printf("Not enough memory to create a new node.\n");
		free(node);
		free(acc);
	}
	node->account = acc;
	node->next = NULL;

	return node;
}

/**
 * Opens a new account given a list, and a name
 *
 */
void openAccount(Node **head, char* name){
	Node *ptr = *head;
	if(strlen(name) >100){
		printf("The given name %s is too long!\n", name);
		return;
	}
	if(*head == NULL){
		(*head) = (Node*)malloc(sizeof(Node *));
		(*head)->next = NULL;
		(*head)->account = (Account*)malloc(sizeof(Account*));
		(*head)->account->name = name;
		(*head)->account->balance = balance;
		(*head)->account->in_session = '0';
		return;
	}
	while(ptr->next != NULL){
		ptr = ptr->next;
	}
	ptr->next = (Node*)malloc(sizeof(Node*));
	ptr->next->next = NULL;
	ptr->next->account = (Account*)malloc(sizeof(Account*));
	ptr->next->account->name = name;
	ptr->next->account->balance = balance;
}

/*
 * Prints out the current list of accounts.
 *
 */
void printList(Node *list){
	if(list == NULL){
		printf("No accounts created.\n");
		return;
	}else{
		Node *ptr = list;
		
		/*THIS CONDITION STOPS IT AT LAST NODE*/
		while(ptr->next != NULL){
			printf("%s, with $%f\n", ptr->account->name, ptr->account->balance);
			ptr = ptr->next;
		}
		printf("%s, with $%f\n", ptr->account->name, ptr->account->balance);
	}
}

/*
 * Deletes an account given a node.
 *
 */
void deleteAccount(Node * target){
	if(target == NULL){
		printf("deleteAccount(): Target account is null!\n");
		return;
	}
	printf("Deleting %s's account...\n", target->account->name);
	free(target->account);
	free(target);
}

/*
 * Deletes the entire list of accounts.
 *
 */
void destroyList(Node *list){
	if(list == NULL){
		printf("Cannot delete empty account list.\n");
		return;
	}
	Node *ptr = list;
	do{
		Node *next = ptr->next;
		deleteAccount(ptr);
		list = next;
		ptr = list;
	}while(ptr != NULL);
	printf("All accounts deleted.\n");
}
