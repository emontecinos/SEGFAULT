#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "cr_API.h"
#include <string.h>

extern char* PATH;

void cr_mount(char* diskname){
    PATH = diskname;
    return;
}
void cr_bitmap(unsigned disk, bool hex){
    return;
}
int cr_exists(unsigned disk, char* filename){

    // printf("%u\n", disk);
    // printf("%s\n", filename);
    // printf("..............\n");
    if (disk == 1 || disk == 2 || disk == 3 || disk == 4)
    {
      FILE *ptr; ///Cachar que onda con esto, se deberia abrir con el path?
      if (strchr(filename, '/') != NULL)
      {
      printf("aaaaaa\n");
      }

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
