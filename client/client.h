#ifndef CLIENT_H_
#define CLIENT_H_

/* opens a TCP client connection
   return file descriptor, or -1 on error
*/
int initConnection();

/*
  implements the connection I/O
*/
void connIO(int conn);

#endif
