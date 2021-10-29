#include <stdio.h>
#include <string.h>

#include "server.h"

int main()
{
	int sockfd = -1;
	
	if ( ( sockfd = initServer() ) < 0 ){
		return -1;
	}
	
	int conn = -1;

	if ( ( conn = acceptConnection(sockfd) ) < 0){
		//TODO Should close only this conn
		close(sockfd);
		return -1;
	}

	connIO(conn);

	close(sockfd);
	
	return 0;
}
