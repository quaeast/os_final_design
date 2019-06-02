//
// Created by ZIdongFang on 2019-06-02.
//

#ifndef OS_FINAL_DESIGN_CONFIG_H
#define OS_FINAL_DESIGN_CONFIG_H

const char* C_to_D_name = "/tmp/C_to_D_fifo";
const char* D_to_C_name = "/tmp/D_to_C_fifo";


//1024B->1KB every small BLOCK
const int FFH_BLOCK_SIZE = 1024;
const int I_NODE_SIZE = 128;

//
const int K = 1024;
const int M = 1048576;//1024*1024;



#endif //OS_FINAL_DESIGN_CONFIG_H
