//
// Created by ZIdongFang on 2019-06-02.
//

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "file_system.h"

using namespace std;

int main(){
    init_disk();
    int root = init_file_sys();
//    int root=0;
    int cur=root;
    FILE *fstream = fopen("/tmp/os_final/fake_disk", "rb+");
    char result[100];
    print_work_place(cur, result, fstream);
    printf("%s");
    list_descendants(cur, "l", result, fstream);
    printf("%s", result);
    fclose(fstream);
    return 0;
}

