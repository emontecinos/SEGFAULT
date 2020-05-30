#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../cr_API/cr_API.h"

extern char* PATH;
int main(int argument_count, char** arguments){
    // FILE* file = fopen("disk/simdiskfilled.bin","rb");
    // fseek(file, 0, SEEK_SET);    
    // unsigned long int lala;
    // unsigned long int punt[6];

    // for (int j = 0; j < block_size/32; j++)
    // {
    //     unsigned char a= getc(file);
    //     int validez = a >> 7;
    //     unsigned char puntero[3];
    //     puntero[0] = a & 127;
    //     puntero[1] = getc(file);
    //     puntero[2] = getc(file);

    //         char name[29];
    //         for (int i = 0; i < 29; i++){
    //             name[i] =  getc(file);
    //         }
    //         if (validez)
    //         {
    //             unsigned long int aa = (unsigned long) puntero[0] << 16 |(unsigned long) puntero[1] << 8 | puntero[2];
    //             printf("validez %i, nombre %s\n", validez, name);
    //             printf("puntero: %lu\n", aa);
    //             punt[j] = aa;
    //         }
    // }
    // for (int k = 0; k < 6; k++)
    // {
    //     fseek(file, punt[k]*block_size+4, SEEK_SET);    
    //     unsigned long int size = 0;
    //     for (int i = 0; i < 8; i++)
    //     {
    //         unsigned int size_ = getc(file);
    //         printf("n %u\n", (unsigned int)size_);
    //         size = size | (unsigned long) size_ << (7-i)*8;
    //     }
    
    // printf("tamaño %lu\n\n", size);
    // }

    // fclose(file);

    // cr_unload(4, "1/Baroque.mp3", "/home/pilar/Desktop");

    cr_load(1, "/home/pilar/Desktop/lala.txt");


    // Modo de uso 
    // ./crfs simdiskfilled.bin
    // printf("%s\n",ROUTE);

    // cr_mount(arguments[1]);

    // printf("%s\n",PATH);

    // cr_bitmap(1, 1);

    // cr_exists(ROUTE, char* filename);

    // cr_ls(ROUTE);

    // cr_open(ROUTE, char* filename, char mode);

    // cr_read (crFILE* file_desc, void* buffer, int nbytes);

    // cr_write(crFILE* file_desc, void* buffer, int nbytes);

    // cr_close(crFILE* file_desc);

    // cr_rm(ROUTE, char* filename);

    // cr_hardlink(ROUTE, char* orig, char* dest);

    // cr_softlink(unsigned disk_orig, unsigned disk_dest, char* orig, char* dest);

    // cr_unload(ROUTE, char* orig, char* dest);


    

    return 1;
}