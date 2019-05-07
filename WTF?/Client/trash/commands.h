#ifndef commands_h
#define commands_h

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <dirent.h>
#include <unistd.h>
#include <arpa/inet.h>


void downloadManifestFile(int sockfd){
    printf("\nEntering downloadManifestFile\n");
    int incomingfilesize;
    read(sockfd, (unsigned char*) &incomingfilesize, sizeof(int));
    
    printf("filesize is %d", incomingfilesize);
    char *incomingData = malloc(incomingfilesize+1);
    recv(sockfd, incomingData, incomingfilesize, 0);
    incomingData[incomingfilesize] = '\0';
    FILE *outfile;
    
    outfile = fopen("./data/servermanifest", "w");
    int i = 0;

    fwrite(incomingData, 1, incomingfilesize, outfile);
    free(incomingData);

}





#endif
