#include <stdlib.h>
#include <stdio.h>
#include "accounts.h"

/*GLOBAL STATIC ACCOUNT REPOSITORY*/
static Node *account_list;

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

void addToList(Node *node){
	if(account_list == NULL){
		account_list = node;
		return;
	}else{
		Node *ptr = account_list;
		while(ptr->next != NULL){
			ptr = ptr->next;
		}
		ptr->next = node;
		return;
	}
}

void printList(){
	if(account_list == NULL){
		printf("List is empty!\n");
		return;
	}else{
		Node *ptr = account_list;
		
		/*THIS CONDITION STOPS IT AT LAST NODE*/
		while(ptr->next != NULL){
			printf("%s, with $%f\n", ptr->account->name, ptr->account->balance);
			ptr = ptr->next;
		}
		printf("%s, with $%f\n", ptr->account->name, ptr->account->balance);
	}
}

void deleteAccount(Node * target){
	if(target == NULL){
		printf("deleteAccount(): Target account is null!\n");
		return;
	}
	printf("Deleting %s's account...\n", target->account->name);
	free(target->account);
	free(target);
}

void destroyList(){
	if(account_list == NULL){
		printf("Cannot delete empty account list.\n");
		return;
	}
	Node *ptr = account_list;
	do{
		Node *next = ptr->next;
		deleteAccount(ptr);
		account_list = next;
		ptr = account_list;
	}while(ptr != NULL);
	printf("All accounts deleted.\n");
}
