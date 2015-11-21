#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

#define MAX_INPUT 40

int main(int argc, char *argv[]) {
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	if(argc<3){
		fprintf(stderr, "Usage %s [hostname] [port]\n", argv[0]);
		exit(0);
	}

	portno = atoi(argv[2]);

	/*Creating a socket*/
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd < 0){
		perror("Error opening a socket!\n");
		exit(1);
	}

	server = gethostbyname(argv[1]);

	if(server == NULL){
		perror("Error getting server host name!\n");
		exit(0);
	}

	bzero((char*) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);

	/*CONNECTING TO THE SERVER*/
	if(connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr))<0){
		perror("Error connecting to server!\n");
		exit(1);
	}

	while(1){	
		/*COLLECTING USER INPUT*/
		printf("Enter message: ");
		char *buffer = malloc(sizeof(char)*(MAX_INPUT+1));
		size_t size;
		int input = getline(&buffer, &size, stdin);
		if(input == -1){
			printf("Error reading line.\n");
			free(buffer);
			size = 0;
			continue;
		}else if(input >= MAX_INPUT){
			printf("Input too large! Shorten it!\n");
			free(buffer);
			size = 0;
			continue;
		}

		/*ACTUALLY SENDING USER INPUT*/
		n = write(sockfd, buffer, strlen(buffer));

		if(n<0){
			perror("Error writing to socket!\n");
			exit(1);
		}

		if(strcmp(buffer, "exit\n") == 0){
			printf("Exiting client.\n");
			break;
		}
		free(buffer);
	}

	close(sockfd);

	return 0;
}
