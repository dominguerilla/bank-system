#include <stdlib.h>
#include <stdio.h>

#include "accounts.h"


int main(int argc, char** argv){
	Node *list = NULL;
	
	openAccount(&list, "Carlos");
	openAccount(&list, "Christopher");
	openAccount(&list, "Zachary");
	openAccount(&list, "Daniel");

	
	printList(list);
	
	Node *testFind1 = findAccount("Carlos", list);
	Node *testFind2 = findAccount("Christopher", list);
	Node *testFind3 = findAccount("Zachary", list);
	Node *testFind4 = findAccount("Daniel", list);
	Node *testFind5 = findAccount("Franco", list);
	
	printf("Found %s.\n", testFind1->account->name);
	printf("Found %s.\n", testFind2->account->name);
	printf("Found %s.\n", testFind3->account->name);
	printf("Found %s.\n", testFind4->account->name);
	
	updateAccount("Carlos", &list, 1024.0);
	updateAccount("Christopher", &list, 2048.0);
	updateAccount("Christopher", &list, -1024.0);
	updateAccount("Daniel", &list, 412.0);
	
	getBalance("Carlos", list);
	getBalance("Christopher", list);
	getBalance("Zachary", list);
	getBalance("Daniel", list);
	getBalance("Franco", list);
	
	printList(list);
	destroyList(&list);
	printList(list);
	destroyList(&list);
	return 0;
}
