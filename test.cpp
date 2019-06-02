//
// Created by ZIdongFang on 2019-06-02.
//

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>

#include "file_system.h"
#include "inode.h"
#include "dir.h"

using namespace std;


int main(){
    DIR dir;
//    dir.inode_pointer[0]=111;
    FILE *f;
    f=fopen("/tmp/test_data", "rb+");
    fread(&dir, sizeof(dir), sizeof(dir), f);
    printf("%d\n", dir.inode_pointer[0]);
    fclose(f);
    return 0;
}

