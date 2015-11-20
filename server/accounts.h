#ifndef ACCOUNTS_H
#define ACCOUNTS_H

/*STRUCT DEFINITIONS*/
typedef struct _account{
	char* name;
	float balance;
	char in_session;
}Account;

typedef struct _node{ 
	Account *account;
	struct _node *next;
}Node;

/*FUNCTION PROTOTYPES*/
Node * createAccount(char *, float);
void addToList(Node *);
void printList();
void deleteAccount(Node *);
void destroyList();
#endif
