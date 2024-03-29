/*
** server5.c -- a stream socket server demo
** Modificado pelo Hae para atender um unico cliente.
** compila server5
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#define PORT "3490" // the port users will be connecting to
#define BACKLOG 1 // how many pending connections queue will hold
#define MAXDATASIZE 256 // max number of bytes we can get at once
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if(sa->sa_family == AF_INET){
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr); 
}

int main(void)
{
    int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo,*p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    memset(&hints,0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    if ((rv = getaddrinfo(NULL, PORT,&hints,&servinfo)) !=0){ 
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for(p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1){
            perror("server: socket");
            continue;
        }

        if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,&yes,sizeof(int)) == -1)
        {
            perror("setsockopt");
            exit(1);
        }
        if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1){
            close(sockfd);
            perror("server: bind");
            continue;
        }
        break;
    }

    freeaddrinfo(servinfo); // all done with this structure
    if(p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if(listen(sockfd, BACKLOG) ==-1)
    {
        perror("listen");
        exit(1);
    }

    printf("server: waiting for connections...\n");
    while(1){
        sin_size = sizeof their_addr;
        new_fd = accept
        (sockfd,(struct sockaddr *)&their_addr,&sin_size);

        if (new_fd == -1) {
            perror("accept");
            continue;
        } else break;
    }

    inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s, sizeof s);
    printf("server: got connection from %s\n", s);
    close(sockfd); // doesn't need the listener anymore

    strcpy(buf,"Mensagem #1 do server para client");
    if (send(new_fd, buf, strlen(buf)+1, 0) == -1) perror("send");
    if (recv(new_fd, buf, MAXDATASIZE, 0) == -1) perror("recv");
    printf("server: received '%s'\n",buf);
    strcpy(buf,"Mensagem #3 do server para client");
    if (send(new_fd, buf, strlen(buf)+1, 0) == -1) perror("send");

    close(new_fd);
    return 0;
}
