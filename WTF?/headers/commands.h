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

#include "utility.h"
#include "manifest.h"

void downloadManifestFile(int sockfd){
    printf("\nEntering downloadManifestFile\n");
    int incomingfilesize;
    read(sockfd, (unsigned char*) &incomingfilesize, sizeof(int));
    
    printf("\nfilesize is %d\n", incomingfilesize);
    char *incomingData = malloc(incomingfilesize+1);
    printf("\nD C 21\n");
    recv(sockfd, incomingData, incomingfilesize, 0);
    incomingData[incomingfilesize+1] = '\0';
    printf("\nD C 24\n");
    FILE *outfile;
//    if (clientside == 0){
    
    outfile = fopen("./Client/data/servermanifest", "w");
//    else{
//        outfile = fopen("./Server/data/servermanifest", "w");
//    }
    int i = 0;
    printf("\nD C 33 incoming data: %s\n", incomingData);
    
    fprintf(outfile, "%s", incomingData);
//    fwrite(incomingData, 1, incomingfilesize, outfile);
    printf("\nD C 35\n");
    free(incomingData);
    fclose(outfile);
}

char* _findFilepathInUpdateRepo(char targetrepo[100], char targetfile[100], int clientside){
    char *targetdir = malloc(100*sizeof(char));
    if (clientside == 0){
        strcpy(targetdir, "./Client/data/");}
    else{
        strcpy(targetdir, "./Server/data/");
    }
    strcat(targetdir, targetrepo);
    strcat(targetdir, "/");
    strcat(targetdir, targetfile);
    
    return targetdir;
}

void resolve(char updatefilepath[100], char manifestfilepath[100], char inputdir[100], char outputdir[100], int clientside){
    
    printf("\nUpdatefilepath: %s", updatefilepath);
    printf("\nManifestfilepath: %s", manifestfilepath);
    printf("\nInputdir: %s", inputdir);
    printf("\nOutputdir: %s", outputdir);
    
//    char *manifestBuffer = readFileToBuffer(manifestFilepath);
//    int manifestfilesize = findFileSize(manifestFilepath);

    char *updatebuffer = readFileToBuffer(updatefilepath);
    int updatefilesize = findFileSize(updatefilepath);
    
    struct hashNode *root;
    struct hashNode *prev;
    char *line = strtok(updatebuffer, "\n");
    while(line == NULL){
        char *word = strtok(line, " ");
        int updatefile_version;
        char updatefile_name[100];
        char updatefile_hash[100];
        char action[4];
        
        updatefile_version = atoi(word);
        
        word = strtok(NULL, " ");
        strcpy(updatefile_name, word);
        
        word = strtok(NULL, " ");
        strcpy(updatefile_hash, word);
        
        word = strtok(NULL, " ");
        strcpy(action, word);
        
        int key = createKey(updatefilepath);
        root = &hashMap[key];
        
        if ( strcmp(action, "U") == 0 ){
            
            insertToHashMap(updatefile_version, updatefile_name, updatefile_hash);
            
            char * origpath = _findFilepathInUpdateRepo(updatefile_name, inputdir, 0);
            
            char syscommand[100];
            strcpy(syscommand, "cp ");
            strcat(syscommand, origpath);
            strcat(syscommand, " ");
            strcat(syscommand, outputdir);
            
            system(syscommand);
            
        }else if ( strcmp(action, "A") == 0){
            
            insertToHashMap(updatefile_version, updatefile_name, updatefile_hash);
            
            char * origpath = _findFilepathInUpdateRepo(updatefile_name, inputdir, 0);
            
            char syscommand[100];
            strcpy(syscommand, "cp ");
            strcat(syscommand, origpath);
            strcat(syscommand, " ");
            strcat(syscommand, outputdir);
            
            
        }else if ( strcmp(action, "D") == 0){

            _deleteFromManifest(updatefile_version, updatefile_name, updatefile_hash);
            
            // CREATE THE FILEPATH TO DELETE
            char targetfile[100];
            strcpy(targetfile, outputdir);
            strcat(targetfile, "/");
            strcat(targetfile, updatefile_name);
            
            char syscommand[100];
            strcpy(syscommand, "rm -rf ");
            strcat(syscommand, outputdir);
            
            system(syscommand);
            
        }else{
            
        }
        
        line = strtok(NULL, "\n");
    }
    
    // CREATE NEW MANIFEST FILE
    createManifestFile(manifestfilepath);
    // DELETE REPO IN /DATA
    removeFilepath(inputdir);
    
    free(updatebuffer);
}

