#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "cr_API.h"

#include <math.h>

extern char* PATH;

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
crFILE* cr_open(unsigned disk, char* filename, char mode){
    int existe;
    existe = cr_exists(disk, filename);
    if (mode == 'r')
    {
      if (existe == 0)
      {
        return NULL;
      }
      FILE *ptr; ///Cachar que onda con esto, se deberia abrir con el path?
      ptr = fopen(PATH, "rb");
      //fseek(ptr, 32*256*20416, SEEK_SET);

      ////buscar el archivo
      
      fseek(ptr, 0, SEEK_SET);
      unsigned char buffer[3];
      fread(buffer, sizeof(buffer), 1, ptr);
      unsigned char b;
      int suma = 0;
      int bloque = 0;

      for (int i = 0; i < 3; i++)
      {
        b = buffer[i];
        //printf("%c", b);
        for(int j = 0; j < 8; j++)
        {
          //bloque += (23 - 8*i)
          bloque = (23 - 8*i) - (j);
          // printf("bloque %i\n", bloque);
          // printf("%d\n", (b>>(7-j))&1);
          if ((b>>(7-j)&1) == 1 && bloque < 23)
          {
            //printf("........\n");
            suma += pow(2, bloque); ///este es el bloque que necesita.
            //suma += 1;
            }
          }
          //printf("\n");
      }
      printf("suma: %i\n", suma);

      // for (int j = 0; j < 256; j++)
      // {
      // unsigned char b;
      // for (int i = 0; i < 8192; i++)
      // {
      //   b = buffer[i];
      //   printf("%c", b);
      //   // for(int i = 0; i < 8; i++)
      //   // {
      //   //     printf("%d", (b>>(7-i))&1);
      //   // }
      //   // printf("\n");
      // }

      printf(",,,,,,,,,,,\n");
      printf("Esto es de prueba\n");
      int porte;
      porte = suma /(256 * 32);
      if (porte * 256 * 32 != suma)
      {
        porte += 1;
      }
      printf("porte: %i\n", porte);

      // unsigned char buffer2[8];
      // fread(buffer2, sizeof(buffer2), 1, ptr);
      //
      // // for (int j = 0; j < 256; j++)
      // // {
      //
      // unsigned char b2;
      // for (int i = 0; i < 8; i++)
      // {
      //   b2 = buffer2[i];
      //   printf("%c\n", b2);
      //   for(int i = 0; i < 8; i++)
      //   {
      //       printf("%d", (b2>>(7-i))&1);
      //   }
      //   printf("\n");
      // }

      printf(",,,,,,,,,,,\n");
    //
    //   unsigned char buffer3[8176];
    //   fread(buffer3, sizeof(buffer3), 1, ptr);
    //
    //   // for (int j = 0; j < 256; j++)
    //   // {
    //
    //   unsigned char b3;
    //   for (int i = 0; i < 1910; i++)
    //   {
    //     for (int k = 0; k < 4; k++)
    //     {
    //     b3 = buffer3[i + k];
    //     //printf("%c\n", b3);
    //     for(int i = 0; i < 8; i++)
    //     {
    //         printf("%d", (b3>>(7-i))&1);
    //     }
    //   }
    //   i = i + 3;
    //     printf("\n");
    //   }
    //   printf("\n");
    // // }

      //int aux; //ver si hay forma de que no sea esto necesario


      fclose(ptr);
      return 0;
    }

    crFILE* crfile = malloc(sizeof(crFILE));
    crfile ->size=1;
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
