#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../cr_API/cr_API.h"

// #include <limits.h> // for CHAR_BIT

extern char* PATH;
int main(int argument_count, char** arguments){

    // Modo de uso
    // ./crfs simdisk.bin

    // printf("%s\n",ROUTE);

    cr_mount(arguments[1]);
    printf("%s\n",PATH);


    // cr_ls(0);
     cr_ls(1);
    //cr_ls(2);
    // cr_ls(3);

    cr_bitmap(1, 1);

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


    // int existe;
    // existe = cr_exists(2, "yes.mp3");
    // printf("existe: %i\n", existe);

    return 1;
}
