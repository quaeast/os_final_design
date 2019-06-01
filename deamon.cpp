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

using namespace std;


//read the fifo
//daemon
int main()
{
    int C_to_D_id, D_to_C_id, file_id;
    char buffer[PIPE_BUF+1];            //4096+1

    mkfifo(C_to_D_name, 0777);
    mkfifo(D_to_C_name, 0777);



    if(access(C_to_D_name, F_OK)==-1 || access(D_to_C_name, F_OK)==-1){
        printf("can't creat named fifo.\n");
        return 1;
    } else{
        printf("daemon started.\n");
    }

    file_id = ("data.txt", O_RDWR);
    C_to_D_id = open(C_to_D_name, O_RDONLY);
    D_to_C_id = open(D_to_C_name, O_WRONLY);
    while (1){
        read(C_to_D_id, buffer, PIPE_BUF);
        char *main_cmd=strtok(buffer, " ");

        if(strcmp(main_cmd, "exit")==0){
            sprintf(buffer, "%s", "good bye");
            write(D_to_C_id, buffer, PIPE_BUF);
            break;
        } else if (0){
        } else{
            sprintf(buffer, "%s%s", main_cmd, ": command not found.\n");
            write(D_to_C_id, buffer, PIPE_BUF);
        }
        buffer[0]=0;
    }

    //exit
    close(file_id);
    close(C_to_D_id);
    close(D_to_C_id);
//    unlink(C_to_D_name);
//    unlink(D_to_C_name);
    return 0;
}