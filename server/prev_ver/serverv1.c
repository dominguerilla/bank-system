#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>

#include "accounts.h"

#define MAX_ARG_LEN 32

char* processInput(char* );


int main(int argc, char** argv){
	int sockfd, newsockfd, portno, cli_len;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;

	int n;

	/*Call the socket() function*/
	sockfd = socket (AF_INET, SOCK_STREAM, 0);

	if(sockfd<0){
		perror("Error opening initial socket!\n");
		exit(1);
	}

	bzero((char*) &serv_addr, sizeof(serv_addr));
	portno = 5001;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	/*SOCKET BINDING THE HOST ADDRESS*/
	if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))){
		perror("Error doing socket binding!\n");
		exit(1);
	}

	listen(sockfd, 5);
	cli_len = sizeof(cli_addr);

	/*ACTUALLY ACCEPTING THE CONNECTION*/
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len);

	if(newsockfd < 0){
		perror("Error accepting connection: \n");
		exit(1);
	}

	/*READING FROM THE CLIENT SOCKET*/
	char *command, *arg;
	while(1){
		command = malloc(sizeof(char)*7);
		arg = malloc(sizeof(char)*MAX_ARG_LEN);

		memset(buffer, 0, 255);
		n = read(newsockfd, buffer, 255);

		if(n < 0){
			perror("Error reading from socket.\n");
			exit(1);
		}
		
		int argnum = sscanf(buffer, "%s %s", command, arg);
		if(argnum != 2){
			printf("Syntax: [command] [argument]\n");
			free(command);
			free(arg);
			continue;
		}

		
		/**
		 * COMMANDS HERE
		 *
		 */
		if(strcmp(command, "exit") == 0){
			printf("Exiting server.");
			break;	
		}
		else if(strcmp(command, "open") == 0){
			Node *n1 = createAccount(arg, 0.0);
			addToList(n1);
			printf("Created an account for %s.\n", arg);
		}
		else{
			printf("Command was not correct.\n");
		}

		/*n = write(newsockfd, "Message received.\n", 18);

		if(n < 0){
			perror("Error writing to socket.\n");
			exit(1);
		}*/
		free(command);
		free(arg);
	}
	free(command);
	free(arg);
	destroyList();

	return 0;
}

char* processInput(char* input){
	
}
