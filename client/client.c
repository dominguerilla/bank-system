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

	/* CONNECTING TO THE SERVER
	 * If unsuccessful, will wait 3 seconds and try again.
	 *
	 */
	printf("Connecting to server...\n");
	while(connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr))<0){
		usleep(3000);
	}
	printf("Connected to server.\n");
	
	char *buffer = NULL;
	while(1){	
		/*COLLECTING USER INPUT*/
		printf("Enter message: ");
		size_t size;
		int input = getline(&buffer, &size, stdin);
		if(input == -1){
			printf("Error reading line.\n");
			memset(buffer, '\0', strlen(buffer));
			size = 0;
			continue;
		}else if(input >= MAX_INPUT){
			printf("Input too large! Shorten it!\n");
			memset(buffer, '\0', strlen(buffer));
			size = 0;
			continue;
		}
		
		char *msg = malloc(sizeof(char)*(input+1));
		strcpy(msg, buffer);
		msg[input] = '\0';

		/*ACTUALLY SENDING USER INPUT*/
		sleep(2);
		n = write(sockfd, msg, strlen(buffer));

		if(n<0){
			perror("Error writing to socket!\n");
			exit(1);
		}

		if(strcmp(buffer, "exit\n") == 0){
			printf("Exiting client.\n");
			break;
		}
		memset(buffer, '\0', strlen(buffer));
		free(msg);
	}
	free(buffer);
	close(sockfd);

	return 0;
}
