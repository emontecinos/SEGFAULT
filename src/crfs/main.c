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
     //cr_ls(1);
    //cr_ls(2);
    // cr_ls(3);

    int existe;
    existe = cr_exists(2, "yes.mp3");
    printf("existe: %i\n", existe);

    return 1;
}
