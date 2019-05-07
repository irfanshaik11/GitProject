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


void moveDataToServerManifest(char reponame[100], int clientside){
    char datamanifest[100];
    char endrepo[100];
    char syscommand[300];
    
    if (clientside == 0){
        strcpy(datamanifest, "./Client/data/");
    }else{
        strcpy(datamanifest, "./Server/data/");
    }
    
    strcat(datamanifest, reponame);
    strcat(datamanifest, "/.manifest");
    
    if (clientside == 0){
        strcpy(endrepo, "./Client/data/servermanifest");
    }else{
        strcpy(endrepo, "./Server/data/servermanifest");
    }
    
    strcpy(syscommand, "mv ");
    strcat(syscommand, datamanifest);
    strcat(syscommand, " ");
    strcat(syscommand, endrepo);
    printf("\n D U executing command %s", syscommand);
    system(syscommand);
    
}
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
    
    FILE*fp = fopen(dirpath, "rb");
    
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

void removeFilepath(char filepath[100]){
    char syscommand2[100];
    strcpy(syscommand2, "rm -rf ");
    strcat(syscommand2, filepath);
    system(syscommand2);

}

char *findFilepathInRepo(char reponame[100], char filename[100], int clientside){
    char *targetdir = malloc(sizeof(char)*100);
    if (clientside == 0){
        strcpy(targetdir, "./Client/Repos/");
    }
    else{
        strcpy(targetdir, "./Server/Repos/");
    }
    strcat(targetdir, reponame);
    strcat(targetdir, "/");
    strcat(targetdir, filename);
    return targetdir;
}

char* findRepoFilepath(char targetrepo[100], int clientside){
    //RETURNS ./Repos/[targetrepo]
    char *targetdir = malloc(sizeof(char)* 100);
    if (clientside == 0){
        strcpy(targetdir, "./Client/Repos/");
    }
    else{
        strcpy(targetdir, "./Server/Repos/");
    }
    strcat(targetdir, targetrepo);
    return targetdir;
}

char *findUpdateFilepath(char targetrepo[100], int clientside){
    
    char *targetdir = malloc(sizeof(char)* 100);
    
    if (clientside == 0){
        strcpy(targetdir, "./Client/");
    }else{
        strcat(targetdir, "./Server/");
    }
    strcat(targetdir, "Repos/");
    strcat(targetdir, targetrepo);
    strcat(targetdir, "/.update");
    return targetdir;
    
}


char *findManifestFilepath(char targetrepo[100], int clientside){
    
    char *targetdir = malloc(sizeof(char)* 100);
    if (clientside == 0){
        strcpy(targetdir, "./Client/Repos/");
    }
    else{
        strcpy(targetdir, "./Server/Repos/");
    }
    strcat(targetdir, targetrepo);
    strcat(targetdir, "/.manifest");
    return targetdir;

}

int createZip(char targetrepo[100], int clientside){
    //CREATES ZIP OF TARGETDIR
    printf("\n      Entered createZip D U\n");
    char checkoutstring[200];
    if (clientside == 0){
        strcpy(checkoutstring, "tar -zcvf ./Client/data/checkout.tar.gz -C ./Client/Repos ");
    }
    else{
        strcpy(checkoutstring, "tar -zcvf ./Server/data/checkout.tar.gz -C ./Server/Repos ");
    }

//    strcpy(checkoutstring, "tar -g -zcvf checkout.gz ");
    strcat(checkoutstring, targetrepo);
    printf("\n D U 124, running %s\n", checkoutstring);

    system(checkoutstring);
    
    return 0;
}

char *findDataRepo(char reponame[100], int clientside){
    char *targetrepo = malloc(100 * sizeof(char));
    
    if (clientside == 0){
        strcpy(targetrepo, "./Client/data/");
    }else{
        strcpy(targetrepo, "./Server/data/");
    }
    
    strcat(targetrepo, reponame);
    
    return targetrepo;
}

void createEmptyProject(char targetrepo[100], int clientside){
    // CREATES PROJECT IN ./REPO/[TARGET]
    printf("\n D U 97\n");
    
    char *targetdir = findRepoFilepath(targetrepo, clientside);
    if(access(targetdir, F_OK) == -1){
        mkdir(targetdir, 0700);
    }
    free(targetdir);
    
    char *manifestfilepath = findManifestFilepath(targetrepo, clientside);
    printf("\nCreating file %s\n", manifestfilepath);
    FILE *fp = fopen(manifestfilepath, "w");
    if (fp == NULL){printf("\nfp is NULL\n");}
    fprintf(fp, "1 Manifest \n");
    printf("\nD U wrote to file %s\n", manifestfilepath);
    close(fp);
    
    free(manifestfilepath);

}

void destroyProject(char target[100], int clientside){
    // DELETES PROJECT IN ./REPO/[TARGET]
    char *targetdir = findRepoFilepath(target, clientside);
    
    char syscommand[200] = "rm -rf ";
    strcat(syscommand, targetdir);
    system(syscommand);
}

void displayError(){
    perror("\n      Incorrect Usage");
    perror("\n      Proper Usage is ./WTF <command> |arg1| |arg2|");
}

char *getConfig(){
    printf("\nD entered getconfig\n");
    
    FILE *configfileptr = fopen("./.configure", "rb");
    if(configfileptr == NULL){
        printf("\n      Could not find the .manifest file");
        printf("\n      Run ./WTF configure <IP> <Hostname>");
        char err = (char) malloc(sizeof(char));
        err = '\0';
        return err;
    }
    
    fseek(configfileptr, 0, SEEK_END);
    long configfilelength = ftell(configfileptr);
    fseek(configfileptr, 0, SEEK_SET);
    char *configbuffer = (char *) malloc(configfilelength* sizeof(char));
    
    fread(configbuffer, sizeof(char), configfilelength, configfileptr);
    fclose(configfileptr);
    return configbuffer;
//    char*p = strtok(configbuffer, " ");




//    char **ip_port;
//    int i;
//    for(i = 0; i < 2; i++){
//        printf("\n p is %s\n", p);
//        *ip_port[i] = malloc(50*sizeof(char));
//        printf("\n 154\n");
//        strcpy(ip_port[i], p);
//        p = strtok(NULL, " ");
//    }
//
//    for(i=0; i < 2; i++){
//        printf("\nip_port[%d] = %s", i, ip_port[i]);
//    }
//    printf("\n 44 \n");
//    free(configbuffer);

//    return **ip_port;
    
}




#endif
