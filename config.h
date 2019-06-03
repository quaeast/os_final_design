//
// Created by ZIdongFang on 2019-06-02.
//

#ifndef OS_FINAL_DESIGN_CONFIG_H
#define OS_FINAL_DESIGN_CONFIG_H


//
static const int K = 1024;
static const int M = 1048576;//1024*1024;

static const char* C_to_D_name = "/tmp/os_final/C_to_D_fifo";
static const char* D_to_C_name = "/tmp/os_final/D_to_C_fifo";

//1024B->1KB every small BLOCK
static const int FFS_BLOCK_SIZE = 1024;
static const int I_NODE_SIZE = 100;
static const int DISK_SIZE = 513 * M;

//
static const char* DISK_NAME = "/tmp/os_final/fake_disk";

#endif //OS_FINAL_DESIGN_CONFIG_H
