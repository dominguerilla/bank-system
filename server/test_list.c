#include <stdlib.h>
#include <stdio.h>

#include "accounts.h"


int main(int argc, char** argv){
	Node *list = NULL;
	
	openAccount(&list, "Carlos", 200.00);
	openAccount(&list, "Christopher", 199.00);
	openAccount(&list, "Zachary", 5000.10);
	openAccount(&list, "Daniel", 3459.80);

	
	printList(list);
	
	destroyList(list);
	return 0;
}