int update(char reponame[100], int clientside){
    // RETURNS 0 IF NO CHANGES OR CHANGES ARE OF THE FORM U, A, D
    // RETURNS 1 OTHERWISE
    int retval = 0;
    printf("\n Entered update");
    char *manifestfilepath;
    if (clientside == 0){
        manifestfilepath = findManifestFilepath(reponame, 0);
        
    }else{
        manifestfilepath = findManifestFilepath(reponame, 1);
    }
    //    if( access( manifestfilepath, F_OK ) == -1 ) {
    //        // FILE DOES NOT EXIST
    //        createManifest(manifestfilepath);
    //    }
    
    int manifestversion = getManifestVersion(manifestfilepath);
    
    readManifest(manifestfilepath);
    
    char*updatefilepath;
    if (clientside == 0){
        updatefilepath = findUpdateFilepath(reponame, 0);
    }else{
        updatefilepath = findUpdateFilepath(reponame, 1);
    }
    
    printf("\n manifestfilepath: %s\n", manifestfilepath);
    printf("\n updatefilepath: %s\n", updatefilepath);
    
    long int servermanifestsize;
    if (clientside == 0){
        servermanifestsize = findFileSize("./Client/data/servermanifest");
    }else{
        servermanifestsize = findFileSize("./Server/data/servermanifest");
    }
    printf("\nD H 322 %s\n", servermanifestsize);
    char servermanifestbuffer[servermanifestsize];
    printf("\nD H 323\n");
    int servermanifestversion;
    if (servermanifestsize != 0){
        if (clientside == 0){
            servermanifestversion = getManifestVersion("./Client/data/servermanifest");
        }else{
            servermanifestversion = getManifestVersion("./Server/data/servermanifest");
        }
    }else{
        servermanifestversion = 0;
    }
    printf("\nD H 324\n");
    if (clientside == 0){
        strcpy(servermanifestbuffer, readFileToBuffer("./Client/data/servermanifest"));
    }else{
        strcpy(servermanifestbuffer, readFileToBuffer("./Server/data/servermanifest"));
    }
    printf("\nD H 305 \n");
    printf("\n servermanifestbuffer %s", servermanifestbuffer);
    char *currline = strtok(servermanifestbuffer, "\n");
    printf("\nD H 307 \n");
    
    struct hashNode serverptr;
    struct hashNode* manifestptr;
    struct hashNode* updateptr;
    struct hashNode* prevptr;
    
    FILE* fp = fopen(updatefilepath, "w");
    if (fp == NULL){
        printf("\n Write Error\n");
    }else{
        printf("\n Writing to %s", updatefilepath);
    }
    int ismanifestheader = 0;
    while(currline != NULL){
        if (ismanifestheader == 0){
            currline = strtok(NULL, "\n");
            ismanifestheader = 1;
            continue; }
        char *word = strtok(currline, " ");
        serverptr.version = atoi(word);
        word = strtok(NULL, " ");
        strcpy(serverptr.filename, word);
        word = strtok(NULL, " ");
        strcpy(serverptr.hash, word);
        
        int key = createKey(serverptr.filename);
        manifestptr = &hashMap[key];
        manifestptr = manifestptr->next;
        int seen = 1;
        int addedtag = 1;
        while (manifestptr != NULL){
            if ( strcmp(manifestptr->filename, serverptr.filename) == 0 ){
                if (manifestversion == servermanifestversion){// MANIFEST SAME
                    
                    if (manifestptr->version == serverptr.version){ // FILEVERSIONS THE SAME
                        if( strcmp(manifestptr->hash, serverptr.hash) == 0 ){
                            // DO NOTHING
                            printf("\n      DO NOTHING FOR FILE %s", serverptr.filename);
                            int addedtag = 0;
                        }else{
                            // UPLOAD
                            printf("\n      %d %s %s U \n", serverptr.version, serverptr.filename, serverptr.hash);
                            
                            fprintf(fp, "%d %s %s U \n", serverptr.version, serverptr.filename, serverptr.hash);
                            int addedtag = 0;
                        }
                        
                    }else{ // FILEVERSIONS DIFFERENT
                        if ( strcmp(manifestptr->hash, serverptr.hash) == 0 ){
                            printf("\n      ERROR ON FILE %s", serverptr.hash);
                        }else{
                            printf("\n      ERROR ON FILE %s", serverptr.hash);
                        }
                        
                    }
                }else{ // MANIFEST DIFFERENT
                    if (manifestptr->version == servermanifestversion){ //FILEVERSION SAME
                        if ( strcmp(manifestptr->hash, serverptr.hash) == 0 ){
                            printf("\n      DO NOTHING FOR FILE %s",  serverptr.filename);
                        }else{
                            printf("\n      %d %s %s U \n", serverptr.version, serverptr.filename, serverptr.hash);
                            
                            fprintf(fp, "%d %s %s U \n", serverptr.version, serverptr.filename, serverptr.hash);
                        }
                        
                    }else{ // FILEVERSIONS DIFFERENT
                        if ( strcmp(manifestptr->hash, serverptr.hash) == 0 ){
                            printf("\n      %d %s %s M \n", serverptr.version, serverptr.filename, serverptr.hash);
                            
                            fprintf(fp, "%d %s %s M \n", serverptr.version, serverptr.filename, serverptr.hash);
                            retval = 1;
                            
                        }else{
                            printf("\n      %d %s %s C \n", serverptr.version, serverptr.filename, serverptr.hash);
                            
                            fprintf(fp, "%d %s %s C \n", serverptr.version, serverptr.filename, serverptr.hash);
                        }
                        
                    }
                    
                    
                }
                prevptr->next = manifestptr->next;
                seen = 0;
                break;
            }
            prevptr = manifestptr;
            manifestptr = manifestptr->next;
        }
        
        if (seen == 1){
            printf("\n D C 421 %s\n", serverptr.filename);
            if (serverptr.version >= manifestversion){
                printf("\n      %d %s %s A \n", serverptr.version, serverptr.filename, serverptr.hash);
                
                fprintf(fp, "%d %s %s A \n", serverptr.version, serverptr.filename, serverptr.hash);
                
            }else{
                printf("\n      %d %s %s D \n", serverptr.version, serverptr.filename, serverptr.hash);
                
                fprintf(fp, "%d %s %s D \n", serverptr.version, serverptr.filename, serverptr.hash);
                
            }
        }
        
        
        currline = strtok(NULL, "\n");
    }
    
    // CHECK FOR DELETES OR ADDS IN LOCAL FILE
    //    int i;
    //    for(i = 0; i < 100; i++){
    //        manifestptr = &hashMap[i];
    //        manifestptr = manifestptr->next;
    //
    //        while (manifestptr != NULL){
    //            printf("\n D C 436 %d\n", manifestversion);
    //            if (manifestptr->version > servermanifestversion){
    //                printf("\n D C 442 >\n");
    //                printf("\n      %d %s %s A \n", manifestptr->version, manifestptr->filename, manifestptr->hash);
    //
    //                fprintf(fp, "%d %s %s A \n", manifestptr->version, manifestptr->filename, manifestptr->hash);
    //
    //            }else{
    //                printf("\n D C 448 <= \n");
    //                printf("\n      %d %s %s A \n", manifestptr->version, manifestptr->filename, manifestptr->hash);
    //
    //                fprintf(fp, "%d %s %s A \n", manifestptr->version, manifestptr->filename, manifestptr->hash);
    //            }
    //            manifestptr = manifestptr->next;
    //        }
    //
    //    }
    
    fclose(fp);
    free(manifestfilepath);
    free(updatefilepath);
    free(currline);
    
    return retval;
}




#endif
