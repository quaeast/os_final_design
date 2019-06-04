//
// Created by ZIdongFang on 2019-06-02.
//

#include <stdio.h>
//#include <memory.h>
//#include <string.h>
#include "file_system.h"

//FILE *fstream = fopen("/tmp/os_final/fake_disk", "rb+");


int main(){
    init_disk();
    int root = init_file_sys();
    FILE *fstream = fopen("/tmp/os_final/fake_disk", "rb+");
    int cur = make_file("fang", root, 'd', fstream);

    copy(root, "fang", "wang", fstream);

    char result[100];
    list_descendants(root, "l", result, fstream);
    printf(result);
    return 0;
}

