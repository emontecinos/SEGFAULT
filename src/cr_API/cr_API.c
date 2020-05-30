#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
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
  // printf("%u\n", disk);
  // printf("%s\n", filename);
  // printf("..............\n");
  if (disk == 1 || disk == 2 || disk == 3 || disk == 4)
  {
    FILE *ptr; ///Cachar que onda con esto, se deberia abrir con el path?
    // if (strchr(filename, '/') != NULL)
    // {
    // printf("aaaaaa\n");
    // }

    ptr = fopen(PATH, "rb");

    fseek(ptr, 32*256*65536*(disk - 1), SEEK_SET);
    unsigned char buffer[32];
    int aux; //ver si hay forma de que no sea esto necesario
    for (int i = 0; i < 256; i++)
    {
      fread(buffer, sizeof(buffer), 1, ptr);

        if(buffer[0] > 0x7f)
        {
          int j;
          for (j = 0; buffer[j + 3] || filename[j]; j++)
          {
            aux = 1;
            if (buffer[j + 3] != filename[j])
            {
              aux = 0;
              break;
            }
          }
          if (aux == 1)
          {
            fclose(ptr);
            return 1;
          }
        }
    }
    fclose(ptr);
    return 0;
  }
else
{
  return -1;
}
}

void cr_ls(unsigned disk){
    return;
}
crFILE* cr_open(unsigned disk, char* filename, char* mode)
{
    if (disk != 1 && disk != 2 && disk != 3 && disk != 4)
      {
        printf("Disco no existente\n");
        return NULL;
      }


    if (strncmp(mode, "r", 2) != 0 && strncmp(mode, "w", 2) != 0)
    {
      printf("modo no valido\n");
      return NULL;
    }
    int existe;
    existe = cr_exists(disk, filename);
    int puntero;
    int referencias;
    int porte;
    int cantidad;

    /////
    if (strncmp(mode, "w", 2) == 0)
    {
      if (existe == 1)
      {
        // printf("nnnnnn\n");
        int buscando = 1;
        int copia = 1;
        while(buscando)
        {
          char str[29];
          char* nuevo_nombre = filename;
          int largo = (sizeof(nuevo_nombre) / sizeof(nuevo_nombre[0]));
          if (largo + copia/10 + 1 >= 28)
          {
            printf("Nombre no disponible\n");
            return NULL;
          }

          sprintf(str, "%d%s", copia, nuevo_nombre);
          //printf("%s\n", str);
          if (cr_exists(disk, str) == 0)
          {
            filename = str;
            buscando = 0;
            printf("%s\n", filename);
          }
          copia += 1;
        }
      }

      FILE *ptr;
      ptr = fopen(PATH, "rb");
      /// ver bloque disponible en bitmap
      unsigned char buff[1];
      uint32_t num;
      fseek(ptr, 32*256*65536*(disk - 1) + 8192, SEEK_SET);
      int bloque_indice = 0;
      int i = 0;
      int encontrado = 1;
      while(encontrado)
      {
        if (i == 8192) //no hay espacio en bitmap
        {
          printf("No se pueden agregar mas archivos a disco\n");
          return NULL;
        }
        fread(buff, sizeof(buff), 1, ptr);
        if (buff[0] < 0xFF)
        {

          int inicio = (disk - 1)*65536;

            unsigned char b = buff[0]; // replace with whatever you've read out of your file
            for(int j = 0; j < 8; j++)
            {
                // printf("%d", (b>>(7 - j))&1);
                if (((b>>(7 - j))&1) == 0)
                {
                  bloque_indice = i*8 + j + inicio;
                  encontrado = 0;
                  break;
                }
            }
      }
      i++;
    }
      puntero = bloque_indice;
      referencias = 0;
      porte = 0;
      cantidad = 0;
    }

    else if(strncmp(mode, "r", 2) == 0)
    {
      if (existe == 0)
      {
        printf("Archivo no existente\n");
        return NULL;
      }

      FILE *ptr;
      ptr = fopen(PATH, "rb");

      int aux; //ver si hay forma de que no sea esto necesario
      unsigned char buffer_directorio[32];
      fseek(ptr, 32*256*65536*(disk - 1), SEEK_SET);

      //int puntero;
      for (int i = 0; i < 256; i++)
      {
        fread(buffer_directorio, sizeof(buffer_directorio), 1, ptr);

        if(buffer_directorio[0] > 0x7f)
        {
          int j;
          for (j = 0; buffer_directorio[j + 3] || filename[j]; j++)
          {
            aux = 1;
            if (buffer_directorio[j + 3] != filename[j])
            {
              aux = 0;
              break;
            }
          }
          if (aux == 1)
          {
            uint32_t num;
            // printf("num: %i\n", buffer[0]);
            num = buffer_directorio[0] - 0x80;
            puntero = num << 16|buffer_directorio[1] << 8|buffer_directorio[2];
            printf("puntero: %i\n", puntero);
            //puntero = i;
            break;
          }
        }
      }
      printf("puntero: %i\n", puntero);

      fseek(ptr, 32*256*puntero, SEEK_SET); //bloque indice

      ///referencias
      unsigned char buffer2[4];
      fread(buffer2, sizeof(buffer2), 1, ptr);

      referencias = buffer2[0] << 24 | buffer2[1] << 16|buffer2[2] << 8|buffer2[3];
      printf("referencias %i\n", referencias);
      // referencias = calculo_numero(buffer2, 4);
      // printf("referencias %i\n", referencias);

      //tamaño
      unsigned char buffer3[8];
      fread(buffer3, sizeof(buffer3), 1, ptr);
      // int cantidad;


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
    // printf("puntero: %i\n", puntero);

    else
    {
      printf("modo no valido\n");
      return NULL;
    }

    /////////
    crFILE* crfile = malloc(sizeof(crFILE));
    crfile -> size = cantidad;
    crfile -> nombre = filename;
    crfile -> puntero_a_bloque = puntero;
    printf("crfile -> puntero_a_bloque %i\n", crfile -> puntero_a_bloque);
    crfile -> cant_hardlinks = referencias;
    crfile -> cant_bloques = porte;
    crfile -> modo = mode;
    crfile -> existe = existe;
    crfile -> byte_leido = 0;
    return crfile;
}

int cr_read (crFILE* file_desc, void* buffer, int nbytes){
    int bloque;
    int byte_de_bloque;
    int bloque_para_leer;
    int bytes_leidos = 0;;
    int en_proceso = 1;
    bloque = (file_desc -> byte_leido) / (256 * 32);
    // printf("bloque: %i\n", bloque);
    byte_de_bloque = (file_desc -> byte_leido) - (bloque * 32 * 256);
    // printf("byte_de_bloque: %i\n", byte_de_bloque);
    /////////Buscar el bloque indice
    FILE *ptr;
    ptr = fopen(PATH, "rb");
    while (en_proceso) {
      // printf("%i\n", file_desc -> puntero_a_bloque);
      // printf("%i\n", 32*256*(file_desc -> puntero_a_bloque) + 12 + bloque*4);
      // fseek(ptr, 32*256*(file_desc -> puntero_a_bloque + 12 + bloque*4), SEEK_SET);
      printf("puntero: %i\n", file_desc -> puntero_a_bloque);
      fseek(ptr, 32*256*(file_desc -> puntero_a_bloque) + 12 + bloque * 4, SEEK_SET);

      ////// Buscar bloque para comenzar a leer
      //fseek(ptr, 12 + bloque*4, SEEK_CUR);
      unsigned char buffer[4];
      fread(buffer, sizeof(buffer), 1, ptr);
      printf("buffer %i\n", buffer[0]);
      bloque_para_leer = buffer[0] << 24 | buffer[1] << 16|buffer[2] << 8|buffer[3];
      printf("bloque_para_leer: %i\n", bloque_para_leer);
      // bloque_para_leer = calculo_numero(buffer, 4);
      // printf("%i\n", bloque_para_leer);
      ///////////

      fseek(ptr, 32*256*bloque_para_leer + byte_de_bloque, SEEK_SET);
      //leer
      //fseek(ptr, byte_de_bloque, SEEK_CUR);
      unsigned char buff[1];
      //minimo entre nbytes, lo que queda de este archivo y lo que queda de bloque
      //int aa = min(nbytes, )
      printf("aaaa\n");
      int i = 0;
      printf("nbytes - bytes_leidos %i\n", nbytes - bytes_leidos);
      printf("file_desc -> size - file_desc -> byte_leido - bytes_leidos %i\n", file_desc -> size - file_desc -> byte_leido - bytes_leidos);
      printf("32*256 - byte_de_bloque %i\n", 32*256 - byte_de_bloque);
      for (i = 0; i < nbytes - bytes_leidos && i < file_desc -> size - file_desc -> byte_leido - bytes_leidos && i < 32*256 - byte_de_bloque; i++)
      {
        // printf("%i\n", i);
        fread(buff, sizeof(buff), 1, ptr);
        //buffer[i] = buff[0];
        printf("%s", buff);
      }
      bytes_leidos += i;
      printf("nbytes %i\n", nbytes);
      // printf("i %i\n", i);
      printf("bytes_leidos = %i\n", bytes_leidos);
      if (file_desc -> size - file_desc -> byte_leido <= bytes_leidos)
      {
        ///no hay mas archivo
        return bytes_leidos;
      }
      else if(nbytes <= bytes_leidos)
      {
        ///se leyo todo lo que se queria leer
        return bytes_leidos;
      }
      else
      {
        bloque += 1;
        printf("\n");
        byte_de_bloque = 0;
      }
  }
    return bytes_leidos;
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
      // printf("\n");
  }
  return suma;
}
