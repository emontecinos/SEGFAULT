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

    printf("%s\n",PATH);

    //cr_bitmap(1, 1);

    // cr_exists(ROUTE, char* filename);

    // cr_ls(ROUTE);

    crFILE* archivo;
    archivo = cr_open(1, "text.txt", "r");
    if (archivo == NULL)
    {
      printf("NULL\n");
    }
    cr_read(archivo, 1, 3906118);

    // cr_write(crFILE* file_desc, void* buffer, int nbytes);

    // cr_close(crFILE* file_desc);

    // cr_rm(ROUTE, char* filename);

    // cr_hardlink(ROUTE, char* orig, char* dest);

    // cr_softlink(unsigned disk_orig, unsigned disk_dest, char* orig, char* dest);

    // cr_unload(ROUTE, char* orig, char* dest);

    // cr_load(ROUTE, char* orig);



    return 1;
}
