#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include "old_main.h"

/*
	CODE ADAPTED FROM http://www.linuxhowtos.org/C_C++/socket.htm
*/

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[])
{
	printf("Your paddle number is: %d\n", getpid());
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[256];
	if (argc < 3) {
	   fprintf(stderr,"usage %s hostname port\n", argv[0]);
	   exit(0);
	}
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, 
		 (char *)&serv_addr.sin_addr.s_addr,
		 server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
		error("ERROR connecting");

	old_main();

	// for some reason prints "Pls enter the msg" twice...??!
//	printf("Your bid: "); // this is going to be a problem, need to reformulate code so that I can just access the entered bid from the writing program.  Maybe through global (local) var?
	strcpy(buffer, entered_bid);
//	bzero(buffer,256);
//	fgets(buffer,255,stdin);
	n = write(sockfd,buffer,strlen(buffer));
	if (n < 0) 
		 error("ERROR writing to socket");
	bzero(buffer,256);
	n = read(sockfd,buffer,255);
	if (n < 0) 
		 error("ERROR reading from socket");
	printf("%s\n",buffer);
	close(sockfd);

/*	
	printf("Please enter the message: ");
	bzero(buffer,256);
	fgets(buffer,255,stdin);
	n = write(sockfd,buffer,strlen(buffer));
	if (n < 0) 
		 error("ERROR writing to socket");
	bzero(buffer,256);
	n = read(sockfd,buffer,255);
	if (n < 0) 
		 error("ERROR reading from socket");
	printf("%s\n",buffer);
	close(sockfd);
*/
	return 0;
}