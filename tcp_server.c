/* A simple server in the internet domain using TCP
 * Answer the questions below in your writeup
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    /* 1. What is argc and *argv[]?
     * argc usually refers the number arguments that are going to be inputed within the command line for the 
       program. Meanwhile, *argv[] is the argumetn vector which is the array of cstrings in the command line.
       Reference: https://www.cs.odu.edu/~zeil/cs330/f13/Public/faq/faq-htmlsu7.html

     */
    int sockfd, newsockfd, portno;
    /* 2. What is a UNIX file descriptor and file descriptor table?
     *  A file descriptor is a numerical value that is given to a file on an operating system while a file descriptor
     table is a collection of integers that are pointer which point to a file descriptor or an open file entry
     */
    socklen_t clilen;

    struct sockaddr_in serv_addr, cli_addr;
    /* 3. What is a struct? What's the structure of sockaddr_in?
     * A struct is essentially like a class which has variables that are associated to the overall thing
       However, a struct is a value type unlike the reference type that classes usually have. Furthermore, 
       the sockaddr uses memebers like sin_addr, sin_port, and sin_family. The struct sockaddr_in is basically 
       helping the program by telling the specfic address and port 
     */
    
    int n;
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    /* 4. What are the input parameters and return value of socket()
     * There are three inputs in a socket which are SOCK_STREAM, SOCK_DGRAM, and SOCK_SEQPACKET. Meanwhile, the
      output of the socket function is a non negative number. If you do get a negative number, this means that the
      socket has an error
     */
    
    if (sockfd < 0) 
       error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0) 
             error("ERROR on binding");
    /* 5. What are the input parameters of bind() and listen()?
     * The input parameters of bind() is the address of the socket which is a pointer, there is total size of
     the socket and the unique name of the socket which is also known as the descriptor. For listen(), the 
     parameters are total connections, the socket() variable, and the descriptor again. 
     */
    
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    
    while(1) {
        /* 6.  Why use while(1)? Based on the code below, what problems might occur if there are multiple simultaneous connections to handle?
        *  When running a while loop with 1, you are essentially running an infinite loop where it will keep 
         running in the server. However, the issue with this is that if have multiple similtaneous connections
         running, then packages or data can be lost orget sent to the wrong client operating system
        */
        
	char buffer[256];
        newsockfd = accept(sockfd, 
                    (struct sockaddr *) &cli_addr, 
                    &clilen);
	/* 7. Research how the command fork() works. How can it be applied here to better handle multiple connections?
         * The fork command allows more than one client to be runned since the fork command creates another server 
         that is similar to its orginial parent server. If we use it, it will allow us to use multiple clients without
         slowing or hurting the overall process of the program 
         */
        
	if (newsockfd < 0) 
             error("ERROR on accept");
	bzero(buffer,256);
        
	n = read(newsockfd,buffer,255);
        if (n < 0) 
            error("ERROR reading from socket");
        printf("Here is the message: %s\n",buffer);
        n = write(newsockfd,"I got your message",18);
        if (n < 0) 
            error("ERROR writing to socket");
        close(newsockfd);
    }
    close(sockfd);
    return 0; 
}
  
/* This program makes several system calls such as 'bind', and 'listen.' What exactly is a system call?
It seems like a system call is way for the program to ask a certain service on an operating system. In this case,
we were trying to connect to the sever of the rasberry pi 
 *
 */
