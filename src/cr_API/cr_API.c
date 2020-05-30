#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h> 
#include <fcntl.h>
#include <math.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include "cr_API.h"


extern char* PATH;



void cr_mount(char* diskname){
    PATH = diskname;
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
    return 1;
}
int cr_softlink(unsigned disk_orig, unsigned disk_dest, char* orig, char* dest){
    return 1;
}


unsigned get_files(char** file_names, unsigned long* file_pointers, unsigned total, unsigned long disk_start)
{
    FILE* file = fopen("disk/simdiskfilled.bin","rb");
    fseek(file, disk_start, SEEK_SET);    

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

    crFILE* cr_file = cr_open(disk, filename, "w");

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
