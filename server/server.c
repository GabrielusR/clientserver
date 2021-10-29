#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

const int MAX_MSG = 6;
const int PORT = 8080;
typedef struct sockaddr addr;

int initServer()
{
	// Socket creation
	int sockfd = -1;

	if ( ( sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ){
		printf("Erro - Socket: %d\n", sockfd);
		return -1;
	}
	
	// Address binded
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	// Socket binding
	int ret = -1;

	if ( ( ret = bind( sockfd, (addr*)&serverAddr, sizeof(serverAddr) ) ) < 0 ){
		printf("Erro - Bind: %d\n", ret);
		close(sockfd);
		return -1;
	}

	// Socket listening
	if ( ( ret = listen(sockfd, 10) ) < 0 ){
		printf("Erro - Listen: %d\n", ret);
		close(sockfd);
		return -1;
	}
	
	return sockfd;
}

int acceptConnection(int sockfd)
{
	// Socket inbound connection
	struct sockaddr_in clientAddr; 
	int len = sizeof(clientAddr);

	int conn = -1;

	if ( ( conn = accept( sockfd, (addr*)&clientAddr, &len ) ) < 0 ){
		printf("Erro - Accept: %d\n", conn);
		return -1;
	}
	else {
		// Establish HELLO-OK protocol
		char buffer[MAX_MSG];
		bzero(buffer, MAX_MSG);		
	
		read(conn, buffer, MAX_MSG);
	
		if ( ( strcmp(buffer, "HELLO") ) != 0 ){
			close(conn);
			return -1;	
		}

		printf("Client: %s\n", buffer);
		
		bzero(buffer, MAX_MSG);
	
		strcpy(buffer, "NOT OK");	
		write(conn, buffer, MAX_MSG);
	}
	
	return conn;
}

void connIO(int conn)
{
	char buffer[MAX_MSG];

	while (1){
		bzero(buffer, MAX_MSG);
		
		read(conn, buffer, MAX_MSG);

		write(conn, buffer, MAX_MSG);
	}
}
