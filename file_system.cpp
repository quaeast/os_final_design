//
// Created by ZIdongFang on 2019-06-02.
//

#include "file_system.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "config.h"
#include "inode.h"
#include "list_print_buffer.h"


int get_INode(INode *i_node_buffer, unsigned int i_node_address, FILE *fstream){
    fseek(fstream, I_NODE_OFFSET+i_node_address, SEEK_SET);
    fread(i_node_buffer, I_NODE_SIZE, 1, fstream);
    return 0;
}

int get_data(int *buffer, unsigned int data_address, FILE *fstream){
    fseek(fstream, DATA_OFFSET+data_address, SEEK_SET);
    fread(buffer, sizeof(int), 256, fstream);
}

int get_father_INode_address(unsigned int current_address, FILE *fstream){
    INode i_node_buffer;
    get_INode(&i_node_buffer, current_address, fstream);
    int *first_data = (int *)calloc(256, sizeof(int));
    get_data(first_data, i_node_buffer.index_list[0], fstream);
    int result = first_data[0];
    free(first_data);
    return result;
}

//parameter size is the number of byte
//
//LSB >>
//LSB first
//
//return the nth block but real address,
//so it need to multiply BLOCK_SIZE or I_NODE_SIZE before use
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


//
int free_block(unsigned char *buffer, int address){
    int buffer_addr = address/8;
    int char_addr = address%8;
    unsigned char mask = 1<<char_addr;
    buffer[buffer_addr]^=mask;
}


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



/*
 * format:
 *      l:
 *      others
 *
 * information:
 *      s: size
 *      t: time
 *
 * order:
 *      S: size order
 *      d: default
 *
 */

int list_descendants(unsigned int father_i_node_address, char *mode, char *result, FILE *fstream){
    INode father_buffer;
    fseek(fstream, I_NODE_OFFSET+father_i_node_address, SEEK_SET);
    fread(&father_buffer, I_NODE_SIZE, 1, fstream);
    unsigned int descendant_array_address = father_buffer.index_list[1];
    unsigned int *descendant_array_buffer = (unsigned int *)calloc(256, sizeof(int));
    fseek(fstream, DATA_OFFSET+descendant_array_address, SEEK_SET);
    fread(descendant_array_buffer, sizeof(int), 256, fstream);
    ListPrintBuffer *listPrintBuffer = (ListPrintBuffer *)calloc(256, sizeof(ListPrintBuffer));
    int list_num=0;
    for (int i = 0; i < 256; ++i) {
        if(descendant_array_buffer[i]!=-1){
            int cur_i_node_addr=descendant_array_buffer[i];
            fseek(fstream, I_NODE_OFFSET+cur_i_node_addr, SEEK_SET);
            INode descendent_buffer;
            fread(&descendent_buffer, I_NODE_SIZE, 1, fstream);
            strcpy(listPrintBuffer[list_num].name, descendent_buffer.name);
            /*
             * 还有其他功能没有写
             */
            list_num++;
        }
    }
    int mo_index=5;
    if(strchr(mode, 'l')){
        mo_index = 1;
    }


    char *result_pointer=result;
    for (int i = 0; i < list_num; ++i) {
        sprintf(result_pointer ,"%s\t", listPrintBuffer[i].name);
        result_pointer+=strlen(result_pointer);

        if(i%mo_index==0){
            sprintf(result_pointer, "\n");
            result_pointer+=strlen(result_pointer);
        }
    }

    free(descendant_array_buffer);
    free(listPrintBuffer);
}

