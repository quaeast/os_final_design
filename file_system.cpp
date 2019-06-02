//
// Created by ZIdongFang on 2019-06-02.
//

#include "file_system.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


//size MB
int init_disk(char *disk_name, int size){
    if(access(disk_name, F_OK)==0){
        return 0;
    }
    int f_d = open (disk_name, O_CREAT | O_WRONLY, 0777);
    if(f_d==-1){
        printf("open failed");
        return -1;
    }
    int res = ftruncate64(f_d, size*1024);
    close(f_d);
    return res;
}


int init_file_sys(char *disk_name, int size){
    if(access(disk_name, F_OK)!=0){
        return -1;
    }

    return 0;
}

