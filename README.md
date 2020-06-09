Grupo: SEGFAULT

Integrantes:
  1. Raimundo Eyzaguirre. Número alumno: 15633616
  2. Eduardo Montecinos. Número alumno 1563776j
  3. Pilar Parga. Número alumno: 15622258
  4. María Jesús Retamales. Número alumno: 16640772


Principales decisiones de diseño:

El struct para crFile incluye los atributos:
  * size: representa el tamaño en bytes del archivo. Solo incluye los bytes efectivos de datos, no el espacio utilizado en guardar otra información.
  * nombre: guarda el nombre del archivo
  * puntero_a_bloque: el número de bloque en donde se encuentra el bloque índice del archivo
  * cant_hardlinks: numero de hard links asociados al archivo
  * modo: el modo en que se está abriendo el archivo
  * cant_bloques: la cantidad de bloques que utiliza el contenido del archivo
  * existe: si existía previamente
  * byte_leido: el último byte leído desde que se abrió el archivo

Supuestos:

Para la función cr_write se considera un archivo inicial vacío y se comienza a escribir. No hay append.

En la función cr_load, no se consideran los softlinks, ya que no es necesario al descargar el archivo al que hacen referencia.

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../cr_API/cr_API.h"

extern char* PATH;
int main(int argument_count, char** arguments){

   //cr_bitmap(1, 0);
  // cr_bitmap(2, 1);
  // cr_bitmap(3, 1);
  // cr_bitmap(4, 1);
  // cr_bitmap(0, 1);
  
  
  
  
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











 /*
 -------------------------------- 1 ---------------------------------------------------
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
   -------------------------------------- 2 ----------------------------------------
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

    /*
    ---------------------------------- 3 ------------------------------------------
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
    cr_softlink(4, 4, "3/Baroque.mp3", "3/Baroque.mp3");

    cr_ls(4);

    printf("----------Remover un Softlink----------\n");
    cr_ls(4);
    cr_rm(4, "1/text.txt");
    cr_ls(4);
    */



    /////////////////////////////////////////////////////////////////
    // cr_unload(1, "Baroque.mp3", "dest");
    // cr_unload(0, NULL, "dest");
    // cr_unload(2, "yes.mp3", "dest");
    // cr_unload(2, NULL, "dest");
    // cr_unload(3, "Program.mkv", "dest");
    // cr_unload(3, NULL, "dest");
    // cr_unload(4, "1/Baroque.mp3", "dest");
    // cr_unload(4, "1/aaaa.gif", "dest");
    // cr_unload(4, NULL, "dest");
    ////////////////////////////////////////////////////////////////
    // cr_load(1, "dest");
    // cr_load(1, "lala,txt");
    ////////////////////////////////////////////////////////////////

    return 1;
}
