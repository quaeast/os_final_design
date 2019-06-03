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

using namespace std;

int main(){
    init_disk();
    int root = init_file_sys();
    int cur=root;

    return 0;
}

