#include <stdio.h>

#include "server.h"

int main()
{
	int sockfd = -1;
	
	if ( ( sockfd = initServer() ) < 0 ){
		close(sockfd);
		return -1;
	}

	acceptConnections(sockfd);
	
	return 0;
}
