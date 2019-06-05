//
// Created by ZIdongFang on 2019-06-02.
//

#ifndef OS_FINAL_DESIGN_FILE_SYSTEM_H
#define OS_FINAL_DESIGN_FILE_SYSTEM_H

#include <stdio.h>
#include "inode.h"


int get_INode(INode *i_node_buffer, unsigned int i_node_address, FILE *fstream);

int write_INode(INode *i_node_buffer, unsigned int i_node_address, FILE *fstream);

int get_data(int *buffer, unsigned int data_address, FILE *fstream);

int write_data(int *buffer, unsigned int data_address, FILE *fstream);

int get_father_INode_address(unsigned int current_address, FILE *fstream);


int find_an_empty_block(unsigned char *buffer, int size);

int free_block(unsigned char *buffer, int address);

//size ->MB
//default 513MB
int init_disk();

int init_file_sys();

int list_descendants(unsigned int father_i_node_address, char *mode, char *result, FILE *fstream);

int make_file(char* name, unsigned int father_address, char file_type, FILE *fstream);

int change_directory(unsigned int current_i_node_address, char *destination, FILE *fstream);

int print_work_directory(unsigned int i_node_address, char *result, FILE *fstream);

int find_descendant_address_with_name(unsigned int father_i_node_address, char *target, FILE *fstream);

int copy(unsigned int father_i_node_address, char *source_name, char *target_name, FILE *fstream);

int rename(unsigned int cur, char *old_name, char *new_name, FILE *fstream);

int delete_file(unsigned int cur, char * target, FILE *fstream);

int write_text(char *data, unsigned int i_node_address, FILE *fstream);

int catch_file(char *data, unsigned int i_node_address, FILE *fstream);

#endif //OS_FINAL_DESIGN_FILE_SYSTEM_H

