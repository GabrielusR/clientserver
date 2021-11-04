#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "client.h"

const int MAX_MSG = 50;
const int PORT = 8080;
typedef struct sockaddr addr;

int initConnection( char * srvaddr )
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
	inet_pton(AF_INET, srvaddr, &serverAddr.sin_addr);

	// Socket connection
	int ret = -1;

	if ( ( ret = connect( sockfd, (addr*)&serverAddr, sizeof(serverAddr) ) ) < 0 ){
		printf("Erro - Connect: %d\n", ret);
		return -1;		
	}
	else {
		// Establish HELLO-OK protocol
		char buffer[MAX_MSG];
		memset( buffer, '\0', sizeof(buffer) );
	
		strcpy(buffer, "HELLO");
		write(sockfd, buffer, MAX_MSG);

		memset( buffer, '\0', sizeof(buffer) );

		read(sockfd, buffer, MAX_MSG);
	
		printf("Server: %s\n", buffer);
	
		if ( strncmp(buffer, "OK", 2) != 0 ){
			puts("Server didnt return 'OK'");
			return -1;
		}
	}

	return sockfd;
}

void connIO(int conn)
{
	char buffer[MAX_MSG];
	
	while (1){
		memset( buffer, '\0', sizeof(buffer) );

		fgets(buffer, MAX_MSG, stdin);
	
		if ( strncmp(buffer, "EXIT", 4) == 0 ){
			strcpy(buffer, "EXIT");
			write(conn, buffer, MAX_MSG);
			
			puts("Closing connection.");
			
			return;
		}

		write(conn, buffer, MAX_MSG);
	
		memset( buffer, '\0', sizeof(buffer) );

		read(conn, buffer, MAX_MSG);

		printf("Server: %s\n", buffer);
	}
}
