//
// Created by ZIdongFang on 2019-06-03.
//

#ifndef OS_FINAL_DESIGN_LIST_PRINT_BUFFER_H
#define OS_FINAL_DESIGN_LIST_PRINT_BUFFER_H

struct ListPrintBuffer{
    char name[48];
    char time[20]; // 2019-06-03-4:12:45
    char type;      //f/d
    int size=-1;
};

typedef struct ListPrintBuffer ListPrintBuffer;

#endif //OS_FINAL_DESIGN_LIST_PRINT_BUFFER_H
