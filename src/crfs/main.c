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


    // crFILE* archivo;
    // archivo = cr_open(1, "guides2.txt", "w");

    // printf("-------------\n");
    //cr_ls(1);

    int aaa[20];

    unsigned char buff[300];
    //unsigned char * buff = ( unsigned char * )malloc( 20 * sizeof( unsigned char ) );
    int aaaaaa;
    aaaaaa = cr_read(archivo, buff, 300);
    //printf("aaaa %i\n", aaaaaa);
    cr_read(archivo, buff, 200);
    free(archivo);


    // cr_write(crFILE* file_desc, void* buffer, int nbytes);

    // cr_close(crFILE* file_desc);

    // cr_bitmap(1, 1);
    // cr_rm(1, "Baroque.mp3");
    // cr_ls(1);
    // cr_bitmap(1, 1);


    //cr_hardlink(2, "yes.mp3", "destino.mp3");


    //cr_softlink(2, 4, "yes.mp3", "yes.mp3");



    //cr_softlink(1, 4, "guides.txt", "guides.txt");


    // cr_unload(ROUTE, char* orig, char* dest);

    // cr_load(ROUTE, char* orig);



    return 1;
}
