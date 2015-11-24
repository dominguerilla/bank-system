#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>

#include "accounts.h"

#define MAX_COMMAND_LEN 12
#define MAX_ARG_LEN 32
#define MAX_INPUT 40

/**
* A single client connection.
*/
typedef struct{
	int sock;
	struct sockaddr address;
	int cli_len;
} client_t;

/**
* Processes the input from the client, given the command, argument, and state
* of the client. 
*
* The 'state' is simply whether or not the client is in a session.
* -1: Client wishes to exit
*  0: Client is connected, but is not in an account session
*  1: Client is connected and is in an account session
*
*/
int process_input(char *buffer, int state){
	
	int i;
	char *parsed = NULL;
	int len = strlen(buffer);
	for(i = 0; i < len; i++){
		if(buffer[i] == '\n'){
			if(i == 0){
				/*If it's just a newline*/
				break;
			}
			parsed = malloc(sizeof(char)*(i+1));
			strncpy(parsed, buffer, i);
			parsed[i] = '\0';
			break;
		}
	}
	if(parsed == NULL){
		printf("Error parsing buffer.\n");
		return state;
	}

	printf("CLIENT> %s\n", parsed);
	
	if(strcmp(parsed, "exit") == 0){
		printf("Client exits!\n");
		return -1;
	}




	char *command, *arg;
	command = strtok(parsed, " ");
	arg = strtok(NULL, " ");

	printf("Command: %s , Arg: %s\n", command, arg);
	if(strcmp(command, "open") == 0){
		/*openAccount(list, arg, 0.0);*/
	}else if(strcmp(command, "start") == 0){
		if(state == 0){
			/*startAccount(list, arg);*/
		}else{
			printf("Still in session!\n");
		}
	}else if(strcmp(command, "credit") == 0){
		if(state == 1){
			/*creditAccount(account, amount);*/
		}else{
			printf("Not currently in session!\n");
		}
	}else if(strcmp(command, "debit") == 0){
		if(state == 1){
			/*debitAccount(account, amount);*/
		}else{
			printf("Not currently in session!\n");
		}
	}else if(strcmp(command, "balance") == 0){
		if(state == 1){
			/*printf(balance);*/
		}else{
			printf("Not currently in session!\n");
		}
	}else if(strcmp(command, "finish") == 0){
		if(state == 1){
			/*closeSession();*/
		}else{
			printf("Not currently in session!\n");
		}
	}
	
	free(parsed);

	return state;
}

/**
 The entry function that new thread clients use. It reads from the socket, passes it
 to process_input, and will loop until the client sends 'exit'.
*/
void client_connect(void *client_ptr){

	client_t *client;
	
	Node *list = NULL;

	if(!client_ptr){
		pthread_exit(0);
	}
	
	client = (client_t*) client_ptr;
	
	printf("Client connected.\n");
	
	char buffer[MAX_INPUT+1];
	int buffer_len = strlen(buffer);
	int client_state = 0;
	while(1){
		read(client->sock, buffer, MAX_INPUT);
		
		client_state = process_input(buffer, client_state);

		memset(buffer, '\0', buffer_len);
		if(client_state == -1) break;
	}
	close(client->sock);
	free(client);
	sleep(1);
	pthread_exit(0);
}

/*
 * Prints out the list of accounts every 20 seconds.
 *
 *
void print_accounts(){
	while(1){
		sleep(20);
		printList();
	}
	pthread_exit(0);
}*/

/**
 Initializes the socket connections and is the 'spawning point' of new threads.
 One thread per client connection.
*/
int main(int argc, char** argv){
	Node **list = NULL;
	int servsockfd, port;
	struct sockaddr_in address;
	pthread_t thread;
	pthread_t print_thread;

	client_t *client;

	/*HARD CODED PORT FOR NOW*/
	port = 5001;

	/*OPENING THE SOCKET*/
	servsockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(servsockfd<0){
		perror("Error opening socket.\n");
		exit(1);
	}
	
	/*BINDING OF SOCKET*/
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	if(bind(servsockfd, (struct sockaddr*)&address, sizeof(struct sockaddr_in)) < 0){
		perror("Error binding socket.\n");
		exit(1);
	}

	/*LISTENING ON PORT*/
	if(listen(servsockfd, 5)<0){
		perror("Error listening on port!\n");
		exit(1);
	}

	/*
	 * INITIALIZING ACCOUNT PRINTING THREAD
	 *
	 *
	pthread_create(&print_thread, 0, (void*)print_accounts, NULL);
	pthread_detach(print_thread);
	*/
	printf("Listening...\n");
	
	while(1){
		client = malloc(sizeof(client_t));
		client->sock = accept(servsockfd, &client->address, &client->cli_len);

		if(client->sock < 0){
			printf("Error accepting client connection.\n");
			free(client);
		}else{
			pthread_create(&thread, 0, (void*)(client_connect), (void*)client);
			pthread_detach(thread);
		}
	}

	return 0;
}
