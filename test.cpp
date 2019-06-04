//
// Created by ZIdongFang on 2019-06-02.
//

#include <iostream>
#include <stdio.h>

#include "file_system.h"
using namespace std;

//int argc, char * argv[]

int main(){
    init_disk();
    int root = init_file_sys();
    FILE *fstream = fopen("/tmp/os_final/fake_disk", "rb+");
    int cur = make_directory("fang", root, fstream);
    cur = make_directory("wang", cur, fstream);
    int fa = get_father_INode_address(cur, fstream);
    char res[48];
    print_work_place(cur, res, fstream);
    return 0;
}

