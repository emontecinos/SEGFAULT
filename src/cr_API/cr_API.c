#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "cr_API.h"

#include <math.h>

extern char* PATH;

int calculo_numero(unsigned char buffer[], int iii);

void cr_mount(char* diskname){
    PATH = diskname;
    return;
}
void cr_bitmap(unsigned disk, bool hex){
    int fd= 2; // STRERR
    char buff[65536];
    off_t bit_a_revisar=0;
    int bytes_read;
    size_t size_block=65536;
    size_t leidos;
    FILE* file = fopen("simdiskfilled.bin","rb");
    int c,i;
    bit_a_revisar= disk*536870912 + 8192;
    fseek(file,bit_a_revisar,SEEK_SET);
    // fread(buff, 1, 1, file);
    for (i=0; i < 16 && (c=getc(file))!=EOF; i++){
        fprintf(stderr,"%x",c);
    }
    printf("\ni %d\n",i);
    fseek(file,bit_a_revisar,SEEK_SET);
    for (i=0; i < 16 && (c=getc(file))!=EOF; i++){
        fprintf(stderr,"%02X",c&0xff);
        fprintf(stderr," ");


    }
    printf("\ni %d\n",i);



    // printf("Marc1\n");
    // fflush(stdout);
    // fread(buff,size_block,1,file);
    // printf("Marc2\n");
    // fflush(stdout);
    // fprintf(stderr,"%x\n",buff);
    fclose(file);

    return;
}

int cr_exists(unsigned disk, char* filename){
    return 1;
}
void cr_ls(unsigned disk){
    return;
}
crFILE* cr_open(unsigned disk, char* filename, char* mode){

    if (mode == "r")
    {
      printf("aaaa\n");
    }
    if (mode != "r" && mode != "w")
    {
      return NULL;
    }
    int existe;
    existe = cr_exists(disk, filename);
    int indice;
    int referencias;
    int porte;


    if (existe == 1)
    {
      FILE *ptr; ///Cachar que onda con esto, se deberia abrir con el path?
      ptr = fopen(PATH, "rb");

      ////FALTA buscar el archivo

      fseek(ptr, 0, SEEK_SET);
      unsigned char buffer[3];
      fread(buffer, sizeof(buffer), 1, ptr);

      indice = calculo_numero(buffer, 3);

      printf("suma: %i\n", indice);

      fseek(ptr, 32*256*indice, SEEK_SET); //bloqie indice

      ///referencias
      unsigned char buffer2[4];
      fread(buffer2, sizeof(buffer2), 1, ptr);

      referencias = calculo_numero(buffer2, 4);
      printf("referencias %i\n", referencias);

      //tamaño
      unsigned char buffer3[8];
      fread(buffer3, sizeof(buffer3), 1, ptr);
      int cantidad;
      cantidad = calculo_numero(buffer3, 8);
      printf("cantidad %i\n", cantidad);

      porte = cantidad /(256 * 32);
      if (porte * 256 * 32 != cantidad)
      {
        porte += 1;
      }
      printf("porte: %i\n", porte);

      fclose(ptr);
    }

    else if(existe == 0)
    {
      if (mode == "r")
      {
        if (existe == 0)
        {
          return NULL;
        }
      }
      else if (mode == "w")
      {
        indice = 0;
        referencias = 0;
        porte = 0;
      }

    }

    else
    {
      return NULL;
    }
    /////////
    crFILE* crfile = malloc(sizeof(crFILE));
    crfile -> size = 1;
    crfile -> nombre = filename;
    crfile -> puntero_a_bloque = indice;
    crfile -> cant_hardlinks = referencias;
    crfile -> cant_bloques = porte;
    crfile -> modo = mode;
    crfile -> existe = existe;
    return crfile;
}

int cr_read (crFILE* file_desc, void* buffer, int nbytes){
    return 1;
}
int cr_write(crFILE* file_desc, void* buffer, int nbytes){
    return 1;
}
int cr_close(crFILE* file_desc){
    free(file_desc);
    return 1;
}
int cr_rm(unsigned disk, char* filename){
    return 1;
}
int cr_hardlink(unsigned disk, char* orig, char* dest){
    return 1;
}
int cr_softlink(unsigned disk_orig, unsigned disk_dest, char* orig, char* dest){
    return 1;
}
int cr_unload(unsigned disk, char* orig, char* dest){
    return 1;
}
int cr_load(unsigned disk, char* orig){
    return 1;
}

int calculo_numero(unsigned char buffer[], int iii)
{
  int suma = 0;
  int bloque = 0;
  unsigned char b;

  for (int i = 0; i < iii; i++)
  {
    b = buffer[i];
    //printf("%c", b);
    for(int j = 0; j < 8; j++)
    {
      //bloque += (23 - 8*i)
      bloque = (8*iii - 8*i - 1) - (j);
      // printf("bloque %i\n", bloque);
      // printf("%d\n", (b>>(7-j))&1);
      if ((b>>(7-j)&1) == 1 && bloque < 23)
      {
        // printf("%i\n", bloque);
        // printf("........\n");
        suma += pow(2, bloque); ///este es el bloque que necesita.
        //suma += 1;
        }
      }
      //printf("\n");
  }
  return suma;
}
