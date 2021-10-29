#ifndef SERVER_H_
#define SERVER_H_

/* Starts a TCP server connection
   return file descriptor or -1 on error
*/
int initServer();

/* Accept inbound connections
   return connection fd or -1 on error
*/
int acceptConnection(int socket);

/* implements connection I/O
*/
void connIO(int conn);

#endif
