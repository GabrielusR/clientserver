#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#include <sys/time.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "server.h"

const int MAX_CONN = 3;
const int MAX_MSG = 50;
const int PORT = 8080;

typedef struct sockaddr addr;

//connections fd array
int connfds[3];	

//connections addr array
struct sockaddr_in activeConns[3];

int initServer()
{
	// Socket creation
	int sockfd = -1;

	if ( ( sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ){
		printf("Erro - Socket: %d\n", sockfd);
		return -1;
	}
	
	int ret = -1;
	int opt = 1;
	
	if ( ( ret = setsockopt( sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt) ) ) < 0 ){
		printf("Erro - setsockopt: %d\n", ret);
		return -1;
	}
	
	// Address binded
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	inet_pton(AF_INET, "10.0.0.105", &serverAddr.sin_addr );
	
	// Socket binding
	if ( ( ret = bind( sockfd, (addr*)&serverAddr, sizeof(serverAddr) ) ) < 0 ){
		printf("Erro - Bind: %d\n", errno);
		return -1;
	}

	// Socket listening
	if ( ( ret = listen(sockfd, 3) ) < 0 ){
		printf("Erro - Listen: %d\n", ret);
		return -1;
	}

	return sockfd;
}

int isConnected( struct sockaddr_in connAddr )
{
	int i = 0;

	for (i = 0; i < MAX_CONN; i++)
		if (connAddr.sin_addr.s_addr == activeConns[i].sin_addr.s_addr)
			return 0;//return 1;
			
	return 0;
}

void acceptConnections(int sockfd)
{	
	memset( connfds, 0, sizeof(connfds) );

	//polled fds
	fd_set fds;

	int i = 0;
	int conn = -1;

	struct sockaddr_in connAddr;
	int len = sizeof(connAddr);

	char buffer[MAX_MSG];
	int numConns = 0;

	//check for connections/activity
	while (1){
		memset( buffer, '\0', sizeof(buffer) );	

		FD_ZERO(&fds);

		FD_SET(sockfd, &fds);

		int maxFd = sockfd;

		for (i = 0; i < MAX_CONN; i++){
			if ( connfds[i] > 0 )
				FD_SET(connfds[i], &fds);

			if ( connfds[i] > maxFd )
				maxFd = connfds[i];
		}

		int activity = select(maxFd+1, &fds, NULL, NULL, NULL);

		if ( (activity < 0) && (errno == EINTR) )
			printf("Erro - Select: %d\n", errno);
		
		//if sockfd has activity theres an inbound connection
		if ( FD_ISSET(sockfd, &fds) ){					
			if ( ( conn = accept( sockfd, (addr*)&connAddr, &len ) ) < 0 ){
				printf("Erro - Accept: %d\n", conn);
			}
			else {
				if ( !isConnected(connAddr) && (numConns < MAX_CONN) ){
					read(conn, buffer, MAX_MSG);
				
					if ( strncmp(buffer, "HELLO", 5) == 0 ){
						printf("Client: %s\n", buffer);
						
						memset( buffer, '\0', sizeof(buffer) );
					
						strcpy(buffer, "OK\n");	
						write(conn, buffer, MAX_MSG);
			
						for (i = 0; i < MAX_CONN; i++){
							if( connfds[i] == 0 ){
								
								numConns += 1;		
								connfds[i] = conn;
								activeConns[i] = connAddr;
								
								break;
							}
						}
					}
					else {
						printf("Client start message is not 'HELLO'\n");
						close(conn);
					}
				}
				else {
					strcpy(buffer, "Client connected or maximum connections achieved.\n");
					write(conn, buffer, MAX_MSG);
					close(conn);
				}
			}
		} // if (FD_ISSET(sockfd

		// checks activity for other connections
		for (i = 0; i < MAX_CONN; i++){
			if ( FD_ISSET(connfds[i], &fds) )
				connIO(connfds[i]);
		}		

	} //while (1)	
}

void connIO(int conn)
{
	char buffer[MAX_MSG];

	memset( buffer, '\0', sizeof(buffer) );

	read(conn, buffer, MAX_MSG);
	
	if ( strncmp(buffer, "DATE", 4) == 0 ){
		memset( buffer, '\0', sizeof(buffer) );

		time_t now = time(NULL);
		
		strcpy( buffer, asctime( localtime(&now) )  );			
	}
	
	int i = 0;
	
	if ( strncmp(buffer, "EXIT", 4) == 0){
		for (i; i < MAX_CONN; i++){
			if (connfds[i] == conn){
					close(conn);
					connfds[i] = 0;
					activeConns[i].sin_addr.s_addr = 0;
					return;
			}	
		}
	}
	
	write(conn, buffer, MAX_MSG);
}
