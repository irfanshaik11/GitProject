#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>

#include "manifest.h"
#include "utility.h"

#define MAX 80
#define SA struct sockaddr
void *serverConversation(int sockfd);


int sockfd;
int exitsignal = -1;
void sigintHandler(int sig_num){
    signal(SIGINT, sigintHandler);
    printf("reached sigintHander\n");
    close(sockfd);

    exitsignal = 0;
}

int main(int argc, char ** argv)
{
    if (argc != 2){
        printf("\n      Incorrect Usage");
        printf("\n      Proper Usage is ./WTFserver <portnumber>");
        return -1;
    }
    
    int PORT = atoi(argv[1]);
    int connfd, len;
    struct sockaddr_in servaddr, cli;
    
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockfd == -1) {
        printf("\n      socket creation failed...\n");
        exit(0);
    }
    else{ printf("\n        Socket successfully created..\n"); }
    
    bzero(&servaddr, sizeof(servaddr));
    
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    
    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("\n      socket bind failed...\n");
        exit(0);
    }
    else{ printf("\n        Socket successfully binded..\n"); }
    
    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("\n      Listen failed...\n");
        exit(0);
    }
    else{ printf("\n        Server listening..\n"); }
    
    len = sizeof(cli);
    
    // Accept the data packet from client and verification
    signal(SIGINT, sigintHandler);
    pthread_t thread_ids[10];
    int active_thread = 0;
    while(exitsignal != 0){
        
        connfd = accept(sockfd, (SA*)&cli, &len);
        
        if(exitsignal == 0){
            printf("\n      Closing down server\n");
            break;
        }
        
        if (connfd < 0) {
            printf("\n      server acccept failed...\n");
            exit(0);
        }
        
        
        
        else{ printf("\n        server accept the client...\n"); }
        
        // Function for chatting between client and server
        
        
        pthread_create(&thread_ids[active_thread], NULL, serverConversation, connfd);
//        func(connfd);
        
        active_thread++;
    }
    int i = 0;
    for(i = 0 ; i < active_thread; i++){
        pthread_join(thread_ids[i], NULL);
    }

    // After chatting close the socket
    close(sockfd);

}



void *serverConversation(int sockfd)
{
    char buff[MAX];
    int n;
    bzero(buff, MAX);
    
    // read the message from client and copy it in buffer
    read( sockfd, buff, sizeof(buff) );
    
    printf("From client: %s\t To client : ", buff);
    int i = 0;
    
    printf("\n  37 \n");
    printf("\n buff: %s \n", buff);
    
    char *word = strtok(buff, " ");
    printf("\n 40 \n");
    if (word == NULL){
        printf("\n      Word is NULL\n");
    }
    
    else if ( (strcmp(word, "checkout") == 0) ){
        // TARS REPO AND SENDS TO CLIENT
        word = strtok(NULL, " ");
        char *targetdir = findRepoFilepath(word);
        printf("targetdir is %s", targetdir);
        createZip(targetdir);
        
        long int checkoutfilesize = findFileSize("checkout.tar.gz");
        write(sockfd, (char *) &checkoutfilesize, sizeof(char));
        
        char *outfileBuffer = readFileToBuffer("checkout.tar.gz");
        int i = 0;
        for(i = 0; i < checkoutfilesize; i++){
            printf("\noutfileBuffer[%d] = %x", i, outfileBuffer[i]);
        }
        write(sockfd, outfileBuffer, checkoutfilesize);
        
        
    }else if (strcmp(word, "update") == 0){
        // SENDS .MANIFEST TO CLIENT
        
        printf("\n entered update");
        word = strtok(NULL, " ");
        
        char *manifestfilepath = findManifestFilepath(word);
        if( access( manifestfilepath, F_OK ) == -1 ) {
            //FILE DOES NOT EXIST THEN CREATE THE MANIFEST FILE
            createManifest(manifestfilepath);
        }

        long int manifestfilesize = findFileSize(manifestfilepath);
        write(sockfd, (char *) &manifestfilesize, sizeof(char));
        printf("\nD S 72\n");
        char *outfileBuffer = readFileToBuffer(manifestfilepath);
        write(sockfd, outfileBuffer, manifestfilesize);
        printf("\nD S 74\n");
        free(manifestfilepath);
        return 0;
        
        
    }else if (strcmp(word, "upgrade") == 0){
        
        
        
    }else if (strcmp(word, "commit") == 0){
        
        
        
    }else if (strcmp(word, "push") == 0){
        
        
        
        
    }else if (strcmp(word, "create") == 0){
        //CREATES PROJECT WITH EMPTY .MANIFEST
        word = strtok(NULL, " ");
        createEmptyProject(word);
        
        // send success message to client
        char message[10] = "success";
        write(sockfd, message, sizeof(message));
        
    }else if (strcmp(word, "destroy") == 0){
        //DELETES PROJECT
        word = strtok(NULL, " ");
        destroyProject(word);
        
        // send success message to client
        char message[10] = "success";
        write(sockfd, message, sizeof(message));
        
    }else if (strcmp(word, "add") == 0){
        
        
        
    }else if (strcmp(word, "remove") == 0){
        
    }else if (strcmp(word, "currentversion") == 0){
        
    }else if (strcmp(word, "history") == 0){
        
    }else if (strcmp(word, "rollback") == 0){
        
    }else if (strcmp(word, "exit") == 0){
        printf("\n      Server Exit...\n");
        //            break;
    }else{
        printf("\n      Unrecognized message exiting server");
        //            break;
    }
    
}


