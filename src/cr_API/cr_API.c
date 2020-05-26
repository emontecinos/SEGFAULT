#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h> 
#include <fcntl.h>
#include "cr_API.h"

extern char* PATH;

void cr_mount(char* diskname){
    PATH = diskname;
    return;
}
void cr_bitmap(unsigned disk, bool hex){
    int fd= 2; // STRERR
    char buff[65536];
    off_t bit_a_revisar=0;
    int bytes_read;
    size_t size_block=65536;
    size_t leidos;
    FILE* file = fopen("simdiskfilled.bin","rb");
    int c,i;
    bit_a_revisar= disk*536870912 + 8192;
    fseek(file,bit_a_revisar,SEEK_SET);
    // fread(buff, 1, 1, file);
    for (i=0; i < 16 && (c=getc(file))!=EOF; i++){
        fprintf(stderr,"%x",c);
    }
    printf("\ni %d\n",i);
    fseek(file,bit_a_revisar,SEEK_SET);
    for (i=0; i < 16 && (c=getc(file))!=EOF; i++){
        fprintf(stderr,"%02X",c&0xff);
        fprintf(stderr," ");
        
        
    }
    printf("\ni %d\n",i);

    
    
    // printf("Marc1\n");
    // fflush(stdout);
    // fread(buff,size_block,1,file);
    // printf("Marc2\n");
    // fflush(stdout);
    // fprintf(stderr,"%x\n",buff);
    fclose(file);

    return;
}
int cr_exists(unsigned disk, char* filename){
    return 1;
}
void cr_ls(unsigned disk){
    return;
}
crFILE* cr_open(unsigned disk, char* filename, char mode){
    crFILE* crfile=malloc(sizeof(crFILE));
    crfile ->size=1;
    return crfile;
}
int cr_read (crFILE* file_desc, void* buffer, int nbytes){
    return 1;
}
int cr_write(crFILE* file_desc, void* buffer, int nbytes){
    return 1;
}
int cr_close(crFILE* file_desc){
    free(file_desc);
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
