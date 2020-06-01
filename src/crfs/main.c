
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
  //  cr_ls(2);
    // cr_exists(ROUTE, char* filename);
    // char qq[3];
    // qq[0] = 's';
    // qq[1] = 2;
    // qq[2] = 3;
    //
    // fseek(ptr, 7*32, SEEK_SET);
    // for (int g = 0; g < 3; g++)
    // {
    //   fwrite(qq[g], sizeof(qq[g]), 1, ptr);
    // }



    cr_open(1, "aaa.txt", "w");

    printf("-------------\n");
    cr_ls(1);

    // cr_read (crFILE* file_desc, void* buffer, int nbytes);

    // cr_write(crFILE* file_desc, void* buffer, int nbytes);

    // cr_close(crFILE* file_desc);

    // cr_rm(ROUTE, char* filename);

    //cr_hardlink(1, "Baroque", "dest4.mp3");

    //cr_softlink(1, 4, "guides.txt", "guides.txt");


    // cr_unload(ROUTE, char* orig, char* dest);

    // cr_load(ROUTE, char* orig);



    return 1;
}
