//
// Created by ZIdongFang on 2019-06-02.
//

#ifndef OS_FINAL_DESIGN_INODE_H
#define OS_FINAL_DESIGN_INODE_H

//use every int(4*8-1 = 31bit -> 2GB) represent an address in data segment
//negative means blank

//100B
struct I_NODE{
    char name[48];
    // index 12k
    int index_list[12];
    //d -> directory
    //f -> file
    char file_tpye;
};

typedef struct I_NODE I_NODE;

#endif //OS_FINAL_DESIGN_INODE_H
