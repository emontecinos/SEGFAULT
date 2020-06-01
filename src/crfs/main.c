#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../cr_API/cr_API.h"

extern char* PATH;
int main(int argument_count, char** arguments){
    // Modo de uso 
    // ./crfs simdiskfilled.bin
    cr_mount(arguments[1]);
    //printf("%s\n",PATH);
    //cr_load(4,"lala.txt");
    
    //cr_unload(1,"Program in C.mkv","/mnt/c/Users/felipe/Desktop/Archivos");
    // cr_unload(4,"lala.txt","/mnt/c/Users/felipe/desktop/Archivos");
    // int j=15;
    // j=cr_exists(4,"lala.txt");
    // printf("Existe?: %d\n",j);
    cr_ls(1);
    //cr_unload(1,"text.txt","");

    //cr_bitmap(4,0);

    // long int bloque_libre=-1;
    // bloque_libre=get_free_block(198000);
    // printf("\n%li\n",bloque_libre);
    // Endianness
    // int x = 1;

    // char *y = (char*)&x;

    // printf("%c\n",*y+48);

    // cr_exists(ROUTE, char* filename);

    // cr_ls(ROUTE);

    // cr_open(ROUTE, char* filename, char mode);

    // cr_read (crFILE* file_desc, void* buffer, int nbytes);

    // cr_write(crFILE* file_desc, void* buffer, int nbytes);

    // cr_close(crFILE* file_desc);

    // cr_rm(ROUTE, char* filename);

    // cr_hardlink(ROUTE, char* orig, char* dest);

    // cr_softlink(unsigned disk_orig, unsigned disk_dest, char* orig, char* dest);

    // cr_unload(ROUTE, char* orig, char* dest);

    // cr_load(ROUTE, char* orig);

    

    return 1;
}