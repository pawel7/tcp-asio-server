#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
// https://stackoverflow.com/questions/24599458/c-client-server-communication-via-socket-in-infinite-loop

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if (argc < 2) {
        printf("Need at least two arguments\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        printf("ERROR opening socket\n");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
                sizeof(serv_addr)) < 0) 
        printf("ERROR on binding\n");
    listen(sockfd,atoi(argv[1]));
    clilen = sizeof(cli_addr);
    while ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) >= 0) {
        if (newsockfd < 0) 
            printf("ERROR on accept\n");
        bzero(buffer,256);
        while(true)
        {   
            n = read(newsockfd,buffer,255);
            if (n < 0)
            {
                printf("ERROR reading from socket\n");
                break;
            }           
            printf("Here is the message: %s\n",buffer);
            n = write(newsockfd,"I got your message\n",18);
            if (n < 0)
            {
                printf("ERROR writing to socket\ns");
                break;
            }
        }
        close(newsockfd);
    }
    close(sockfd);
    return 0;
}  