//
// Created by ZIdongFang on 2019-06-01.
//

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>


#include <iostream>

#include "config.h"
#include "mystring.h"


using namespace std;

int main()
{
    int C_to_D_id, D_to_C_id;
    char buffer[PIPE_BUF+1];
    printf("welcome to use fsh (๑•̀ㅂ•́)و✧\n\n");
    C_to_D_id = open(C_to_D_name, O_WRONLY);
    D_to_C_id = open(D_to_C_name, O_RDONLY);
    while (1){
        printf("> ");
        cin.getline(buffer, sizeof(buffer));
        write(C_to_D_id, buffer, PIPE_BUF);
        read(D_to_C_id, buffer, PIPE_BUF);

        // judge exit
        if(strcmp(buffer, "good bye\n")==0){
            printf(buffer);
            break;
        }
        else if(strcmp(buffer, "vim")==0){
            printf("%s\n", "vim_started");
            vim(buffer);
            write(C_to_D_id, buffer, PIPE_BUF);
            read(D_to_C_id, buffer, PIPE_BUF);
            printf(buffer);
        }
        else if(strcmp(buffer, "clear")==0){
            system("clear");
        }
        else{
            printf(buffer);
        }
        buffer[0]=0;
    }
    close(C_to_D_id);
    close(D_to_C_id);
    return 0;
}