/// DESTROY
//        printf("\n 77 \n");
//            word = strtok(NULL, " ");
//            char targetdir[100];
//            strcpy(targetdir, "./Repos/");
//            strcat(targetdir, word);
//            printf("\n 81 \n");
//            if(access(targetdir, F_OK) != -1){
//                // FILE EXISTS
//                char deletestring[200] = "rm -rf ";
//                strcat(deletestring, targetdir);
//                system(deletestring);
//
//                printf("\n      Deleted repo %s", word);
//
//                strcpy(buff, "Success");
//                write(sockfd, buff, sizeof(buff));
//
//            }else{
//                // DIR DOES NOT EXIST
//                printf("\n      Repo %s does not exist, nothing was deleted\n", word);
//
//                strcpy(buff, "Failure");
//                write(sockfd, buff, sizeof(buff));
//
//            }
////            break;


/// UPDATE
//        char targetmanifestfile[100];
//        strcpy(targetmanifestfile, "./Repos/");
//        word = strtok(NULL, " ");
//        strcat(targetmanifestfile, word);
//        printf("\n 112");
//
//        char targetdirectory[100];
//        strcpy(targetdirectory, targetmanifestfile);
//        createManifest(targetdirectory);
//        printf("\n 116\n");
//
//        strcat(targetmanifestfile, "/.manifest");
//        printf("\n targetmanifestfile %s \n", targetmanifestfile);
//        FILE *fptr = fopen(targetmanifestfile, "r");
//
//        if (fptr == NULL){
//            printf("\nfptr is NULL");
//            return 1;
//        }
//
//        printf("\n 118 \n");
//        fseek(fptr, 0, SEEK_END);
//        long int manifestSize = ftell(fptr);
//        fseek(fptr, 0, SEEK_SET);
//        printf("manifestSize is %d", manifestSize);
//        fclose(fptr);
//        int i;
//
//        printf("\n120\n");
//        write(sockfd, (char *) &manifestSize, sizeof(char));
//        printf("\n Done Writing; manifestSize is %d \n", manifestSize);
//
//        char *uniquebuffername = readFileToBuffer(targetmanifestfile);
//
//        write(sockfd, uniquebuffername, manifestSize);
//        printf("\n 134 \n");


/// CREATE
//        printf("\n 51 \n");
//        word = strtok(NULL, " ");
//        char targetdir[100];
//        strcpy(targetdir, "./Repos/");
//        strcat(targetdir, word);
//        printf("\n 56 \n");
//        if(access(targetdir, F_OK) != -1){
//            // FILE EXISTS
//            printf("\n 59 \n");
//            printf("\n      Repo %s already exists, nothing was created", word);
//
//            strcpy(buff, "Success");
//            write(sockfd, buff, sizeof(buff));
//
//        }else{
//            // DIR DOES NOT EXIST
//            printf("\n 64 \n");
//            // MAKE USER REQUESTED REPO
//
//            mkdir(targetdir, 0700);
//            // MAKE .MANIFEST FILE
//            strcat(targetdir, "/.manifest");
//            printf("\n targetdir: %s\n", targetdir);
//
//            FILE *makemanifestptr = fopen(targetdir, "w");
//            fclose(makemanifestptr);
//
//            strcpy(buff, "Failure");
//            write(sockfd, buff, sizeof(buff));
//
//        }
//        printf("\n 72 \n");
////            break;


/// CHECKOUT
//        fseek(fptr, 0, SEEK_END);
//        int checkoutSize = ftell(fptr);
//        rewind(fptr);
//        void *outfileBuffer = malloc(checkoutSize+1);


//            fwrite(outfileBuffer, 1, checkoutSize, fptr);
//        fclose(fptr);
//        write(sockfd, outfileBuffer, checkoutSize);



//        if (strcmp(buff,"exit\n") != 0){
//            bzero(buff, MAX);
//            n = 0;
//            // copy server message in the buffer
//            while ((buff[n++] = getchar()) != '\n');}

// and send that buffer to client
//        write(sockfd, buff, sizeof(buff));

// if msg contains "Exit" then server exit and chat ended.
//        if (strncmp("exit", buff, 4) == 0) {
//            printf("\n      Server Exit...\n");
//            break;
//        }
//    }


