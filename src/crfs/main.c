#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../cr_API/cr_API.h"

extern char* PATH;
int main(int argument_count, char** arguments){
    // Modo de uso
    // ./crfs simdiskfilled.bin
    // printf("%s\n",ROUTE);

    cr_mount(arguments[1]);


    /*
    cr_bitmap(1, 1);
    cr_bitmap(2, 1);
    cr_bitmap(3, 1);
    cr_bitmap(4, 1);
    */

    // cr_exists(ROUTE, char* filename);

    // cr_ls(ROUTE);

    // cr_open(ROUTE, char* filename, char mode);

    // cr_read (crFILE* file_desc, void* buffer, int nbytes);

    // cr_write(crFILE* file_desc, void* buffer, int nbytes);

    // cr_close(crFILE* file_desc);

    //cr_bitmap(1, 1);
    cr_rm(1, "lala.txt");
    //cr_ls(1);
    //cr_bitmap(1, 1);


    //cr_hardlink(2, "yes.mp3", "destino.mp3");


    //cr_softlink(2, 4, "yes.mp3", "yes.mp3");

    //cr_load(1, "lala.txt");

    cr_ls(1);

    //cr_unload(1, "lala.txt", "/mnt/c/Users/reyza/Desktop/");





    return 1;
}
