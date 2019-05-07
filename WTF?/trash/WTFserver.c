#include <stdio.h>

#include <netb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 800
#define PORT 8080
#define SA struct sockaddr
void func(int sockfd){
    
    char buff[MAX];
    int n;
    
    for(;;){
        bzero(buff, MAX);
    }
    
    read(sockfd, buff, sizeof(buff) );
    
    print("From client: %s\t To client : ", buff);
    bzero(buff,MAX);
    n = 0;
    
    while ( (buff[n++] = getchar()) != '\n' );
    
    write(sockfd, buff, sizof(buff));
    
    if (strncmp("exit", buff, 4) == 0){
        printf("Server Exit...\n");
        break;
    }
    
}

int main(int argc, const char * argv[]){
    if (argc < 2){
        printf("\n      Incorrect Usage:");
        printf("\n      Correct usage is: ./WTFserver |INTEGER|");
        return -1;
    }
    
    int portnumber;
    portnumber = atoi(argv[1]);
    
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockfd == -1){
        printf("socket creation failed...\n");
        exit(0);
    }
    else{
        printf("Socket successfully created...\n");
    }
    bzero(&servaddr, sizeof(servaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
    
    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else{
        printf("Server listening..\n");
    }
    
    len = sizeof(cli);
    
    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server acccept failed...\n");
        exit(0);
    }
    else{
        printf("server acccept the client...\n");
    }
    
    // Function for chatting between client and server
    func(connfd);
    
    // After chatting close the socket
    close(sockfd);

    
    
}
