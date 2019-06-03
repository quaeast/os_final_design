//
// Created by ZIdongFang on 2019-06-02.
//

#ifndef OS_FINAL_DESIGN_FILE_SYSTEM_H
#define OS_FINAL_DESIGN_FILE_SYSTEM_H

#include <stdio.h>

//size ->MB
//default 1GB
int init_disk();

int init_file_sys();

int find_an_empty_block(unsigned char *buffer, int size);

int print_work_place(unsigned int i_node_address, FILE *fstream, char *name);

int make_directory(char* name, unsigned int father_address, FILE *fstream);

#endif //OS_FINAL_DESIGN_FILE_SYSTEM_H

