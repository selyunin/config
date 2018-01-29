/*
 * socket_server_v2.cpp
 *
 *  Created on: Jan 27, 2018
 *      Author: selyunin
 */


/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main__(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0){
        error("ERROR opening socket");
     }

     int enable = 1;
     if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
         error("setsockopt(SO_REUSEADDR) failed");
     }

     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_addr.s_addr = INADDR_ANY;
//***
     struct in_addr inp;
	int inet_res = inet_aton("192.168.0.85", &inp);
//	int inet_res = inet_aton("127.0.0.1", &inp);
	if (inet_res == 0){
		printf("inet_aton failed!\n");
	}
	printf("inet_aton = %x\n", inp.s_addr);
     serv_addr.sin_addr = inp;
//****

     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd,
                 (struct sockaddr *) &cli_addr,
                 &clilen);
     if (newsockfd < 0)
          error("ERROR on accept");
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);

     const char* server_response = "Server got your message";
     n = write(newsockfd,server_response,strlen(server_response));

     if (n < 0) error("ERROR writing to socket");

     close(newsockfd);
     close(sockfd);

     return 0;
}

