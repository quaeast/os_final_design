//
// Created by ZIdongFang on 2019-06-02.
//

#include <stdio.h>
//#include <memory.h>
//#include <string.h>
#include "mystring.h"
#include "file_system.h"

//FILE *fstream = fopen("/tmp/os_final/fake_disk", "rb+");


int main(){
    init_disk();
    int root = init_file_sys();
    FILE *fstream = fopen("/tmp/os_final/fake_disk", "rb+");
    make_file("fang", root, 'd', fstream);
    unsigned int liu_add = make_file("liu.txt", root, 'f', fstream);

    INode inode_buffer;

    get_INode(&inode_buffer, liu_add, fstream);
    printf("%d\n", inode_buffer.index_list[2]);

    return 0;
}

