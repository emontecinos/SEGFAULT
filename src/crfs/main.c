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

    ///////////
    // cr_ls(1);
    // crFILE* archivo;
    // archivo = cr_open(1, "guides2.txt", "r");
    // unsigned char buff[300];
    // int a;
    // a = cr_read(archivo, buff, 300);
    // printf("%i\n", a);
    ///////

    ///////////
    // cr_ls(2);
    // crFILE* archivo;
    // archivo = cr_open(2, "text.txt", "r");
    // unsigned char buff[300];
    // int a;
    // a = cr_read(archivo, buff, 3);
    // printf("%i\n", a);
    ///////

    ///////////
    // cr_ls(1);
    // int a;
    // a = cr_exists(1, "text.txt");
    // printf("text.txt existe: %i\n", a);
    // int b;
    // b = cr_exists(1, "holahola.txt");
    // printf("holahola.txt existe: %i\n", b);
    ///////

    ///////////
    // cr_ls(2);
    // crFILE* archivo;
    // archivo = cr_open(2, "text.txt", "w");
    // cr_ls(2);
    ///////



    return 1;
}
