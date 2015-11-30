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
Node * findAccount(char *, Node *);
int startAccount(Account **, Node**);
int finishAccount(Account **, int);
void openAccount(Node **, char *);
int updateAccount(char*, Node **, float);
float getBalance(char*, Node*);
void printList(Node *);
void deleteAccount(Node *);
void destroyList(Node **);
#endif
