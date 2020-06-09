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


    // cr_hardlink y cr_rm agrgeando harlink en partición 2 y agregando

    /*
    printf("---------Agregar un Hardlink-----------\n");
    // Agregar un hardlink bien
    cr_ls(2);
    cr_hardlink(2, "yes.mp3", "destino.mp3");
    cr_ls(2);

    printf("----------Agregar un Hardlink con el nombre mal escrito-----------\n");
    // Agregar un hardlink con nombre mal escrito
    cr_ls(2);
    cr_hardlink(2, "yess.mp3", "destino2.mp3");
    cr_ls(2);

    // Borrar uno de los archivos
    printf("---------Borrar un Hardlink pero no todas las referencias----------\n");
    cr_bitmap(2, 0);
    cr_ls(2);
    cr_rm(2, "yes.mp3");
    cr_ls(2);
    cr_bitmap(2, 0);

    //Borrar el archivo completo
    printf("---------Borrar un Hardlink y todas las referencias----------\n");
    cr_ls(2);
    cr_rm(2, "destino.mp3");
    cr_ls(2);
    cr_bitmap(2, 0);

    // Agregar un hardlink a us softlink
    cr_ls(4);
    printf("---------Agregar un hardlink a un softlink----------\n");
    cr_hardlink(4, "1/Baroque.mp3", "destino2.mp3");
    cr_ls(4);
    */




    /*
    // cr_rm con los hardlinks de la partición 3
    printf("---------Borrar todas las referencias de un hardlink una por una----------\n");
    cr_ls(3);
    cr_rm(3, "Program in C.mkv");
    cr_ls(3);
    cr_bitmap(3, 0);
    cr_rm(3, "Program.mkv");
    cr_ls(3);
    cr_bitmap(3, 0);
    cr_rm(3, "in.mkv");
    cr_ls(3);
    cr_bitmap(3, 0);
    cr_rm(3, "C.mkv");
    cr_ls(3);
    cr_bitmap(3, 0);

    printf("---------Borrar un hardlink con el nombre mal escrito----------\n");
    cr_ls(3);
    cr_rm(3, "text.mkv");
    cr_ls(3);
    */




    // cr_softlink

    /*
    printf("----------Agregar un Softlink con el nombre mal escrito-----------\n");
    cr_ls(4);
    cr_softlink(1, 4, "text.mp3", "text.mp3");
    cr_ls(4);

    printf("----------Agregar un Softlink con nombres distintos-----------\n");
    cr_ls(4);
    cr_softlink(1, 4, "text.txt", "textt.txt");
    cr_ls(4);

    printf("----------Agregar un Softlink bien-----------\n");
    cr_ls(4);
    cr_softlink(1, 4, "text.txt", "text.txt");
    printf("       \n");
    cr_ls(4);

    printf("----------Agregar un Softlink a un softlink-----------\n");
    cr_ls(4);
    cr_softlink(1, 4, "3/Baroque.mp3", "3/Baroque.mp3");

    cr_ls(4);

    printf("----------Remover un Softlink----------\n");
    cr_ls(4);
    cr_rm(4, "1/text.txt");
    cr_ls(4);
    */








    return 1;
}
