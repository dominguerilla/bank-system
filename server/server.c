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
*
*/
int process_input(char *command, char *arg, Node **list){
	
	if(strcmp(command, "open") == 0){
		openAccount(list, arg, 0.0);
		/*
		Node *new = malloc(sizeof(Node));
		new = createAccount(arg, 0.0);
		if(new != NULL){
			addToList(new, list);
			printf("Created account for %s.\n", new->account->name);
		}else{
			printf("Error making account.\n");
		}*/
	}
	return 0;
}

/**
 The entry function that new thread clients use. It reads from the socket, passes it
 to process_input, and will loop until the client sends 'exit\n'.
*/
void client_connect(void *client_ptr){

	client_t *client;
	
	Node *list = NULL;

	if(!client_ptr){
		pthread_exit(0);
	}
	
	client = (client_t*) client_ptr;
	
	printf("Client connected.\n");
	
	char *buffer = malloc(sizeof(char)*(MAX_INPUT+1));
	while(1){
		read(client->sock, buffer, MAX_INPUT);
		/*buffer[MAX_INPUT] = '\0';*/
		
		int i;
		char *parsed = NULL;
		for(i = 0; i < strlen(buffer); i++){
			if(buffer[i] == '\n'){
				if(i == 0){
					printf("Please enter a command.\n");
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
			continue;
		}

		printf("CLIENT> %s\n", parsed);
		


		if(strcmp(parsed, "exit") == 0){
			printf("Client exits!\n");
			break;
		}else if(strcmp(parsed, "print") == 0){
			printList(list);
		}
		
		char *command, *arg;
		arg = strtok(parsed, " ");
		command = strcpy(arg, command);
		arg = strtok(NULL, " ");

		printf("Command: %s , Arg: %s\n", command, arg);


		/*
		char *arg;
		if(sscanf(buffer, "%s %s ", command, arg) != 2){
			printf("Proper syntax: [command] [argument]\n");
			free(command);
			free(arg);
			memset(buffer, '\0', strlen(buffer));
			continue;
		}*/
		/*command[MAX_COMMAND_LEN] = '\0';
		arg[MAX_ARG_LEN] = '\0';*/

		/*int processed_input = process_input(command, arg, *list);
		free(command);
		free(arg);*/
		free(parsed);
		memset(buffer, '\0', strlen(buffer));
	}
	free(buffer);
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
