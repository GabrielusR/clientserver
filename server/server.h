#ifndef SERVER_H_
#define SERVER_H_

#include <arpa/inet.h>

/* Starts a TCP server connection
   return file descriptor or -1 on error
*/
int initServer();

/* Accept inbound connections
*/
void acceptConnections(int socket);

/* implements connection I/O
*/
void connIO(int conn);

/* test if the connectiona addr is already connected
*/
int isConnected( struct sockaddr_in connAddr );

#endif
