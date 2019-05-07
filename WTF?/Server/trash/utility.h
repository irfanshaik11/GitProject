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

char* findRepoFilepath(char targetrepo[100]){
    //RETURNS ./Repos/[targetrepo]
    char *targetdir = malloc(sizeof(char)* 100);
    strcpy(targetdir, "./Repos/");
    strcat(targetdir, targetrepo);
    return targetdir;
}

char *findUpdateFilepath(char targetrepo[100]){

    char *targetdir = malloc(sizeof(char)* 100);
    strcpy(targetdir, "./Repos/");
    strcat(targetdir, targetrepo);
    strcat(targetdir, "/.update");
    return targetdir;
    
}


char *findManifestFilepath(char targetrepo[100]){
    
    char *targetdir = malloc(sizeof(char)* 100);
    strcpy(targetdir, "./Repos/");
    strcat(targetdir, targetrepo);
    strcat(targetdir, "/.manifest");
    return targetdir;

}

int createZip(char targetdir[100]){
    //CREATES ZIP OF TARGETDIR
    char checkoutstring[200];
    
    strcpy(checkoutstring, "tar -g -zcvf checkout.gz ");
    strcat(checkoutstring, targetdir);
    
    system(checkoutstring);
    
    
    return 0;
}



void createEmptyProject(char targetrepo[100]){
    // CREATES PROJECT IN ./REPO/[TARGET]
    printf("\n D U 97\n");
    char *targetdir = findRepoFilepath(targetrepo);
    if(access(targetdir, F_OK) == -1){
        mkdir(targetdir, 0700);
    }
    free(targetdir);
    
    char *manifestfilepath = findManifestFilepath(targetrepo);
    printf("Creating file %s", manifestfilepath);
    FILE *fp = fopen(manifestfilepath, "w");
    fprintf(fp, "1 Manifest \n");
    close(fp);
    
    free(manifestfilepath);

}

void destroyProject(char target[100]){
    // DELETES PROJECT IN ./REPO/[TARGET]
    char *targetdir = findRepoFilepath(target);
    
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
    
    FILE *configfileptr = fopen(".configure", "rb");
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
