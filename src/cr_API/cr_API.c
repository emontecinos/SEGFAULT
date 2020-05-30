#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h> 
#include <fcntl.h>
#include <math.h>
#include <dirent.h>
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

unsigned int get_free_block(unsigned int puntero_indice){
    int disk=0;
    int inicio=0;
    int final=0;
    if (puntero_indice < 65536){
        disk=1;
        inicio=0;
        final=65535;
    }else if(puntero_indice < 131072){
        disk=2;
        inicio=65536;
        final=131071;
    }else if(puntero_indice < 196608){
        disk=3;
        inicio=131072;
        final=196607;
    }else{
        disk=4;
        inicio=196608;
        final=262143;
    }
    int byte_leido=0;
    int aux_bit;
    unsigned int bloque_libre=-1;
    unsigned char aux_buffer;
    unsigned int bloque_inicio_rev=(disk-1)*pow(2,29)+pow(2,13);
    
    FILE* file = fopen(PATH,"rb");
    fseek(file,bloque_inicio_rev,SEEK_SET);//Bitmap de particion
    //fprintf(stderr,"inicio: %d,byte inicio rev %d, final; %d\n",inicio,bloque_inicio_rev, final);
    for (int i=0;i<65536;i++){
        byte_leido=fread(&aux_buffer,1,1,file);
        int valor_byte = (int) aux_buffer;
        //fprintf(stderr,"BYTE LEIDO: %d \n",valor_byte);
            
        if (valor_byte<255){
            int indice_bloque;
            //tiene algun 0 en alguna parte
            
            for(int j=7;j>=0;j--){
                indice_bloque=aux_buffer&(int)pow(2,j);
                if(indice_bloque==0){
                    bloque_libre=inicio+7-j;
                    fclose(file);
                    return bloque_libre;
                }
            }
        }
    }
    fclose(file);
    return bloque_libre;
}
int escribir_bloque_dir_simple(unsigned int dir_bloque_dir_simple, unsigned int bloque_a_escribir, crFILE*file_desc,FILE* file){
    int bloque = (int)(file_desc->size/8196)+1-2044;// bloque "entero" usado de dir simple
    unsigned char aux_buffer;
    unsigned dir_a_escribir;
    dir_a_escribir=(file_desc->puntero_a_bloque+8196-16);
    fseek(file,dir_a_escribir,SEEK_SET);
    fread(&aux_buffer,1,2,file);
    dir_a_escribir=aux_buffer+bloque;
    fseek(file,dir_a_escribir,SEEK_SET);
    fwrite(&bloque_a_escribir,2,1,file);
    return 1;
}
int actualizar_bitmap(unsigned int bloque_a_escribir,crFILE* file_desc,FILE* file){
    int disk=0;
    if (bloque_a_escribir < 65536){
        disk=1;
    }else if(bloque_a_escribir < 131072){
        disk=2;
    }else if(bloque_a_escribir < 196608){
        disk=3;
    }else{
        disk=4;
    }
    unsigned int byte_escritura;
    unsigned int byte_lectura;
    int byte_actualizado;
    byte_escritura=(disk-1)*pow(2,29)+pow(2,13) + (int)bloque_a_escribir/8;
    fseek(file,byte_escritura,SEEK_SET);
    fread(&byte_lectura,1,1,file);
    byte_actualizado=byte_lectura|(int)pow(2,bloque_a_escribir%8);
    fseek(file,-1,SEEK_CUR);
    fwrite(&byte_actualizado,1,1,file);
    return 1;
}
int escribir_bloque_indice(unsigned int bloque_a_escribir,crFILE*file_desc,FILE* file){
    int bloque = (int)(file_desc->size /8196)+1;// bloque "entero" usado
    unsigned int dir_a_escribir;
    dir_a_escribir=file_desc->puntero_a_bloque+12 + bloque;
    fseek(file,dir_a_escribir,SEEK_SET);
    // unsigned char* b_a_e;
    // b_a_e=(unsigned char *) bloque_a_escribir;
    fwrite(&bloque_a_escribir,2,1,file);
    return 1;
}
void actualizar_tamano_archivo(int bytes,crFILE*file_desc){
    file_desc->size+=bytes;
    return;
}

