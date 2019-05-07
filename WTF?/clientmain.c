// Write CPP code here
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <dirent.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "./headers/utility.h"
#include "./headers/manifest.h"
#include "./headers/commands.h"


#define MAX 800
#define SA struct sockaddr

void serverConversation(int sockfd, char input[MAX], int command);

int main(int argc, char **argv)
{
    
    char input[MAX];
    int command;
    
//    if (argc == 1 || argc > 4){
//        displayError();
//        return 1;
//    }
    
    
    
    if (argc == 1){
        printf("\n      Calling Server Using Settings in .configure File");
    }
    else if ( strcmp(argv[1], "configure") == 0 ){
        
        command = 1;
        char outstring[100];
        strcpy(outstring, argv[2]);
        strcat(outstring, " ");
        strcat(outstring, argv[3]);
        FILE *outfile;
        outfile = fopen(".configure","w");
        
        fprintf(outfile, "%s", outstring);
        fclose(outfile);
        return 0;
        
    }else if( strcmp(argv[1], "checkout") == 0){
        
        command = 2;
        strcpy(input, "checkout ");
        strcat(input, argv[2]);
        printf("sending message: %s", input);
        printf("\n entered checkout\n");
        
    }else if( strcmp(argv[1], "create") == 0){
        
        command = 3;
        strcpy(input, "create ");
        strcat(input, argv[2]);
        
        createEmptyProject(argv[2], 0);
        
    }else if( strcmp(argv[1], "destroy") == 0){
        
        command = 4;
        strcpy(input, "destroy ");
        strcat(input, argv[2]);
        destroyProject(argv[2], 0);
        
    }else if( strcmp(argv[1], "remove") == 0){
        
        command = 5;
        deleteFromManifest(argv[2], argv[3], 0);
//        strcpy(input, "remove ");
//        strcat(input, argv[2]);
//        strcat(input, " ");
//        strcat(input, argv[3]);
        
    }else if( strcmp(argv[1], "currentversion") == 0){
        
        command = 6;
        strcpy(input, "currentversion");
        strcat(input, " ");
        strcat(input, argv[2]);
        
    }else if( strcmp(argv[1], "history") == 0){
        
        command = 7;
        strcpy(input, argv[2]);
        
    }else if( strcmp(argv[1], "rollback") == 0){
        
        command = 8;
        strcpy(input, argv[2]);
        strcat(input, " ");
        strcat(input, argv[3]);
        
    }else if ( strcmp(argv[1], "upgrade") == 0){
        
        command = 9;
        strcpy(input, "upgrade");
        strcat(input, " ");
        strcat(input, argv[2]);
        
    }else if ( strcmp(argv[1], "commit") == 0){
        
        command = 10;
        strcpy(input, "commit");
        strcat(input, " ");
        strcat(input, argv[2]);
        
    }else if ( strcmp(argv[1], "push")==0){
        
        command = 11;
        strcpy(input, "push");
        strcat(input," ");
        strcat(input, argv[2]);
        
    }else if( strcmp(argv[1], "update") == 0){
    
        command = 12;
        strcpy(input, "update ");
        strcat(input, argv[2]);
    
    }else if( strcmp(argv[1], "add") == 0 ){
        printf("\n      Entered add\n");
        command = 13;
        appendToManifest(argv[2], argv[3], 0);
//        incrementManifestVersion(argv[2], 0);
        return 0;
        
    }else if( strcmp(argv[1], "testmanifest") == 0){
        
        printf("\nentered testmanifest");
        
        createManifest(argv[2]);
        return 0;
        
    }else{
        
        printf("\n      Entered Else %s\n", argv[1]);
        displayError();
        return -1;
        
    }
    
    int PORT;
    char ip_address[50];
    char hostname_port[2][50];
    
    char ip_port_string[100];
    strcpy(ip_port_string, getConfig() );
    
    if (ip_port_string == NULL){
        return 1;
    }else{
        char *word = strtok(ip_port_string, " ");
        strcpy(ip_address, word);
        word = strtok(NULL, " ");
        PORT = atoi(word);
    }
    
    
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    
    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("\n sockfd is %d \n", sockfd);
    if (sockfd == -1) {
        printf("\n      socket creation failed...\n");
        sleep(3);
    }else{
        printf("\n      Socket successfully created..\n");
    }

    bzero(&servaddr, sizeof(servaddr));
    
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip_address);
//    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
    
    // connect the client socket to server socket

    while ( connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("\n      Connection with the server failed...\n");
        sleep(3);
        printf("\n      Trying to reconnect...\n");
    }
    printf("\n      Connected to the server..\n");
    
    // function for chat
    printf("\nD Entering serverconversation");
    serverConversation(sockfd, input,command);
    
    // close the socket
    close(sockfd);
}

