#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "cr_API.h"

extern char* PATH;

void cr_mount(char* diskname){
    PATH = diskname;
    return;
}
void cr_bitmap(unsigned disk, bool hex){
    return;
}
int cr_exists(unsigned disk, char* filename){
    return 1;
}
void cr_ls(unsigned disk){
    return;
}
// crFILE* cr_open(unsigned disk, char* filename, char mode){
//     crFILE* crfile=malloc(sizeof(crFILE));
//     crfile ->size=1;
//     return crfile;
// }
int cr_read (crFILE* file_desc, void* buffer, int nbytes){
    return 1;
}
int cr_write(crFILE* file_desc, void* buffer, int nbytes){
    return 1;
}
int cr_close(crFILE* file_desc){
    return 1;
}
int cr_rm(unsigned disk, char* filename){
    return 1;
}
int cr_hardlink(unsigned disk, char* orig, char* dest){
    return 1;
}
int cr_softlink(unsigned disk_orig, unsigned disk_dest, char* orig, char* dest){
    return 1;
}
int cr_unload(unsigned disk, char* orig, char* dest){
    return 1;
}
int cr_load(unsigned disk, char* orig){
    return 1;
}
