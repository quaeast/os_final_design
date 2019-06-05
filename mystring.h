//
// Created by ZIdongFang on 2019-06-05.
//

#ifndef OS_FINAL_DESIGN_MYSTRING_H
#define OS_FINAL_DESIGN_MYSTRING_H

#include <iostream>
#include <string.h>

using namespace std;

int vim(char *data){
    char *pointer = data;
    while (1){
        cin.getline(pointer, 100);
        if(strcmp(pointer, ":wq")==0){
            break;
        }
        pointer+=strlen(pointer);
        *pointer='\n';
        pointer++;
        *pointer=0;
    }
    pointer = data + strlen(data) - strlen(pointer);
    *pointer = 0;
    return 0;
}

#endif //OS_FINAL_DESIGN_MYSTRING_H
