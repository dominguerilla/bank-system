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
#define MAX_CLIENTS 20


/**
* A single client connection.
*/
typedef struct{
	int sock;
	struct sockaddr address;
	int cli_len;
	Node *head;
	char* current_acc;
} client_t;

/*GLOBAL VARIABLES*/
int client_count = 0;
pthread_mutex_t count_lock;

/*FUNCTION DECLARATIONS*/
void update_client_count(int);
int process_input(char *, int, client_t **);
void client_connect(void *);
void print_accounts(void *);

/**
 Initializes the socket connections and is the 'spawning point' of new threads.
 One thread per client connection.
*/
int main(int argc, char** argv){
	
	Node *list = malloc(sizeof(Node *));
	Account *acc = malloc(sizeof(Account *));
	list->account = acc;
	list->account->name = "";
	list->account->balance = 0.0;
	list->account->in_session = '0';
	list->next = NULL;
	
	if (pthread_mutex_init(&count_lock, NULL) != 0){
		printf("Error initializing count lock!\n");
		return 1;
	}
	
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
	 */
	pthread_create(&print_thread, 0, (void*)print_accounts, (void*) list);
	pthread_detach(print_thread);
	
	printf("Listening...\n");
	while(1){
		client = malloc(sizeof(client_t));
		client->head = list;
		client->sock = accept(servsockfd, &client->address, &client->cli_len);
		
		/**
		* Upon client connection, an integer acknowledgement is sent to the client.
		* The client should know that:
		* 1 = Successful connection
		* -1 = Max number of clients reached--the server will drop the connection
		*
		*/
		if(client->sock < 0){
			printf("Error accepting client connection.\n");
			free(client);
		}else if (client_count >= MAX_CLIENTS){
			int result = -1;
			int n = write(client->sock, &result, sizeof(int));
		}else{
			int result = 1;
			int n = write(client->sock, &result, sizeof(int));
			update_client_count(1);
			pthread_create(&thread, 0, (void*)(client_connect), (void*)client);
			pthread_detach(thread);
		}
	}

	return 0;
}

/**
* Updates the total number of clients
*/
void update_client_count(int update){
	pthread_mutex_lock(&count_lock);
	client_count += update;
	if(client_count < 0){
		client_count = 0;
	}
	pthread_mutex_unlock(&count_lock);
}


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
int process_input(char *buffer, int state, client_t **client){
	
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
	

	Node **list = &((*client)->head);

	char *command, *arg;
	char name[MAX_ARG_LEN];
	command = strtok(parsed, " ");
	arg = strtok(NULL, " ");
	memset(name, '\0', sizeof(name));
	if(arg != NULL){
		strcpy(name, arg);
	}


	if(strcmp(parsed, "exit") == 0){
		printf("Client exits!\n");
		if(state == 1){
			printf("Finishing customer session.\n");
			Node *node = findAccount((*client)->current_acc, *list);
			Account *acc = node->account;
			finishAccount(&acc, state);
			free((*client)->current_acc);
		}
		update_client_count(-1);
		free(parsed);
		return -1;
	}else if(strcmp(command, "open") == 0){
		if(state == 1){
			printf("Still in customer session!\n");
			free(parsed);
			return 1;
		}
		int open = openAccount(list, name);
		char *msg = "Account opened for ";
		printf("%s%s.\n", msg, name);
		if(open == -1){
			printf("Error opening account!\n");
			free(parsed);
			return 0;
		}
		free(parsed);
		return 0;
	}else if(strcmp(command, "start") == 0){
		if(state == 0){
			Node *ptr = findAccount(name, *list);
			if(ptr == NULL){
				free(parsed);
				return 0;
			}
			Account *acc = ptr->account;
			int n = startAccount(&acc, list);
			if(n == 0){
				printf("Account is still in use.\n");
				free(parsed);
				return 0;
			}
			char *current = malloc(strlen(name)*sizeof(char));
			strcpy(current, name);
			(*client)->current_acc = current;
			free(parsed);
			return 1;
		}else{
			printf("Still in session!\n");
			free(parsed);
			return 1;
		}
	}else if(strcmp(command, "credit") == 0){
		if(state == 1){
			float new = updateAccount((*client)->current_acc, list, atof(arg));
			printf("Balance of %s is now %.2f.\n", (*client)->current_acc, new);
			free(parsed);
			return 1;
		}else{
			printf("Not currently in session!\n");
			free(parsed);
			return 0;
		}
	}else if(strcmp(command, "debit") == 0){
		if(state == 1){
			float new = updateAccount((*client)->current_acc, list, atof(arg) * -1.0);
			printf("Balance of %s is now %.2f.\n", (*client)->current_acc, new);
			free(parsed);
			return 1;
		}else{
			printf("Not currently in session!\n");
			free(parsed);
			return 0;
		}
	}else if(strcmp(command, "balance") == 0){
		if(state == 1){
			char* current = (*client)->current_acc;
			float b = getBalance(current, *list);
			printf("Balance: %.2f\n", b);
			free(parsed);
			return 1;
		}else{
			printf("Not currently in session!\n");
			free(parsed);
			return 0;
		}
	}else if(strcmp(command, "finish") == 0){
		if(state == 1){
			printf("Finishing customer session.\n");
			Node *node = findAccount((*client)->current_acc, *list);
			Account *acc = node->account;
			finishAccount(&acc, state);
			free((*client)->current_acc);
			free(parsed);
			return 0;
		}else{
			printf("Not currently in session!\n");
			free(parsed);
			return 0;
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
	list = client->head;
	
	printf("Client connected.\n");
	
	char buffer[MAX_INPUT+1];
	int buffer_len = strlen(buffer);
	int client_state = 0;
	while(1){
		read(client->sock, buffer, MAX_INPUT);
		
		client_state = process_input(buffer, client_state, &client);
		
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
 */
void print_accounts(void* list_ptr){
	Node *list = (Node*)list_ptr;
	while(1){
		sleep(20);
		printf("Number of clients connected: %d\n", client_count);
		printList(list);
	}
	pthread_exit(0);
}

