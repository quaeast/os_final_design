//
// Created by ZIdongFang on 2019-06-02.
//

#ifndef OS_FINAL_DESIGN_DIR_H
#define OS_FINAL_DESIGN_DIR_H


//1024B
struct DIR{
    int inode_pointer[256];
};

typedef struct DIR DIR;

#endif //OS_FINAL_DESIGN_DIR_H
