#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h> 
#include <fcntl.h>
#include <math.h>
#include "cr_API.h"

extern char* PATH;

void cr_mount(char* diskname){
    PATH = diskname;
    return;
}
void cr_bitmap(unsigned disk, bool hex){
    int64_t valor=0;
    int* leer_disco(unsigned disk,bool hex, int* valores){

        unsigned char buffer[65536]; //#bloques en particion
        unsigned char aux_buffer;
        //int aux_buffer;
        size_t bytes_leidos=0;
        unsigned int byte_lectura;
        int bloques_ocupados=0;
        int bloques_desocupados=0;
        int aux_bytes_leidos=0;
        int aux_bit;
        int pos_buffer=0;
        FILE* file = fopen(PATH,"rb");
        byte_lectura=(disk-1)*pow(2,29)+pow(2,13); // tamano particion + un bloque
        // lectura en bytes
        fseek(file,byte_lectura,SEEK_SET); // puntero lectura desde posicion byte_lectura desde el inicio (SEEK_SET)
        while(bytes_leidos < 8192){
            aux_bytes_leidos=fread(&aux_buffer,1,1,file); // byte que leo
            //printf("BYTE: %ld, %u\n",bytes_leidos, aux_buffer);
            //aux_bytes_leidos=fread(&aux_buffer,1,1,file);
            for (int i =0; i < 8; i++){
                aux_bit= aux_buffer&128;
                aux_bit=aux_bit>>=7;
                //aux_bit= aux_buffer&1; // and entre el byte leido y 1; al shiftearlo voy bit por bit.
                //fprintf(stderr,"%d\n",aux_bit);
                
                if(aux_bit==1){ // aumento el contador
                    bloques_ocupados++;
                }else {
                    bloques_desocupados++;
                    }
                buffer[pos_buffer]=aux_bit+'0';
                aux_buffer=aux_buffer<<1;
                //aux_buffer>>=1;
                // aux_buffer[0]>>=1; //shift right en 1.
                //aux_bit++;
                pos_buffer++;
            }
            bytes_leidos += 1;
        }
        fclose(file);
        if(hex==0){
            for(int i=0; i < 65536;i++){
                fprintf(stderr,"%c",buffer[i]);
                //valor+=pow(2,i); endianness?
            }
            //fprintf(stderr,"\n");
        }else{
            int aux_valor=0;
            int resto;
            for(int i=0; i < 65536;i++){
                aux_valor+=(buffer[i]-48)*pow(2,3-(i%4));// Ver endianess, 2^resto o 2^i-resto
                if (i%4==0&&i!=0){
                    fprintf(stderr,"%X",aux_valor); 
                    valor+= aux_valor;
                    aux_valor=0;
                }
            }
        }
        valores[0]=bloques_ocupados;
        valores[1]=bloques_desocupados;
        return valores;
    }
    
    if(disk >0){
        int arr[2];
        int* valores=leer_disco(disk,hex,arr);
        fprintf(stderr,"disco: %d\nOcupados: %d Libres: %d\n",disk,valores[0],valores[1]);
    }else{
        int bloques_ocupados[4];
        int bloques_desocupados[4];
        int arr[2];
        for (int i=1;i<5;i++){
            int* valores =leer_disco(i,hex,arr);
            bloques_ocupados[i-1]=valores[0];
            bloques_desocupados[i-1]=valores[1];
        }
        for (int i=1;i<5;i++){
            fprintf(stderr,"disco: %d\nOcupados: %d Libres: %d\n",i,bloques_ocupados[i-1],bloques_desocupados[i-1]);
        }
    }
    // printf("\nValor, %llX\n%lld\n",valor,valor);
    
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
    int n_max=0;
    if (file_desc->size+nbytes <=file_desc->max_size_bytes)n_max=nbytes;
    else n_max=file_desc->max_size_bytes-file_desc->size;
    for(int i=0;i<n_max;i++){
        file_desc->bytes[file_desc->size]=buffer[i];//file_desc->pos_en_buffer quizas sirve
        size++;
    }
    return n_max;
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
