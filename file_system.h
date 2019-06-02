//
// Created by ZIdongFang on 2019-06-02.
//

#ifndef OS_FINAL_DESIGN_FILE_SYSTEM_H
#define OS_FINAL_DESIGN_FILE_SYSTEM_H


//size ->MB
//default 1GB
int init_disk(char *disk_name="/tmp/fake_disc", int size=1024);

int init_file_sys(char *disk_name="/tmp/fake_disc", int size=1024);


#endif //OS_FINAL_DESIGN_FILE_SYSTEM_H