int cr_write(crFILE* file_desc, void* buffer, int nbytes){
    int n_max=0;
    if (file_desc->size+nbytes <=4092*pow(2,13))n_max=nbytes;
    else n_max=4092*pow(2,13)-file_desc->size;
    int cant_escribir_indice=0;
    int cant_escribir_dir_simple=0;
    //calcular espacio libre para archivo
    unsigned int espacio_libre_archivo=4092*pow(2,13)-file_desc->size;
    int byte_a_escribir=0;
    void **byte_buffer = malloc(sizeof(void *) * nbytes);

    //Ver si: cabe
    if (nbytes <= espacio_libre_archivo){
        if(file_desc->size>=2044*pow(2,13)){
            cant_escribir_indice=0;
        }else{
            cant_escribir_indice=2044*pow(2,13) - file_desc->size;
        }
        cant_escribir_dir_simple=nbytes-cant_escribir_indice;
    }else{
        //No cabe en el archivo
        if(file_desc->size>=2044*pow(2,13)){
            cant_escribir_indice=0;
        }else{
            cant_escribir_indice=2044*pow(2,13) - file_desc->size;
        }
        cant_escribir_dir_simple= (4092*pow(2,13))-file_desc->size;
    }
    //Seguir aqui
    FILE* file = fopen(PATH,"wb");
    //Cuanto escribir en cada parte
    //Si hay espacio para escribir en indice,
    unsigned int escritos_indice=0;
    if(cant_escribir_indice>0){
        //Escribir en indice
        //Bytes a escbirir
        
        while (escritos_indice<cant_escribir_indice){
            unsigned int bloque_a_escribir=get_free_block(file_desc->puntero_a_bloque);
            //Si puedo escribir
            if(bloque_a_escribir!=-1){
                //Escribir en cada byte del bloque
                fseek(file,bloque_a_escribir*8192,SEEK_SET);//Ponerse en byte a escribir, no en bloque
                for (int j=0;j<8192;j++){
                    //Escribir buf
                    char* a_escribir;
                    a_escribir=byte_buffer[byte_a_escribir];
                    fwrite(a_escribir,1,1,file);
                    byte_a_escribir++;
                    escritos_indice++;
                    if (byte_a_escribir>=nbytes){
                        break;//Termine de escribir el archivo
                    }
                }
                escribir_bloque_indice(bloque_a_escribir,file_desc,file);
                actualizar_bitmap(bloque_a_escribir,file_desc,file);
                //Escribir en bloque indice dir de bloque (bloque_a_escribir)
                //Actualizar bitmap
            }else{
                break;
            }
        }
    }
    unsigned int escritos_simple=0;
    if(cant_escribir_dir_simple>0){
        //Escribir en bloque dir simple
        unsigned int dir_bloque_dir_simple=file_desc->puntero_a_bloque+8188;//pos del bloque de dir simple; 8192-4
        while(escritos_simple<cant_escribir_dir_simple){
            unsigned int bloque_a_escribir=get_free_block(file_desc->puntero_a_bloque);//Dentro de la partición; rango de disk
            if(bloque_a_escribir!=-1){
                fseek(file,bloque_a_escribir*8192,SEEK_SET);
                //Escribir el bloque
                for(int j=0;j<8192;j++){
                    char* a_escribir;
                    a_escribir=byte_buffer[byte_a_escribir];
                    //char a_escribir[0]=buffer[byte_a_escribir];
                    fwrite(a_escribir,1,1,file);
                    byte_a_escribir++;
                    escritos_simple++;
                    if(byte_a_escribir>=nbytes){
                        break;
                    }
                }
                escribir_bloque_dir_simple(dir_bloque_dir_simple, bloque_a_escribir,file_desc,file);
                actualizar_bitmap(bloque_a_escribir,file_desc,file);
                //Escribir en bloque dir simple dir de bloque (bloque_a_escribir)
                //Actualizar bitmap
            }else{
                break;
            }
        }
    }
    //Si se escribe se actualiza
    if(escritos_simple+escritos_indice>0) actualizar_tamano_archivo(escritos_simple+escritos_indice,file_desc);

    free(byte_buffer);
    return escritos_indice+escritos_simple;
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
unsigned get_files(char** file_names, unsigned long* file_pointers, unsigned total, unsigned long disk_start)
{
    FILE* file = fopen("disk/simdiskfilled.bin","rb");
    unsigned int disk_chelo_start;
    disk_chelo_start=disk_start;
    fprintf(stderr,"\n\t Numeros\n\t%lu,%u\n",disk_start,disk_chelo_start);
    fseek(file, disk_chelo_start, SEEK_SET); 
    // fseek(file, disk_start, SEEK_SET);    

    unsigned char aux;
    unsigned char splited_pointer[3];
    unsigned long pointer = 0;
    int validity;
    char name[29];

    for (int i = 0; i < max_files; i++)
    {
        pointer  = 0;
        aux      = getc(file);
        validity = aux >> 7;
        splited_pointer[0] = aux & 127;  //127 = 01111111
        splited_pointer[1] = getc(file);
        splited_pointer[2] = getc(file);

        for (int j = 0; j < 29; j++){
            name[j] =  getc(file);}

        if (validity == 1)
        {   
            pointer = pointer | (unsigned long) splited_pointer[0] << 8*2;
            pointer = pointer | (unsigned long) splited_pointer[1] << 8*1;
            pointer = pointer | (unsigned long) splited_pointer[2];
            
            strcpy(file_names[i], name);
            file_pointers[i] = pointer;
            total++;
        }
    }

    fclose(file);
    return total;
}

void read_write_data_block(unsigned long pointer, FILE* file, FILE* file_copy, unsigned long used, unsigned long leftover, int offset)
{
    char byte;
    unsigned long bytes_used, data_pointer;

    printf("used %lu leftover %lu\n ", used, leftover);

    for (int i = 0; i < used; i++)
    {
        bytes_used   = block_size;
        data_pointer = 0;

        fseek(file, pointer * block_size + offset + 4*i, SEEK_SET);

        for (int j = 3; j > -1; j--){
            data_pointer = data_pointer | (unsigned long) getc(file) << 8*j;}

        fseek(file, data_pointer * block_size, SEEK_SET);
        
        if (i == used - 1 & leftover != 0){
            bytes_used =  leftover;}

        for (int j = 0; j < bytes_used; j++)
        {     
            byte = getc(file);
            fwrite(&byte, 1, sizeof(char), file_copy);
        }
    }
}

void write_file(char* name, unsigned long pointer, char* dest)
{
    FILE* file = fopen("disk/simdiskfilled.bin","rb");
    
    fseek(file, pointer * block_size + 4, SEEK_SET);    
    unsigned long size = 0;

    for (int i = 7; -1 < i; i--){
            size = size | (unsigned long) getc(file) << 8*i;}

    unsigned long module      = size % block_size;
    unsigned long leftover      = (module == 0) ? block_size : module;
    unsigned long blocks_used   = (module == 0) ? size / block_size : (size - module) / block_size + 1;
    unsigned long direct_used   = (blocks_used > 2044) ? 2044 : blocks_used;
    unsigned long indirect_used = (blocks_used - direct_used > 0) ? blocks_used - direct_used : 0;

    char dest_name[250];
    strcpy(dest_name, dest);
    strcat(dest_name, "/");
    strcat(dest_name, name);

    FILE* file_copy = fopen(dest_name,"wb");

    unsigned long leftover_direct = (direct_used == 2044) ? block_size : leftover;

    read_write_data_block(pointer, file, file_copy, direct_used, leftover_direct, 12);

    unsigned long indirect_pointer = 0;
    fseek(file, 8176, SEEK_CUR);

    for (int i = 3; i > -1; i--){
        indirect_pointer = indirect_pointer | (unsigned long) getc(file) << 8*i;}

    read_write_data_block(indirect_pointer, file, file_copy, indirect_used, leftover, 0);

    fclose(file_copy);
    fclose(file);
}

void transfer_files(unsigned disk_start, char* orig, char* dest)
{   
    char **file_names = malloc(max_files * sizeof(char*));
    for (int i = 0; i < max_files; i++){
        file_names[i] = malloc(29 * sizeof(char)); }
    
    unsigned long *file_pointers = malloc(max_files * sizeof(unsigned long));
    unsigned total = 0;
    total = get_files(file_names, file_pointers, total, disk_start);

    for (int i = 0; i < total; i++)
    {   
        if (orig == NULL){
            write_file(file_names[i], file_pointers[i], dest);}   

        else if (strcmp(orig, file_names[i]) == 0){
            write_file(file_names[i], file_pointers[i], dest);}    
    }

    for (int i = 0; i < max_files; i++){ 
        free(file_names[i]);}
    free(file_names);
    free(file_pointers);
}

int cr_unload(unsigned disk, char* orig, char* dest){
    if (disk != 0)
    {   
        unsigned long disk_start = (disk - 1) * n_blocks_part * block_size;
        transfer_files(disk_start, orig, dest);
    }

    else
    {   
        for (int i = 0; i < 4; i++){
            transfer_files(n_blocks_part * i, orig, dest);}
    }
    
    return 0;
}

// int cr_load(unsinged disk, char* orig). 
// Función que se encarga de copiar un archivo o los
// contenidos de una carpeta, referenciado por orig a la partición designada. 
// En caso de que un archivo sea
// demasiado pesado para el disco, se debe escribir todo lo posible hasta 
// acabar el espacio disponible. En caso de
// que el sea una carpeta, se deben copiar los archivos que estén dentro de 
// esta carpeta, ignorando cualquier carpeta
// adicional que tenga.

int is_file(char* string)
{   
    char dot = 46;
    int has_dot = 0, has_char = 0, i = 0;

    while(string[i] != '\0')
    {
        if(string[i] == dot){
            has_dot = 1;}

        else if (!has_dot){
            has_char = 1;}
        i++;
    }

    return (has_dot & has_char);
}

void load_write_file(unsigned disk, char* filename)
{
    FILE* file = fopen(filename, "rb");

    fseek(file, 0, SEEK_END);
    unsigned size = ftell(file);
    printf("size of %s is %u\n", filename, size);

    size = (size > max_file_size) ? max_file_size : size;
    fprintf(stderr,"imprimir");
    crFILE* cr_file = cr_open(disk, filename, 'w');
    fprintf(stderr,"imprimir1");

    unsigned char* buffer = malloc(block_size * sizeof(char));    
    
    unsigned bytes_used  = block_size;
    unsigned module      = size % block_size;
    unsigned leftover    = (module == 0) ? block_size : module;
    unsigned blocks_used = (module == 0) ? size / block_size : (size - module)/ block_size + 1;
    

    for (int i = 0; i < blocks_used; i++)
    {
        if (i == blocks_used - 1){
            bytes_used = leftover;}

        for (int j = 0; j < bytes_used; i++){
            buffer[j] = getc(file);}
        fprintf(stderr,"imprimir2");
        cr_write(cr_file, buffer, bytes_used);
    }    

    free(buffer);
    fclose(file);    
    cr_close(cr_file);

}

int cr_load(unsigned disk, char* orig){
    if (is_file(orig))
        {
            load_write_file(disk, orig);
        }

    else  // cógigo para leer directorios sacado de internet
    {
        DIR *dp;
        struct dirent *ep;     
        dp = opendir (orig);

        if (dp != NULL)
        {
            while (ep = readdir (dp))
            {
                if (is_file(ep -> d_name))
                {
                    char dir[80];
                    strcpy(dir, orig);
                    strcat(dir, ep -> d_name);
                    printf("%s\n", dir);
                    load_write_file(disk, dir);
                }
            }                
            (void) closedir (dp);

        }
        else
            perror ("Directorio incorrecto");

    }
    return 0;
}
