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
    return 1;
}
void cr_ls(unsigned disk){
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
    return 1;
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
    for (int i = 0; i < 256; i++)
    {
      unsigned char buffer[32];
      fread(buffer, sizeof(buffer), 1, ptr);
      printf("%d\n",buffer[0] );
      printf("%c\n", buffer[3] );
      if(buffer[0] > 0x7f)
      {
        unsigned char nombre[29];
        memcpy(nombre, &buffer[3], 29 * sizeof(unsigned char));
        printf("%c\n", nombre[0] );
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
          printf("Puntero: %s, Nombre: %s\n", buffer, nombre );

        }
      }
    }
    fclose(ptr);
    return 0;
  }
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
