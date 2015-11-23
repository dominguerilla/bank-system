#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(){
	char buffer[26] = "open Alpha\n";

	char* first = strtok(buffer, " \n");
	char* second = strtok(NULL, " \n");

	printf("BUFFER: %s\n", buffer);
	printf("FIRST: %s , SECOND: %s\n", first, second);
	return 0;
}
