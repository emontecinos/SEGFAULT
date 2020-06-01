#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include "cr_API.h"
#include <string.h>

extern char* PATH;

void cr_mount(char* diskname){
    PATH = diskname;
    return;
}
void cr_bitmap(unsigned disk, bool hex){

    unsigned char buffer[65536]; //#bloques en particion
    char aux_buffer[1];
    size_t bytes_leidos=0;
    unsigned int byte_lectura;
    int bloques_ocupados=0;
    int bloques_desocupados=0;
    int aux_bytes_leidos=0;
    int aux_bit;
    int pos_buffer=0;
    FILE* file = fopen(PATH,"rb");
    if(disk > 0){
        byte_lectura=(disk-1)*536870912+8192; // tamano particion + un bloque
        // lectura en bytes
        fseek(file,byte_lectura,SEEK_SET); // puntero lectura desde posicion byte_lectura desde el inicio (SEEK_SET)
        while(bytes_leidos < 8192){
            aux_bytes_leidos=fread(aux_buffer,1,1,file);
            for (int i =0; i < 8; i++){
                aux_bit= aux_buffer[0]&1; // and entre el byte leido y 1; al shiftearlo voy bit por bit.

                if(aux_bit==1){ // aumento el contador
                    bloques_ocupados++;
                }else {
                    bloques_desocupados++;
                    }
                buffer[pos_buffer]=aux_bit+'0';
                aux_buffer[0]>>=1; //shift right en 1.
                aux_bit++;
                pos_buffer++;
            }
            bytes_leidos += 1;
        }
    }else{
        printf("non\n");
    }
    // for(int i=0; i < 65536;i++){
    //     printf("%c",buffer[i]);
    //     if (i %15==0){
    //         printf("\n");
    //     }
    // }
    printf("disco: %d\nOcupados: %d Libres: %d\nTotal: %d\n",disk,bloques_ocupados,bloques_desocupados,bloques_ocupados+bloques_desocupados);
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

    if (disk != 1 && disk != 2 && disk != 3 && disk != 4)
    {
      printf("Disco no existente\n");
      return;
    }
    unsigned char buffer[32];
    FILE *ptr; ///Cachar que onda con esto, se deberia abrir con el path?

    ptr = fopen(PATH,"rb");

    fseek(ptr, 32*256*65536*(disk - 1), SEEK_SET);

    for (int i = 0; i < 256; i++)
    {
      fread(buffer,sizeof(buffer),1,ptr);

        if(buffer[0] > 0x7f)
        {
          uint32_t num, num4;
          num4 = buffer[0] - 0x80;
          num = num4 << 16 | buffer[1] << 8|buffer[2];
          printf("puntero: %i\n", num);
          for (int j = 3; j < 32; j++)
          {
            printf("%c", buffer[j]);
          }
          printf("\n");
        }
    }
    fclose(ptr);
    return;
}
crFILE* cr_open(unsigned disk, char* filename, char mode){
    crFILE* crfile=malloc(sizeof(crFILE));
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
  if(cr_exists(disk, filename)){
    printf("%s\n", filename );
    printf("True\n");
    FILE *ptr;
    if (strchr(filename, '/') != NULL)
    {
    printf("aaaaaa\n");
    }

    ptr = fopen(PATH, "rb");

    fseek(ptr, (disk-1)*32*256*65536, SEEK_SET);
    int aux;
    int puntero;
    int aux_puntero_vaciar = 0;
    uint32_t puntero_vaciar = (disk-1)*32*256*65536;
    unsigned char nombre[29];
    unsigned char buffer[32];
    unsigned char buffer_destino[3];
    for (int i = 0; i < 256; i++)
    {

      fread(buffer, sizeof(buffer), 1, ptr);
      //printf("%c\n", buffer[3] );
      if(buffer[0] > 0x7f)
      {
        memcpy(nombre, &buffer[3], 29 * sizeof(unsigned char));
        //printf("%c\n", nombre[0] );
        //fread(nombre, sizeof(nombre), 1, ptr);
        int j;
        //printf("Nueva palabra\n");
        for (j = 0; nombre[j] || filename[j]; j++)
        {
          //printf("%c\n", orig[j] );
          //printf("%c\n", nombre[j]);
          aux = 1;
          if (nombre[j] != filename[j])
          {
            aux = 0;
            break;
          }
        }
        if (aux == 1)
        {
          buffer_destino[0] = buffer [0];
          buffer_destino[1] = buffer [1];
          buffer_destino[2] = buffer [2];
          buffer [0] = buffer[0]&127;
          int num = (int)buffer[0]<<16| (int)buffer[1]<<8 | (int)buffer[2];
          puntero = num;
          printf("Puntero: %d, Nombre: %s\n", puntero, nombre);
          aux_puntero_vaciar = 1;
          break;

        }
        if(aux_puntero_vaciar == 0){
          puntero_vaciar += 32;
        }
      }
      else if (buffer[0] <=  0x7f && aux_puntero_vaciar == 0){
        puntero_vaciar += 32;
      }
    }
    fclose(ptr);


    ptr = fopen(PATH, "rb+");
    fseek(ptr, puntero_vaciar, SEEK_SET);
    unsigned char buffer_vacio[32];
    int i;
    for ( i= 0; i<32; i++){
      buffer_vacio[i] = 0;
    }
    fwrite(buffer_vacio, sizeof(buffer_vacio), 1, ptr);

    unsigned char cantidad_hardlinks[4];
    fseek(ptr, 32*256*puntero, SEEK_SET);
    fread(cantidad_hardlinks, sizeof(cantidad_hardlinks), 1, ptr);
    int num = (int)cantidad_hardlinks[0]<<24| (int)cantidad_hardlinks[1]<<16 | (int)cantidad_hardlinks[2]<<8 |(int)cantidad_hardlinks[3];
    printf("%d\n",num);
    num -= 1;
    unsigned char bytes[4];
    bytes[0] = (num >> 24) & 0xFF;
    bytes[1] = (num >> 16) & 0xFF;
    bytes[2] = (num >> 8) & 0xFF;
    bytes[3] = num & 0xFF;
    fseek(ptr, 32*256*puntero, SEEK_SET);
    fwrite(bytes, sizeof(bytes), 1, ptr);
    fseek(ptr, 32*256*puntero, SEEK_SET);
    fread(cantidad_hardlinks, sizeof(cantidad_hardlinks), 1, ptr);
    int nume = (int)bytes[0]<<24| (int)bytes[1]<<16 | (int)bytes[2]<<8 |(int)bytes[3];
    printf("%d\n",nume);
    if(num == 0){
      int punto = puntero*256*32;
      unsigned int byte_escritura;
      unsigned char byte_lectura;
      int byte_actualizado;
      byte_escritura=((int)puntero - (disk-1)*pow(2,16))/8 + (disk-1)*pow(2,29)+pow(2,13);
      fseek(ptr,byte_escritura,SEEK_SET);
      fread(&byte_lectura,1,1,ptr);
      byte_actualizado=(int)byte_lectura & (255 - (int)pow(2, (int)(puntero - (disk-1)*pow(2,16)) % 8));
      fseek(ptr,-1,SEEK_CUR);
      fwrite(&byte_actualizado,1,1,ptr);

      punto += 8;
      unsigned char tamano[8];
      int num2 = calculo_numero(tamano, 8);
      uint64_t porte;
      porte = num2/(8192);
      if (porte * 8192 != num2)
      {
        porte += 1;
      }
      int i;
      if(porte <= 2044 ){
        for (i = 0; i < porte; i++){
          unsigned char bit_a_borrar[4];
          fseek(ptr, punto, SEEK_SET);
          fread(bit_a_borrar, sizeof(bit_a_borrar), 1, ptr);
          int n_bit_a_borrar = (int)bit_a_borrar[0]<<24| (int)bit_a_borrar[1]<<16 | (int)bit_a_borrar[2]<<8 |(int)bit_a_borrar[3];
          byte_escritura=(n_bit_a_borrar - (disk-1)*pow(2,16))/8 + (disk-1)*pow(2,29)+pow(2,13);
          fseek(ptr,byte_escritura,SEEK_SET);
          fread(&byte_lectura,1,1,ptr);
          byte_actualizado=(int)byte_lectura & (255 -(int)pow(2, (int)(n_bit_a_borrar - (disk-1)*pow(2,16)) % 8));
          fseek(ptr,-1,SEEK_CUR);
          fwrite(&byte_actualizado,1,1,ptr);
          punto += 4;
        }
      }
      else if (porte > 2044 ){
        for (i = 0; i < 2044; i++){
          unsigned char bit_a_borrar[4];
          fseek(ptr, punto, SEEK_SET);
          fread(bit_a_borrar, sizeof(bit_a_borrar), 1, ptr);
          int n_bit_a_borrar = (int)bit_a_borrar[0]<<24| (int)bit_a_borrar[1]<<16 | (int)bit_a_borrar[2]<<8 |(int)bit_a_borrar[3];
          byte_escritura=(n_bit_a_borrar - (disk-1)*pow(2,16))/8 + (disk-1)*pow(2,29)+pow(2,13);
          fseek(ptr,byte_escritura,SEEK_SET);
          fread(&byte_lectura,1,1,ptr);
          byte_actualizado=(int)byte_lectura & (255 -(int)pow(2, (int)(n_bit_a_borrar - (disk-1)*pow(2,16)) % 8));
          fseek(ptr,-1,SEEK_CUR);
          fwrite(&byte_actualizado,1,1,ptr);
          punto += 4;
        }
        fseek(ptr, punto, SEEK_SET);
        unsigned char indireccionamiento[4];
        fread(indireccionamiento, sizeof(punto), 1, ptr);
        for (i = 0; i < porte - 2044; i++){
          unsigned char bit_a_borrar[4];
          fseek(ptr, punto, SEEK_SET);
          fread(bit_a_borrar, sizeof(bit_a_borrar), 1, ptr);
          int n_bit_a_borrar = (int)bit_a_borrar[0]<<24| (int)bit_a_borrar[1]<<16 | (int)bit_a_borrar[2]<<8 |(int)bit_a_borrar[3];
          byte_escritura=(n_bit_a_borrar - (disk-1)*pow(2,16))/8 + (disk-1)*pow(2,29)+pow(2,13);
          fseek(ptr,byte_escritura,SEEK_SET);
          fread(&byte_lectura,1,1,ptr);
          byte_actualizado=(int)byte_lectura & (255 -(int)pow(2, (int)(n_bit_a_borrar - (disk-1)*pow(2,16)) % 8));
          fseek(ptr,-1,SEEK_CUR);
          fwrite(&byte_actualizado,1,1,ptr);
          punto += 4;
        }
      }
    }
    fclose(ptr);
    return 1;
  }
  else{
    printf("El archivo %s no existe.\n", filename);
    return 1;
  }
}
int cr_hardlink(unsigned disk, char* orig, char* dest){
  if(cr_exists(disk, orig)){
    printf("%s\n", orig );
    printf("True\n");
    FILE *ptr;
    if (strchr(orig, '/') != NULL)
    {
    printf("aaaaaa\n");
    }

    ptr = fopen(PATH, "rb");

    fseek(ptr, (disk-1)*32*256*65536, SEEK_SET);
    int aux;
    int puntero;
    int aux_puntero_nuevo = 0;
    uint32_t puntero_nuevo_link = (disk-1)*32*256*65536;
    unsigned char nombre[29];
    unsigned char buffer[32];
    unsigned char buffer_destino[3];
    for (int i = 0; i < 256; i++)
    {

      fread(buffer, sizeof(buffer), 1, ptr);
      //printf("%c\n", buffer[3] );
      if(buffer[0] > 0x7f)
      {
        memcpy(nombre, &buffer[3], 29 * sizeof(unsigned char));
        //printf("%c\n", nombre[0] );
        //fread(nombre, sizeof(nombre), 1, ptr);
        int j;
        //printf("Nueva palabra\n");
        for (j = 0; nombre[j] || orig[j]; j++)
        {
          //printf("%c\n", orig[j] );
          //printf("%c\n", nombre[j]);
          aux = 1;
          if (nombre[j] != orig[j])
          {
            aux = 0;
            break;
          }
        }
        if (aux == 1)
        {
          buffer_destino[0] = buffer [0];
          buffer_destino[1] = buffer [1];
          buffer_destino[2] = buffer [2];
          buffer [0] = buffer[0]&127;
          int num = (int)buffer[0]<<16| (int)buffer[1]<<8 | (int)buffer[2];
          puntero = num;
          printf("Puntero: %d, Nombre: %s\n", puntero, nombre);

        }
        if(aux_puntero_nuevo == 0){
          puntero_nuevo_link += 32;
        }
      }
      else if (buffer[0] <=  0x7f && aux_puntero_nuevo == 0){
        printf("%i\n", puntero_nuevo_link);
        aux_puntero_nuevo = 1;
      }
    }
    fclose(ptr);
    if(aux_puntero_nuevo == 0){
      printf("No quedá memoria en el disco\n");

      return 1;
    }
    else{
      ptr = fopen(PATH, "rb+");
      fseek(ptr, puntero_nuevo_link, SEEK_SET);
      fwrite(buffer_destino, 1, 3, ptr);
      fwrite(dest, strlen(dest), 1, ptr);
      fclose(ptr);

      unsigned char cantidad_hardlinks[4];
      ptr = fopen(PATH, "rb+");


      fseek(ptr, 32*256*puntero, SEEK_SET);
      fread(cantidad_hardlinks, sizeof(cantidad_hardlinks), 1, ptr);
      int num = (int)cantidad_hardlinks[0]<<24| (int)cantidad_hardlinks[1]<<16 | (int)cantidad_hardlinks[2]<<8 |(int)cantidad_hardlinks[3];
      printf("%d\n",num);
      num += 1;
      unsigned char bytes[4];
      bytes[0] = (num >> 24) & 0xFF;
      bytes[1] = (num >> 16) & 0xFF;
      bytes[2] = (num >> 8) & 0xFF;
      bytes[3] = num & 0xFF;
      fseek(ptr, 32*256*puntero, SEEK_SET);
      fwrite(bytes, sizeof(bytes), 1, ptr);
      fseek(ptr, 32*256*puntero, SEEK_SET);
      fread(cantidad_hardlinks, sizeof(cantidad_hardlinks), 1, ptr);
      int nume = (int)bytes[0]<<24| (int)bytes[1]<<16 | (int)bytes[2]<<8 |(int)bytes[3];
      printf("%d\n",nume);
      return 1;
    }
  }
  else{
    printf("El archivo %s no existe\n", orig);
    return 1;
  }
}
int cr_softlink(unsigned disk_orig, unsigned disk_dest, char* orig, char* dest){
  if(orig != dest){
    printf("El archivo %s es distinto de %s\n", orig, dest);
    return 1;
  }

  else if(cr_exists(disk_orig, orig)){
    FILE *ptr;
    ptr = fopen(PATH, "rb+");
    char str_dest[50];
    sprintf(str_dest, "%d", disk_orig);
    strcat(str_dest, "/");
    strcat(str_dest, dest);
    printf("%s\n", str_dest );
    fseek(ptr, (disk_dest-1)*32*256*65536, SEEK_SET);
    unsigned char buffer2[32];
    int aux2 = 0;
    int contador = (disk_dest-1)*32*256*65536;
    for (int i = 0; i < 256; i++)
    {
      fread(buffer2, sizeof(buffer2), 1, ptr);
      //printf("%c\n", buffer[3] );
      if(buffer2[0] <= 0x7f && aux2 == 0){
        aux2 = 1;
        break;
      }
      else{
        contador += 32;
      }
    }
    fseek(ptr, contador, SEEK_SET);
    unsigned char bytes[3];
    bytes[0] = (0 >> 16) & 0xFF;
    bytes[1] = (0 >> 8) & 0xFF;
    bytes[2] = (0) & 0xFF;
    bytes[0] = bytes[0]|128;
    fwrite(bytes, sizeof(bytes), 1, ptr);
    fwrite(str_dest, strlen(str_dest), 1, ptr);
    fclose(ptr);
    return 1;
  }
  else{
    printf("El archivo %s no existe.\n", orig);
    return 1;
  }
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
    for(int j = 0; j < 8; j++)
    {
      bloque = (8*iii - 8*i - 1) - (j);
      if ((b>>(7-j)&1) == 1 && bloque < 23)
      {
        suma += pow(2, bloque); ///este es el bloque que necesita.
        }
      }
  }
  return suma;
}
