//
//  main.c
//  WTF?
//
//  Created by 206452150 on 4/26/19.
//  Copyright © 2019 206452150. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>


int main(int argc, const char * argv[]) {
    // insert code here...
    if (argc < 2){
        printf("\n      Incorrect Usage:");
        printf("\n      Correct usage is: ./WTF <flag> |IP/hostname| |port|");
        return -1;
    }
    
    if (strcmp(argv[0],"./WTFserver") == 0){
        char command[100];
        
        strcpy(command, "./Client/WTFserver ");
        strcat(command, argv[1]);
        system(command);
        
        return 0;
    }
    
    if (strcmp(argv[1], "configure") == 0){
        
        
    }else if (strcmp(argv[1], "checkout") == 0){
        
    }else if (strcmp(argv[1], "update") == 0){
        
    }else if (strcmp(argv[1], "commit") == 0){
        
    }else if (strcmp(argv[1], "push") == 0){
        
    }else if (strcmp(argv[1], "create") == 0){
        
    }else if (strcmp(argv[1], "destroy") == 0){
        
    }else if (strcmp(argv[1], "add") == 0){
        
    }else if (strcmp(argv[1], "remove") == 0){
        
    }else if (strcmp(argv[1], "currentversion") == 0){
        
    }else if (strcmp(argv[1], "history") == 0){
        
    }else if (strcmp(argv[1], "rollback") == 0){
        
    }else{
        printf("\n      Incorrect Usage:");
        printf("\n      Correct usage is: ./WTF <flag> |IP/hostname| |port|");

    }
    
    printf("Hello, World!\n");
    
    return 0;
}
