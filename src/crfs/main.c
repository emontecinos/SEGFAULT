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

    // cr_bitmap(4, 1);


    ///////////
    // cr_ls(1);
    // printf("aa\n");
    // crFILE* archivo;
    // archivo = cr_open(1, "guides2.txt", "r");
    // unsigned char buff[300];
    // int a;
    // a = cr_read(archivo, buff, 300);
    // printf("%i\n", a);
    // cr_close(archivo);
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
    // crFILE* archivo;
    // archivo = cr_open(1, "holahola.txt", "r");
    // crFILE* archivo2;
    // archivo2 = cr_open(1, "holahola.txt", "f");
    ///////

    ///////////
    // cr_ls(2);
    // crFILE* archivo;
    // archivo = cr_open(2, "holahola.txt", "r");
    // int aa;
    // unsigned char buff[300];
    // aa = cr_read(archivo, buff, 2);
    ///////

    ///////////
    // cr_ls(2);
    // crFILE* archivo;
    // archivo = cr_open(2, "text.txt", "w");
    // cr_ls(2);
    ///////

    ///////////
    // cr_ls(8);
    // int a;
    // a = cr_exists(4, "text.txt");
    // printf("%i\n", a);
    // cr_ls(8);
    ///////

    ///////
    // // cr_hardlink y cr_rm agrgeando harlink en partición 2 y agregando
    //
    // /*
    // printf("---------Agregar un Hardlink-----------\n");
    // // Agregar un hardlink bien
    // cr_ls(2);
    // cr_hardlink(2, "yes.mp3", "destino.mp3");
    // cr_ls(2);
    ///////

    ///////
    // printf("----------Agregar un Hardlink con el nombre mal escrito-----------\n");
    // // Agregar un hardlink con nombre mal escrito
    // cr_ls(2);
    // cr_hardlink(2, "yess.mp3", "destino2.mp3");
    // cr_ls(2);
    ///////


    ///////
    // // Borrar uno de los archivos
    // printf("---------Borrar un Hardlink pero no todas las referencias----------\n");
    // cr_bitmap(2, 0);
    // cr_ls(2);
    // cr_rm(2, "yes.mp3");
    // cr_ls(2);
    // cr_bitmap(2, 0);
    ///////


    ///////
    // //Borrar el archivo completo
    // printf("---------Borrar un Hardlink y todas las referencias----------\n");
    // cr_ls(2);
    // cr_rm(2, "destino.mp3");
    // cr_ls(2);
    // cr_bitmap(2, 0);
    ///////


    ///////
    // // Agregar un hardlink a us softlink
    // cr_ls(4);
    // printf("---------Agregar un hardlink a un softlink----------\n");
    // cr_hardlink(4, "1/Baroque.mp3", "des")
    ///////

    return 1;
}