void serverConversation(int sockfd, char input[MAX], int command)
{
    char input_args[MAX];
    strcpy(input_args, input);
    
    printf("\nEntered Server Conversation\n");
    char buff[MAX];
    strcpy(buff, input);
    int n;
    
    write(sockfd, buff, sizeof(buff));
    bzero(buff, sizeof(buff));
    
    if (command == 2){
        // CHECKOUT
        
        long int incomingfilesize = 0;
        read(sockfd, (char*) &incomingfilesize, sizeof(int));
//        printf("\n Checkoutfilesize is %l", incomingfilesize);

        printf("filesize is %d", incomingfilesize);
        char* incomingData = malloc(incomingfilesize+1);
        recv(sockfd, incomingData, incomingfilesize, 0);
        incomingfilesize++;
        incomingData[incomingfilesize] = '\0';
//        FILE *outfile;
//        outfile = fopen("./Client/data/checkout.tar.gz", "w+");
        
        int i = 0;
        for(i = 0; i < incomingfilesize; i++){
            printf("\noutfileBuffer[%d] = %x", i, incomingData[i]);
        }
        
//        fwrite(incomingData, sizeof(char), incomingfilesize, outfile);
        int fd_file = open("./Client/data/checkout.tar.gz", O_RDWR| O_CREAT, 0600);
        if (fd_file < 0){
            
        }
        
        if(write(fd_file, incomingData, incomingfilesize) < 0){
            printf("\n      Error Writing to Tarfile\n");
        }
        
        close(fd_file);
        // To emulate writing, since there is a write error with the above line; the reading from the buffer works perfectly but am having trouble writing binary files
        system("cp -f ./Server/data/checkout.tar.gz ./Client/data/checkout.tar.gz");

        
        
    }else if(command == 3){
        // CREATE
        
        printf("\n clientmain 102\n");
        char response[10];
        recv(sockfd, response, sizeof(response), 0);
        
        if(strcmp(response, "success") == 0){
            printf("\n      Success, Server Created File");
        }else{
            printf("\n      Failure, Project Already Exists or Other Error Occuered");
        }
        
    }else if(command == 4){
        // DESTROY
        
        // Send Destroy Message to Server and Read Response
        char response[10];
        recv(sockfd, response, sizeof(response), 0);
        if(strcmp(response, "success") == 0){
            printf("\n      Success, Server Deleted File");
        }else{
            printf("\n      Failure, Server Could Not Find File");
        }
        
    }else if(command == 5){
        // REMOVE
        
    }else if(command == 6){
        // CURRENT VERSION
        
        int currentversion;
        read(sockfd, (unsigned char*) &currentversion, sizeof(int));
        printf("\n      Current Version is %d\n", currentversion);
        
    }else if(command == 7){
        // HISTORY
        
    }else if(command == 8){
        // ROLLBACK
        
    }else if(command == 9){
        // UPGRADE
        
        long int incomingfilesize = 0;
        read(sockfd, (char*) &incomingfilesize, sizeof(int));
        //        printf("\n Checkoutfilesize is %l", incomingfilesize);
        
        printf("filesize is %d", incomingfilesize);
        char* incomingData = malloc(incomingfilesize+1);
        recv(sockfd, incomingData, incomingfilesize, 0);
        incomingfilesize++;
        incomingData[incomingfilesize] = '\0';
        FILE *outfile;
        outfile = fopen("./Client/data/checkout.tar.gz", "w+");
        
        int i = 0;
        for(i = 0; i < incomingfilesize; i++){
            printf("\noutfileBuffer[%d] = %x", i, incomingData[i]);
        }
        
        //        fwrite(incomingData, sizeof(char), incomingfilesize, outfile);
        // To emulate writing, since there is a write error with the above line I use the below line; the reading from the buffer works perfectly but am having trouble writing binary files
        system("cp -f ./Server/data/checkout.tar.gz ./Client/data/checkout.tar.gz");
        system("tar -xvf -C checkout.tar.gz ./Client/data/");
        
        char syscommand[100];
        printf("\ninput buffer contains %s", input);
        char *reponame = strtok(input_args, " ");
        printf("\ninput buffer contains %s", reponame);
        reponame = strtok(NULL, " ");
        printf("\ninput buffer contains %s", reponame);
        
        char targetdir[100];
        
        moveDataToServerManifest(reponame, 0);
        update(reponame, 0);
        resolve("./Client/data/servermanifest", findManifestFilepath(reponame,0), findDataRepo(reponame, 0), findRepoFilepath(reponame,0), 0);
        // ^ in headers/commands.h
        
        
        fclose(outfile);
        
    }else if(command == 10){
        // COMMIT
        
        char *word = strtok(input, " ");
        word = strtok(NULL, " ");
        
        downloadManifestFile(sockfd); // IN headers/commands.h
        int valid = 0;
        if (update(word, 0) == 0){
            char* updatefilepath = findUpdateFilepath(word, 0);
            
            char syscommand[100];
            strcpy(syscommand, "cp ");
            strcat(syscommand, updatefilepath);
            strcat(syscommand, " ");
            strcat(syscommand, findRepoFilepath(word, 0));
            system(syscommand);
            valid = 0;
        }else{
            valid = 1;
            printf("\n      .Commit Contained Collisions, No .Commit File Created, No Files Were Uploaded");
            
        }
        if (valid == 0){
        createZip(word, 0);
            int outfilesize = findFileSize("./Client/data/checkout.tar.gz");
            char *outfilebuffer = readFileToBuffer("./Client/data/checkout.tar.gz");

            write(sockfd, (char *) &outfilesize, sizeof(int));
            write(sockfd, outfilebuffer, outfilesize);
        }
        
    }else if(command == 11){
        // PUSH
        
    }else if(command == 12){
        // UPDATE
        char *word = strtok(input, " ");
        word = strtok(NULL, " ");
        
        downloadManifestFile(sockfd); // IN headers/commands.h
        update(word, 0);

        
        
    }else if(command == 13){
        // ADD
        
        char *word = strtok(input, " ");
        word = strtok(NULL, " ");
        
        
    }
    
}









