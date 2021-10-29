#include <stdio.h>
#include <string.h>

#include "client.h"

int main()
{
	int sockfd = -1;

	if ( ( sockfd = initConnection() ) < 0 ){
		return -1;
	}

	connIO(sockfd);	

	close(sockfd);

	return 0;
}
