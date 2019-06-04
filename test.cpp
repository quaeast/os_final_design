//
// Created by ZIdongFang on 2019-06-02.
//

#include <stdio.h>
//#include <memory.h>
//#include <string.h>
#include "file_system.h"


int main(){
    init_disk();
    int root = init_file_sys();
    FILE *fstream = fopen("/tmp/os_final/fake_disk", "rb+");
    int cur = make_file("fang", root, fstream);
    cur = make_file("wang", cur, fstream);
    int n = change_directory(104, "..", fstream);
    printf("%d", n);
    return 0;
}

