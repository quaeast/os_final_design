#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>

#include <iostream>

#include "config.h"
#include "file_system.h"



//read the fifo
//daemon
int main()
{
    int C_to_D_id, D_to_C_id, file_id;
    char buffer[PIPE_BUF+1];            //4096+1

    mkfifo(C_to_D_name, 0777);
    mkfifo(D_to_C_name, 0777);

    if(access(C_to_D_name, F_OK)==-1 || access(D_to_C_name, F_OK)==-1){
        printf("can't creat named fifo.\n");
        return 1;
    } else{
        printf("daemon started.\n");
    }

    file_id = ("data.txt", O_RDWR);
    C_to_D_id = open(C_to_D_name, O_RDONLY);
    D_to_C_id = open(D_to_C_name, O_WRONLY);
    unsigned int root_address= -1;
    unsigned int cur=-1;
    FILE *fstream;

    while (1){
        read(C_to_D_id, buffer, PIPE_BUF);
        //

        printf(">-----------------------------------------\n");
        printf("command: %s\nsplit: \n", buffer);

        //
        char *main_cmd= strtok(buffer, " ");
        char *sub_cmd = strtok(NULL, " ");
        char *target = strtok(NULL, " ");
        //

        printf("%s\t%s\t%s\n", main_cmd, sub_cmd, target);

        //

        if(strcmp(main_cmd, "exit")==0){
            sprintf(buffer, "%s", "good bye\n");
            write(D_to_C_id, buffer, PIPE_BUF);
            break;
        }
        else if(strcmp(main_cmd, "init")==0){
            init_disk();
            root_address = init_file_sys();
            fstream = fopen(DISK_NAME, "rb+");
            cur = root_address;
            sprintf(buffer, "%s", "init succeed\n");
            write(D_to_C_id, buffer, PIPE_BUF);
        }
        else if(strcmp(main_cmd, "open")==0){
            fstream = fopen(DISK_NAME, "rb+");
            root_address = 0;
            cur = root_address;
            sprintf(buffer, "%s%s", DISK_NAME, " open succeed\n");
            write(D_to_C_id, buffer, PIPE_BUF);
        }
        else if (strcmp(main_cmd, "ls")==0){
            buffer[0]=0;
            list_descendants(cur, "l", buffer, fstream);
            write(D_to_C_id, buffer, PIPE_BUF);
        }
        else if (strcmp(main_cmd, "pwd")==0){
            print_work_directory(cur, buffer, fstream);
            buffer[strlen(buffer)] = '\n';
            write(D_to_C_id, buffer, PIPE_BUF);
        }
        else if(strcmp(main_cmd, "cd")==0){
            cur = change_directory(cur, sub_cmd, fstream);
            buffer[0]=0;
            write(D_to_C_id, buffer, PIPE_BUF);
        }
        else if(strcmp(main_cmd, "touch")==0){
            make_file(sub_cmd, cur, 'f', fstream);
            buffer[0]=0;
            write(D_to_C_id, buffer, PIPE_BUF);
        }
        else if(strcmp(main_cmd, "mkdir")==0){
            make_file(sub_cmd, cur, 'd', fstream);
            buffer[0]=0;
            printf("%s %s\n", main_cmd, sub_cmd);
            write(D_to_C_id, buffer, PIPE_BUF);
        }
        else if(strcmp(main_cmd, "cp")==0){
            copy(cur, sub_cmd, target, fstream);
            buffer[0]=0;
            write(D_to_C_id, buffer, PIPE_BUF);
        }
        else if (strcmp(main_cmd, "rm")==0){
            delete_file(cur, sub_cmd, fstream);
            buffer[0]=0;
            write(D_to_C_id, buffer, PIPE_BUF);
        }
        else if (strcmp(main_cmd, "rename")==0){
            rename(cur, sub_cmd, target, fstream);
            buffer[0]=0;
            write(D_to_C_id, buffer, PIPE_BUF);
        }
        else if (strcmp(main_cmd, "vim")==0){
            char *vim_buffer = (char *)calloc(PIPE_BUF, 1);
            strcpy(vim_buffer, "vim");
            write(D_to_C_id, vim_buffer, PIPE_BUF);
            read(C_to_D_id, vim_buffer, PIPE_BUF);
            printf("%s\t", sub_cmd);
            int target_address = find_descendant_address_with_name(cur, sub_cmd, fstream);
            if(target_address==-1){
                printf("can not find target!\n");
            }
            write_text(vim_buffer, target_address, fstream);
            strcpy(vim_buffer, "vim exit\n");
            write(D_to_C_id, vim_buffer, PIPE_BUF);
            printf(vim_buffer);
        }
        else if(strcmp(main_cmd, "cat")==0){
            int target_address = find_descendant_address_with_name(cur, sub_cmd, fstream);
            if(target_address==-1){
                printf("can not find target!\n");
            }
            catch_file(buffer, target_address, fstream);
            write(D_to_C_id, buffer, PIPE_BUF);
        }
        else if(strcmp(main_cmd, "export")==0){
            char real_target[100] = "/tmp/export";
            if(target!=NULL){
                strcpy(real_target, target);
            }
            int target_address = find_descendant_address_with_name(cur, sub_cmd, fstream);
            if(target_address==-1){
                printf("can not find target!\n");
            }
            catch_file(buffer, target_address, fstream);
            FILE *fp = fopen(real_target, "w+");
            printf(buffer);
            fwrite(buffer, 4, strlen(buffer), fp);
            fclose(fp);
            write(D_to_C_id, buffer, PIPE_BUF);
        }
        else if(strcmp(main_cmd, "import")==0){
            import(sub_cmd, target, cur, fstream);
            buffer[0]=0;
            write(D_to_C_id, buffer, PIPE_BUF);
        }
        else if(strcmp(main_cmd, "clear")==0){
            write(D_to_C_id, buffer, PIPE_BUF);
        }
        else{
            sprintf(buffer, "%s%s", main_cmd, ": command not found.\n");
            write(D_to_C_id, buffer, PIPE_BUF);
        }
        printf("result:\n");
        printf(buffer);
        buffer[0]=0;
    }

    //exit
    close(file_id);
    close(C_to_D_id);
    close(D_to_C_id);
//    unlink(C_to_D_name);
//    unlink(D_to_C_name);
    return 0;
}


