#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

const int MAX_MSG = 6;
const int PORT = 8080;
typedef struct sockaddr addr;

int initConnection()
{
	// Socket creation
	int sockfd = -1;

	if ( ( sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ){
		printf("Erro - Socket: %d\n", sockfd);
		return -1;
	}

	// Server connection address
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	// Socket connection
	int ret = -1;

	if ( ( ret = connect( sockfd, (addr*)&serverAddr, sizeof(serverAddr) ) ) < 0 ){
		printf("Erro - Connect: %d\n", ret);
		close(sockfd);
		return -1;		
	}
	else {
		// Establish HELLO-OK protocol
		char buffer[MAX_MSG];
		bzero(buffer, MAX_MSG);
	
		strcpy(buffer, "HELLO");
		write(sockfd, buffer, MAX_MSG);

		bzero(buffer, MAX_MSG);

		read(sockfd, buffer, MAX_MSG);
	
		if ( strcmp(buffer, "OK") != 0 ){
			printf("Server didnt return 'OK'\n");
			close(sockfd);
			return -1;
		}

		printf("Server: %s\n", buffer);
	}

	return sockfd;
}

void connIO(int conn)
{
	char buffer[MAX_MSG];
	
	while (1){
		bzero(buffer, MAX_MSG);

		fgets(buffer, MAX_MSG, stdin);
		write(conn, buffer, MAX_MSG);
	
		bzero(buffer, MAX_MSG);

		read(conn, buffer, MAX_MSG);
		printf("Server: %s\n", buffer);
	}
}
