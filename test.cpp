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
    INode i_node_buffer;
    get_INode(&i_node_buffer, liu_add, fstream);
    i_node_buffer.index_list[2]=1000;
    write_INode(&i_node_buffer, liu_add, fstream);

    INode i_node_buffer2;
    get_INode(&i_node_buffer2, liu_add, fstream);
    printf("%d", i_node_buffer2.index_list[2]);

    return 0;
}