int make_file(char* name, unsigned int father_address, char file_type, FILE *fstream){
    char list_result[100];
    list_descendants(father_address, "l", list_result, fstream);
    if(strstr(list_result, name)){
        return -1;
    }

    unsigned char i_node_bitmap[32];
    unsigned char data_bitmap[64*K];
    fseek(fstream, 0, SEEK_SET);
    fread(i_node_bitmap, 1, 32, fstream);
    fseek(fstream, DATA_BITMAP_OFFSET, SEEK_SET);
    fread(data_bitmap, 1, 64*K, fstream);

    /*
     * creat new i node
     * and write to the file
     */
    INode new_dir;
    new_dir.file_type = file_type;
    strcpy(new_dir.name, name);
    for (int i = 0; i < 12; ++i) {
        new_dir.index_list[i] = -1;  //py: 2**32=4294967296 max of unsigned int
    }
    unsigned int empty_i_node_address = I_NODE_SIZE * find_an_empty_block(i_node_bitmap, sizeof(i_node_bitmap));
    unsigned int empty_data_address = FFS_BLOCK_SIZE * find_an_empty_block(data_bitmap, sizeof(data_bitmap));
    unsigned int another_empty_data_address = FFS_BLOCK_SIZE * find_an_empty_block(data_bitmap, sizeof(data_bitmap));
    new_dir.index_list[0]=empty_data_address;
    new_dir.index_list[1]=another_empty_data_address;

    // write new i node
    fseek(fstream, I_NODE_OFFSET+empty_i_node_address, SEEK_SET);
    fwrite(&new_dir, I_NODE_SIZE, 1, fstream);

    // write father and self to data
    unsigned int address[2];
    unsigned int *descendants=(unsigned int*)calloc(256, sizeof(int));
    for (int i = 0; i < 256; ++i) {
        descendants[i]=-1;
    }
    address[0]=father_address;
    address[1]=empty_i_node_address;

    fseek(fstream, DATA_OFFSET+empty_data_address, SEEK_SET);
    fwrite(address, sizeof(unsigned int), 2, fstream);

    fseek(fstream, DATA_OFFSET+another_empty_data_address, SEEK_SET);
    fwrite(descendants, sizeof(unsigned int), 256, fstream);
    free(descendants);


    /*
     * change father i_node
     */

    if(father_address!=-1){
        INode father_i_node_buffer;
        fseek(fstream, I_NODE_OFFSET+father_address, SEEK_SET);
        fread(&father_i_node_buffer, I_NODE_SIZE, 1, fstream);
        unsigned int descendant_array_address=father_i_node_buffer.index_list[1];
        unsigned int *descendant_array = (unsigned int*)calloc(256, sizeof(int));
        fseek(fstream, DATA_OFFSET+descendant_array_address, SEEK_SET);
        fread(descendant_array, sizeof(int), 256, fstream);

        int flag_in_descendant_array=-1;
        for (int i=0; i < 256; ++i) {
            if(descendant_array[i]==-1){
                flag_in_descendant_array=i;
                break;
            }
        }
        if(flag_in_descendant_array==-1){
            return -1;
        }
        descendant_array[flag_in_descendant_array]=empty_i_node_address;
        fseek(fstream, DATA_OFFSET+descendant_array_address, SEEK_SET);
        fwrite(descendant_array, sizeof(int), 256, fstream);
        free(descendant_array);
    }

    fseek(fstream, 0, SEEK_SET);
    fwrite(i_node_bitmap, 1, 32, fstream);
    fseek(fstream, DATA_BITMAP_OFFSET, SEEK_SET);
    fwrite(data_bitmap, 1, 64*K, fstream);
    return empty_i_node_address;
}



//root i node('/') address is 0
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
    int root_i_node_address = make_file(root_name, -1, 'd', fstream);
    char name[48];
    fclose(fstream);
    return root_i_node_address;
}


// error: -1
// return i node address
int find_descendant_address_with_name(unsigned int father_i_node_address, char *target, FILE *fstream){
    INode father_i_node_buffer;
    fseek(fstream, I_NODE_OFFSET+father_i_node_address, SEEK_SET);
    fread(&father_i_node_buffer, I_NODE_SIZE, 1, fstream);

    unsigned int *descendent_address_array = (unsigned int *)calloc(256, sizeof(int));
    unsigned descendent_address_array_address = father_i_node_buffer.index_list[1];
    fseek(fstream, DATA_OFFSET+descendent_address_array_address, SEEK_SET);
    fread(descendent_address_array, sizeof(unsigned int), 256, fstream);
    for (int i = 0; i < 256; ++i) {
        if(descendent_address_array[i]!=-1){
            INode descendent_buffer;
            get_INode(&descendent_buffer, descendent_address_array[i], fstream);
            if(strcmp(target, descendent_buffer.name)==0){
                return descendent_address_array[i];
            }
        }
    }
    free(descendent_address_array);
    return -1;
}

// return i node address
int change_directory(unsigned int current_i_node_address, char *destination, FILE *fstream){
    if(strcmp(destination, ".")==0){
        return current_i_node_address;
    }
    if(strcmp(destination, "..")==0){
        return get_father_INode_address(current_i_node_address, fstream);

    }

    INode i_node_buffer;
    get_INode(&i_node_buffer, current_i_node_address, fstream);
    unsigned int loop_current_address = current_i_node_address;
    if(destination[0]=='/'){
        loop_current_address = 0;
    }
    char *loop_descendant_name_buffer;
    char *strtok_pointer;
    char destination_buffer[100];
    strcpy(destination_buffer, destination);
    loop_descendant_name_buffer = strtok_r(destination_buffer, "/", &strtok_pointer);
    while (loop_descendant_name_buffer){
        if(loop_current_address==-1){
            return -1;
        }
        loop_current_address = find_descendant_address_with_name(loop_current_address, loop_descendant_name_buffer, fstream);
        loop_descendant_name_buffer = strtok_r(NULL, "/", &strtok_pointer);
    }
    return loop_current_address;
}

void strrev(char *str){
    unsigned long len = strlen(str);
    for (int i = 0; i < len/2; ++i) {
        char buf = str[i];
        str[i] = str[len-1-i];
        str[len-1-i] = buf;
    }
}

int print_work_directory(unsigned int i_node_address, char *result, FILE *fstream){
    unsigned int cur_i_node_address = i_node_address;
    char *str_pointer=result;
    while (cur_i_node_address!=-1){
        INode i_node_buffer;
        get_INode(&i_node_buffer, cur_i_node_address, fstream);
        sprintf(str_pointer, "%s/", i_node_buffer.name);
        strrev(str_pointer);
        str_pointer+=strlen(str_pointer);
        cur_i_node_address = get_father_INode_address(cur_i_node_address, fstream);
    }
    result[strlen(result)-1] = 0;
    strrev(result);
    result[strlen(result)-1] = 0;
    return 0;
}



