#ifndef ACCOUNTS_H
#define ACCOUNTS_H

/*STRUCT DEFINITIONS*/
typedef struct _account{
	char* name;
	float balance;
	char in_session;
	/*pthread_mutex_t lock;*/
}Account;

typedef struct _node{ 
	Account *account;
	struct _node *next;
}Node;

/*FUNCTION PROTOTYPES*/

/**
* Attempts to find an account under the given name in the given list.
* Returns the node of the account if present; otherwise, returns NULL.
*/
Node * findAccount(char *, Node *);

/**
* Starts the account session for a given account.
*/
int startAccount(Account **, Node**);

/**
* Finishes the account session.
* The return value is meant to set the state.
*/
int finishAccount(Account **, int);

/**
 * Opens a new account given a list and a name
 *
 */
int openAccount(Node **, char *);

/**
* Returns the balance of a given account in a given list.
* If the account doesn't exist, returns 0.0 by default.
*/
float updateAccount(char*, Node **, float);

/**
* Credits/debits the account under a given name, from a given list, by a given amount.
* Returns the balance..
*/
float getBalance(char*, Node*);

/*
 * Prints out the current list of accounts.
 *
 */
void printList(Node *);


/*UNUSED FUNCTIONS*/
/*
 * Deletes an account given a node.
 *
 */
void deleteAccount(Node *);

/*
 * Deletes the entire list of accounts.
 *
 */
void destroyList(Node **);
#endif
