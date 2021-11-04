#include <stdio.h>

#include "client.h"

int main(int argc, char *argv[])
{
	int sockfd = -1;

	if ( !argv[1] ){
		puts("Inform the remote server address.\n");
		return -1;
	}

	if ( ( sockfd = initConnection( argv[1] ) ) < 0 ){
		close(sockfd);
		return -1;		
	}

	connIO(sockfd);	

	close(sockfd);

	return 0;
}
