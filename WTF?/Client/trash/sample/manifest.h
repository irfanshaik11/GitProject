#ifndef manifest_h
#define manifest_h

//#include <openssl/md5.h>
#include <dirent.h>
#include <stdio.h>


int generate_MD5()
{
    unsigned char c[MD5_DIGEST_LENGTH];
    char *filename="file.c";
    int i;
    FILE *inFile = fopen (filename, "rb");
    MD5_CTX mdContext;
    int bytes;
    unsigned char data[1024];
    
    if (inFile == NULL) {
        printf ("%s can't be opened.\n", filename);
        return 0;
    }
    
    MD5_Init (&mdContext);
    while ((bytes = fread (data, 1, 1024, inFile)) != 0)
        MD5_Update (&mdContext, data, bytes);
        MD5_Final (c,&mdContext);
        for(i = 0; i < MD5_DIGEST_LENGTH; i++) printf("%02x", c[i]);
            printf (" %s\n", filename);
            fclose (inFile);
            return 0;
}

int create_manifest(char target_dir[100]){
    DIR *dir;
    struct dirent *ent;
    struct dirent *ent2;
    int num_files = 0;

    if ( (dir = opendir(target_dir) != NULL) ){
        char temp_read_dir[100];
        
        while ((ent = readdir(dir)) != NULL){
            printf("44: %s\n", ent->d_name);
            
//            strcpy(temp_read_dir, target_dir);
//            strcat(temp_read_dir, ent->d_name);
//            FILE *infile = fopen(temp_read_dir, "rb");
//            MD5_CTX mdContext;
//            int bytes;
//            unsigned char data[1024];
            
//            if (inFile == NULL){
//                printf("%s can't be opened\n", temp_read_dir);
//                return -1;
//            }
            
//            MD5_init(&mdContext);

//            num_files++;
//            printf("%s\n", ent->d_name);
        }
        
        closedir(dir);
        
    }else{
        perror("\n      Could not open directory: %s", target_dir);
        return -1;
    }
    
//    char files[num_files][100];
//    if (dir = opendir(target_dir) != NULL){
//        int i = 0;
//
//        while ( (ent2 = readdir(dir)) != NULL){
//            strcpy(files[i], ent->d_name);
//            i++;
//        }
//
//
//
//    }else{
//        perror("\n      Could not reopen directory: %s", target_dir);
//        return -1;
//    }

    
    
    return 0;
}


#endif /* Header_h */
