#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "accounts.h"


/**
* Attempts to find an account under the given name in the given list.
* Returns the node of the account if present; otherwise, returns NULL.
*/
Node * findAccount(char* name, Node* list){
	Node *ptr = list;
	while(ptr != NULL){
		if(strcmp(ptr->account->name, name) == 0){
			return ptr;
		}
		ptr = ptr->next;
	}
	printf("Account under %s not found.\n", name);
	return NULL;
}

/**
* Starts the account session for a given account.
*/
int startAccount(Account **acc, Node **head){
	/*pthread_mutex_lock(&((*acc)->lock));*/
	if(acc == NULL){
		printf("NULL account.\n");
		return -1;
	}else if((*acc)->in_session == '1'){
		printf("Already in session!");
		return 0;
	}
	(*acc)->in_session = '1';
	printf("Account session for %s started.\n", (*acc)->name);
	/*pthread_mutex_unlock(&((*acc)->lock));*/
	return 1;
}

/**
* Finishes the account session.
* The return value is meant to set the state.
*/
int finishAccount(Account **acc, int state){
	/*pthread_mutex_lock(&((*acc)->lock));*/
	if(state == 0){
		printf("Currently not in session!\n");
		return 0;
	}
	if(acc == NULL){
		printf("NULL account.\n");
		return state;
	}
	(*acc)->in_session = '0';
	/*pthread_mutex_unlock(&((*acc)->lock));*/
	return 0;
}
/**
 * Opens a new account given a list and a name
 *
 */
int openAccount(Node **head, char* name){
	Node *ptr = *head;
	if(strlen(name) >100){
		printf("The given name %s is too long!\n", name);
		return -1;
	}
	if(*head == NULL){
		*head = (Node*)malloc(sizeof(Node *));
		(*head)->next = NULL;
		(*head)->account = (Account*)malloc(sizeof(Account*));
		char* nomen = malloc(sizeof(char)*strlen(name));
		strcpy(nomen, name);
		/*pthread_mutex_init(&((*head)->account->lock), NULL);*/
		(*head)->account->name = nomen;
		(*head)->account->balance = 0.0;
		(*head)->account->in_session = '0';
		printf("New account created under %s.\n", (*head)->account->name);
		
		return 1;
	}
	while(ptr->next != NULL){
		ptr = ptr->next;
	}
	Node *new = malloc(sizeof(Node*));
	new->next = NULL;
	new->account = malloc(sizeof(Account*));
	char *nomen  = malloc(sizeof(char)*strlen(name));
	strcpy(nomen, name);
	/*pthread_mutex_init(&(new->account->lock), NULL);*/
	new->account->name = nomen;
	new->account->balance = 0.0;
	ptr->next = new;
	return 1;
}

/**
* Returns the balance of a given account in a given list.
* If the account doesn't exist, returns 0.0 by default.
*/

float getBalance(char *name, Node *list){
	Node *ptr = findAccount(name, list);
	if(ptr == NULL) return 0.0;
	float balance = ptr->account->balance;
	return balance;
}
/**
* Credits/debits the account under a given name, from a given list, by a given amount.
* Returns the balance..
*/
float updateAccount(char* name, Node **list, float amount){
	Node *target_ptr = findAccount(name, *list);
	if(target_ptr == NULL) return -1;
	
	/*pthread_mutex_lock(&(target_ptr->account->lock));*/
	Node **target = &target_ptr;
	(*target)->account->balance += amount;
	/*pthread_mutex_unlock(&(target_ptr->account->lock));*/
	return (*target)->account->balance;
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
		while(ptr != NULL){
			if(strlen(ptr->account->name) == 0){
				ptr = ptr->next;
				continue;
			}
			printf("%s, with $%.2f", ptr->account->name, ptr->account->balance);
			if(ptr->account->in_session == '1'){
				printf(" <- IN SESSION\n");
			}else{
				printf("\n");
			}
			ptr = ptr->next;
		}
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
void destroyList(Node **list){
	if(*list == NULL){
		printf("Cannot delete empty account list.\n");
		return;
	}
	Node *ptr = *list;
	do{
		Node *next = ptr->next;
		deleteAccount(ptr);
		*list = next;
		ptr = *list;
	}while(ptr != NULL);
	printf("All accounts deleted.\n");
}
