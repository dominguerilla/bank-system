#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
	if(acc == NULL){
		printf("NULL account.\n");
		return 0;
	}
	(*acc)->in_session = '1';
	printf("Account session for %s started.\n", (*acc)->name);
	return 1;
}

/**
* Finishes the account session.
* The return value is meant to set the state.
*/
int finishAccount(Account **acc, int state){
	if(state == 0){
		printf("Currently not in session!\n");
		return 0;
	}
	if(acc == NULL){
		printf("NULL account.\n");
		return state;
	}
	(*acc)->in_session = '0';
	return 0;
}
/**
 * Opens a new account given a list and a name
 *
 */
void openAccount(Node **head, char* name){
	Node *ptr = *head;
	if(strlen(name) >100){
		printf("The given name %s is too long!\n", name);
		return;
	}
	if(*head == NULL){
		*head = (Node*)malloc(sizeof(Node *));
		(*head)->next = NULL;
		(*head)->account = (Account*)malloc(sizeof(Account*));
		(*head)->account->name = name;
		(*head)->account->balance = 0.0;
		(*head)->account->in_session = '0';
		printf("New account created under %s.\n", (*head)->account->name);
		return;
	}
	while(ptr->next != NULL){
		ptr = ptr->next;
	}
	ptr->next = (Node*)malloc(sizeof(Node*));
	ptr->next->next = NULL;
	ptr->next->account = (Account*)malloc(sizeof(Account*));
	ptr->next->account->name = name;
	ptr->next->account->balance = 0.0;
	printf("New account created under %s.\n", ptr->next->account->name);
}

/**
* Returns the balance of a given account in a given list.
* If the account doesn't exist, returns 0.0 by default.
*/

float getBalance(char *name, Node *list){
	Node *ptr = findAccount(name, list);
	if(ptr == NULL) return 0.0;

	float balance = ptr->account->balance;
	printf("Balance of account %s is %f.\n", name, balance);
	return balance;
}
/**
* Credits/debits the account under a given name, from a given list, by a given amount.
* Returns 1 if successful, -1 if otherwise.
*/
int updateAccount(char* name, Node **list, float amount){
	Node *target_ptr = findAccount(name, *list);
	if(target_ptr == NULL) return -1;

	Node **target = &target_ptr;
	(*target)->account->balance += amount;
	printf("Balance of %s is now %f.\n", (*target)->account->name, (*target)->account->balance);
	return 1;
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
			/*printf("%s, with $%f\n", ptr->account->name, ptr->account->balance);*/
			getBalance(ptr->account->name, list);
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