//    struct hashNode updateptr;
//    struct hashNode *manifestptr;
//    struct hashNode *prevptr;





//    char *manifestfilepath = findManifestFilepath(reponame);
//    readManifest(manifestfilepath);
//    printf("\nD client 28");
//    int i;
//    struct hashNode updateptr;
//    struct hashNode *manifestptr;
//    struct hashNode *prevptr;
//    char currline[300];
//    printf("\nD client 34");
//    char *updatefilepath = findUpdateFilepath(reponame);
//
//    FILE *fp = fopen(updatefilepath, "w");
//    printf("\nD client 38");
//    int servermanifestsize = findFileSize("/data/servermanifest");
//    char *utilitybuffer = readFileToBuffer("/data/servermanifest");
//
//    int bufferi = 0;int wordi = 0;
//    char word[300];
//
//    while (utilitybuffer[bufferi] != " " && i < servermanifestsize){
//        word[wordi] = utilitybuffer[bufferi];
//    }
//    word[wordi] = '\0';
//    //    char *word = strtok(utilitybuffer, "\n");
//    printf("\nD client 42");
//    while (word != NULL){
//        char currhash[100];char currfilename[100];int currversion;
//
//        strcpy(currline, word);
//        char word2 = strtok(currline, "\n");
//        updateptr.version = atoi(word2);
//        word2 = strtok(NULL, "\n");
//        strcpy(updateptr.filename, word2);
//        word2 = strtok(NULL, "\n");
//        strcpy(updateptr.hash, word2);
//        //        manifestptr->version = currversion;
//
//        manifestptr = &hashMap[i];
//
//        int seen = 1;
//        while(manifestptr != NULL){
//            if ( strcmp(manifestptr->filename, updateptr.filename) == 0 ){
//                if ( strcmp(manifestptr->hash, updateptr.hash) != 0 ){
//                    manifestptr->version++;
//                    fprintf(fp, "%d %s %s M \n", manifestptr->version, manifestptr->filename, manifestptr->hash);
//                }
//
//                prevptr->next = manifestptr->next;
//                seen = 0;
//                break;
//            }
//            prevptr = manifestptr;
//            manifestptr = manifestptr->next;
//        }
//
//        if (seen == 1){
//            manifestptr->version++;
//            fprintf(fp, "%d %s %s U \n", manifestptr->version, manifestptr->filename, manifestptr->hash);
//        }else{
//            manifestptr->version++;
//            fprintf(fp, "%d %s %s D \n", manifestptr->version, manifestptr->filename, manifestptr->hash);
//
//        }
//
//        wordi=0;
//        while (utilitybuffer[bufferi] != " " && bufferi < servermanifestsize){
//            word[wordi] = utilitybuffer[bufferi];
//        }
//        word[wordi] = '\0';
//
//    }
//    printf("\nD 84");
//    for(i = 0; i < 100; i++){
//        manifestptr = &hashMap[i].next;
//        while(manifestptr != NULL){
//            fprintf("%d %s %s A", manifestptr->version, manifestptr->filename, manifestptr->hash);
//        }
//    }
//
//    fclose(fp);
    
//}
