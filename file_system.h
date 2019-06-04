//
// Created by ZIdongFang on 2019-06-02.
//

#ifndef OS_FINAL_DESIGN_FILE_SYSTEM_H
#define OS_FINAL_DESIGN_FILE_SYSTEM_H

#include <stdio.h>
#include "inode.h"


int get_INode(INode *i_node_buffer, unsigned int i_node_address, FILE *fstream);

int get_data(int *buffer, unsigned int data_address, FILE *fstream);

int get_father_INode_address(unsigned int current_address, FILE *fstream);


int find_an_empty_block(unsigned char *buffer, int size);

//size ->MB
//default 513MB
int init_disk();

int init_file_sys();

int list_descendants(unsigned int father_i_node_address, char *mode, char *result, FILE *fstream);

int make_directory(char* name, unsigned int father_address, FILE *fstream);

int print_work_place(unsigned int i_node_address, char *result, FILE *fstream);


int find_descendant_address_with_name(unsigned int father_i_node_address, char *target, FILE *fstream);


#endif //OS_FINAL_DESIGN_FILE_SYSTEM_H

