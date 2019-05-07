#ifndef utility_h
#define utility_h

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


long int findFileSize(char dirpath[100]){
    // RETURNS FILE LENGTH
    char symbol;
    FILE *fp = fopen(dirpath, "r");
    
    fseek(fp, 0, SEEK_END);
    long int filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    fclose(fp);
    return filesize;
}

char * readFileToBuffer(char dirpath[100]){
    //RETURNS BUFFER WITH CONTENTS OF FILE
    printf("\n entered readFileToBuffer");
    char symbol;
    long int filesize = findFileSize(dirpath);
    char *source = malloc(filesize);
    
    FILE*fp = fopen(dirpath, "r");
    
    if(fp != NULL)
    {
        int i = 0;
        while((symbol = getc(fp)) != EOF)
        {
            source[i] = symbol;
            i++;
        }
        fclose(fp);
    }
    
    return source;
}

char* findRepoFilepath(char targetrepo[100]){
    //RETURNS ./Repos/[targetrepo]
    char *targetdir = malloc(sizeof(char)* 100);
    strcpy(targetdir, "./Repos/");
    strcat(targetdir, targetrepo);
    return targetdir;
}

int createZip(char targetdir[100]){
    //CREATES ZIP OF TARGETDIR
    char checkoutstring[200];
    
    strcpy(checkoutstring, "tar -zcvf checkout.gz ");
    strcat(checkoutstring, targetdir);
    
    system(checkoutstring);
    
    
    return 0;
}


void createEmptyProject(char target[100]){
    // CREATES PROJECT IN ./REPO/[TARGET]
    char *targetdir = findRepoFilepath(target);
//    char targetdir[100];
//    strcpy(targetdir, "./Repos/");
//    strcat(targetdir, target);
    if(access(targetdir, F_OK) == -1){
        mkdir(targetdir, 0700);
    }
    
    strcat(targetdir, "/.manifest");
    FILE *fp = fopen(targetdir, "w");
    close(fp);

}

void destroyProject(char target[100]){
    // DELETES PROJECT IN ./REPO/[TARGET]
    printf("\n 91 \n");
    char *targetdir = findRepoFilepath(target);
    printf("\n 93 \n");
    char syscommand[200] = "rm -rf ";
    strcat(syscommand, targetdir);
    system(syscommand);
    printf("\n97\n");
}

void displayError(){
    perror("\n      Incorrect Usage");
    perror("\n      Proper Usage is ./WTF <command> |arg1| |arg2|");
}

char **getConfig(){
    printf("\n 19 \n");
    FILE *configfileptr = fopen(".configure", "rb");
    if(configfileptr == NULL){
        printf("\n      Could not find the .manifest file");
        printf("\n      Run ./WTF configure <IP> <Hostname>");
        char err = (char) malloc(sizeof(char));
        err = '\0';
        return err;
    }
    printf("\n 28 \n");
    fseek(configfileptr, 0, SEEK_END);
    long configfilelength = ftell(configfileptr);
    fseek(configfileptr, 0, SEEK_SET);
    char *configbuffer = (char *) malloc(configfilelength* sizeof(char));
    printf("\n 33 \n");
    fread(configbuffer, sizeof(char), configfilelength, configfileptr);
    fclose(configfileptr);
    printf("\n 36 \n");
    char*p = strtok(configbuffer, " ");
    
    
    printf("\n 41 \n");
    
    char **ip_port;
    int i;
    for(i = 0; i < 2; i++){
        ip_port[i] = malloc(50*sizeof(char));
        strcpy(ip_port[i], p);
        p = strtok(NULL, " ");
    }
    
    for(i=0; i < 2; i++){
        printf("\nip_port[%d] = %s", i, ip_port[i]);
    }
    printf("\n 44 \n");
    free(configbuffer);r

    return ip_port;
    
}




#endif
