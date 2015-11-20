#include <stdlib.h>
#include <stdio.h>

#include "accounts.h"


int main(int argc, char** argv){
	Node *n1 = createAccount("Carlos", 200.00);
	Node *n2 = createAccount("Christopher", 199.00);
	Node *n3 = createAccount("Zachary", 5000.10);
	Node *n4 = createAccount("Daniel", 3459.80);

	addToList(n1);
	addToList(n2);
	addToList(n3);
	addToList(n4);
	
	printList();
	
	destroyList();
	return 0;
}
