//
// Created by ZIdongFang on 2019-06-02.
//

#include "file_system.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#include "config.h"
#include "inode.h"

//size MB
int init_disk(){
    FILE *fstream;
    fstream = fopen(DISK_NAME, "wb");
    if(fstream==NULL){
        return -1;
    }
    fseek(fstream, DISK_SIZE, SEEK_SET);
    fwrite("1", 1, 1, fstream);
    fclose(fstream);
    return 0;
}

//size is the number of byte
//
//LSB >>
//LSB first
int find_an_empty_block(unsigned char *buffer, int size){
    for (int i = 0; i < size; ++i) {
        unsigned char short_buffer=buffer[i];
        unsigned char mask = 1;
        for (unsigned char j = 0; j < 8; mask<<=(unsigned)1, j++) {
            if((mask | short_buffer)!=short_buffer){
                buffer[i]^=mask;
                return i*8+j;
            }
        }
    }
    return -1;
}


int make_directory(char* name, unsigned int father_address, FILE *fstream){
    unsigned char i_node_bitmap[32];
    unsigned char data_bitmap[64*K];
    fseek(fstream, 0, SEEK_SET);
    fread(i_node_bitmap, 1, 32, fstream);
    fseek(fstream, K, SEEK_SET);
    fread(data_bitmap, 1, 64*K, fstream);

    /*
     * creat new i node
     * and write to the file
     */
    I_NODE new_dir;
    new_dir.file_type='d';
    strcpy(new_dir.name, name);
    for (int i = 0; i < 12; ++i) {
        new_dir.index_list[i]=-1;  //py: 2**32=4294967296 max of unsigned int
    }
    unsigned int empty_i_node_address = find_an_empty_block(i_node_bitmap, sizeof(i_node_bitmap));
    unsigned int empty_data_address = find_an_empty_block(data_bitmap, sizeof(data_bitmap));
    unsigned int another_empty_data_address = find_an_empty_block(data_bitmap, sizeof(data_bitmap));
    new_dir.index_list[0]=empty_data_address;
    new_dir.index_list[1]=another_empty_data_address;

    // write new i node
    fseek(fstream, 100*K+empty_i_node_address, SEEK_SET);
    fwrite(&new_dir, I_NODE_SIZE, 1, fstream);

    // write father and self to data
    unsigned int address[2];
    unsigned int *descendants=(unsigned int*)calloc(256, 4);
    for (int i = 0; i < 256; ++i) {
        descendants[i]=-1;
    }
    address[0]=father_address;
    address[1]=empty_i_node_address;

    fseek(fstream, M+empty_data_address, SEEK_SET);
    fwrite(address, sizeof(unsigned int), 2, fstream);

    fseek(fstream, M+another_empty_data_address, SEEK_SET);
    fwrite(descendants, sizeof(unsigned int), 256, fstream);
    free(descendants);


    /*
     * change father i_node
     */

    if(father_address!=-1){
        I_NODE father_i_node_buffer;
        fread(&father_i_node_buffer, I_NODE_SIZE, 1, fstream);

        int i=-1;
        for (; i < 256; ++i) {
            
        }
        if(i==-1){
            return -1;
        }
        fwrite(&father_i_node_buffer, I_NODE_SIZE, 1, fstream);
    }

    fseek(fstream, 0, SEEK_SET);
    fwrite(i_node_bitmap, 1, 32, fstream);
    fseek(fstream, K, SEEK_SET);
    fwrite(data_bitmap, 1, 64*K, fstream);
    return empty_i_node_address;
}

int print_work_place(unsigned int i_node_address, FILE *fstream, char *name){
    fseek(fstream, 100*K+i_node_address, SEEK_SET);
    I_NODE i_node_buffer;
    fread(&i_node_buffer, I_NODE_SIZE, 1, fstream);
    strcpy(name, i_node_buffer.name);
    return 0;
}

int init_file_sys(){
    if(access(DISK_NAME, F_OK)!=0){
        return -1;
    }
    FILE *fstream;
    unsigned char *buffer=(unsigned char *)calloc(65*K, 1);
    memset(buffer, 0, 65*K);
    fstream = fopen(DISK_NAME, "rb+");
    fwrite(buffer, 1, 65*K, fstream);   //i_node bit map and data bit map
    free(buffer);

    char root_name[48]="/";
    int root_i_node_address = make_directory(root_name, -1, fstream);
    char name[48];
    fclose(fstream);
    return root_i_node_address;
}

