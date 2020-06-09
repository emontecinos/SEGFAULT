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
#include <dirent.h>

extern char* PATH;

int is_softlink_file(char* file_name){
char backslash = 47;
int has_backslash = 0, has_char = 0, i = 0;

while(file_name[i] != '\0')
{
if(file_name[i] == backslash){
has_backslash = 1;}

else if (!has_backslash){
has_char = 1;}
i++;
}

return (has_backslash & has_char);
}

uint64_t calculo_numero(unsigned char buffer[], int iii)
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

void cr_mount(char* diskname){
    PATH = diskname;
    return;
}
void cr_bitmap(unsigned disk, bool hex){
    int64_t valor=0;
    int* leer_disco(unsigned disk,bool hex, int* valores){
        unsigned char buffer[65536]; //#bloques en particion
        unsigned char aux_buffer=0;
        //int aux_buffer;
        size_t bytes_leidos=0;
        unsigned int byte_lectura=0;
        int bloques_ocupados=0;
        int bloques_desocupados=0;
        int aux_bytes_leidos=0;
        int aux_bit=0;
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
                aux_bit=aux_buffer&128;
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
                //fprintf(stderr,"%c",buffer[i]);
                //valor+=pow(2,i); endianness?
            }
            //fprintf(stderr,"\n");
        }else{
            int aux_valor=0;
            int resto;
            for(int i=0; i < 65536;i++){
                aux_valor+=(buffer[i]-48)*pow(2,3-(i%4));// Ver endianess, 2^resto o 2^i-resto
                if (i%4==0&&i!=0){
                    //fprintf(stderr,"%X",aux_valor);
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

int cr_exists(unsigned disk, char* filename)
{
    if (disk == 1 || disk == 2 || disk == 3 || disk == 4)
    {
      FILE *ptr;
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
    printf("Disco no existente\n");
    return 0;
  }
}


void cr_ls(unsigned disk){
    if (disk != 1 && disk != 2 && disk != 3 && disk != 4)
    {
      printf("Disco no existente\n");
      return;
    }
    unsigned char buffer[32];
    FILE *ptr;
    ptr = fopen(PATH,"rb");
    fseek(ptr, 32*256*65536*(disk - 1), SEEK_SET);

    for (int i = 0; i < 256; i++)
    {
      fread(buffer,sizeof(buffer),1,ptr);
        if(buffer[0] >= 115)
        {
          // uint32_t num, num4;
          // num4 = buffer[0] - 0x80;
          // num = num4 << 16 | buffer[1] << 8|buffer[2];
          // printf("puntero: %i\n", num);
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
crFILE* cr_open(unsigned disk, char* filename, char* mode)
  {
    if (disk != 1 && disk != 2 && disk != 3 && disk != 4)
      {
        printf("Disco no existente\n");
        return NULL;
      }

    if (strchr(filename, '/') != NULL)
    {
      char disco[1];
      strncpy(disco, filename, 1);
      printf("%c\n", disco[0]);
      char* nombre_link = filename + 2;
      printf("cont %s\n", nombre_link);

      crFILE* archivo;
      archivo = cr_open(atoi(&disco[0]), nombre_link, "r");
      return archivo;
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
      ptr = fopen(PATH, "rb+");
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
            unsigned char b = buff[0];
            for(int j = 0; j < 8; j++)
            {
                if (((b>>(7 - j))&1) == 0)
                {
                  if (i != 0 || j != 0)
                  {
                  bloque_indice = i*8 + j + inicio;
                  encontrado = 0;
                  uint32_t num2, num3;
                  num2 = pow(2, 7 - j);
                  num3 = num2 | buff[0];

                  fseek(ptr, 32*256*65536*(disk - 1) + 8192 + i, SEEK_SET);
                  unsigned long n2 = num3;
                  unsigned char bytes[1];
                  bytes[0] = n2 & 0xFF;
                  fwrite(&bytes[0], sizeof(bytes[0]), 1, ptr);
                  break;
                }
                }
            }
      }
      i++;
    }
      puntero = bloque_indice;
      referencias = 0;
      porte = 0;
      cantidad = 0;

      //Agregar a directorio.
      fseek(ptr, 32*256*65536*(disk - 1), SEEK_SET);
      unsigned char buffd[32];
      for (int i = 0; i <= 256; i++)
      {
        if (i == 256)
        {
          printf("No se pueden agregar mas archivos a disco\n");
          return NULL;
        }
        fread(buffd, sizeof(buffd), 1, ptr);
        uint32_t para_escribir;
        if (buffd[0] <= 0x7F)
        {
          int ocupado = 128;
          para_escribir = ocupado << 16 | puntero;

          fseek(ptr, 32*256*65536*(disk - 1) + i*32, SEEK_SET);
          unsigned char bytes[3];
          unsigned long n = para_escribir;

          bytes[0] = (n >> 16) & 0xFF;
          bytes[1] = (n >> 8) & 0xFF;
          bytes[2] = n & 0xFF;
          for (int g = 0; g < 3; g++)
          {
             fwrite(&bytes[g], sizeof(bytes[g]), 1, ptr);
          }
          for (int l = 0; filename[l]; l++)
          {
             fwrite(&filename[l], sizeof(filename[l]), 1, ptr);
          }
          break;
        }
      }
      fclose(ptr);
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
            // printf("puntero: %i\n", puntero);
            break;
          }
        }
      }
      // printf("puntero: %i\n", puntero);

      fseek(ptr, 32*256*puntero, SEEK_SET); //bloque indice
      ///referencias
      unsigned char buffer2[4];
      fread(buffer2, sizeof(buffer2), 1, ptr);

      referencias = buffer2[0] << 24 | buffer2[1] << 16|buffer2[2] << 8|buffer2[3];

      //tamaño
      unsigned char buffer3[8];
      fread(buffer3, sizeof(buffer3), 1, ptr);

      cantidad = calculo_numero(buffer3, 8);

      porte = cantidad /(256 * 32);
      if (porte * 256 * 32 != cantidad)
      {
        porte += 1;
      }
      // printf("porte: %i\n", porte);

      fclose(ptr);
    }

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
    crfile -> cant_hardlinks = referencias;
    crfile -> cant_bloques = porte;
    crfile -> modo = mode;
    crfile -> existe = existe;
    crfile -> byte_leido = 0;
    return crfile;
}

int cr_read (crFILE* file_desc, void* buffer, int nbytes){

    if (file_desc == NULL)
    {
      printf("Archivo no valido\n");
      return 0;
    }

    unsigned char* bufferr = (unsigned char*)buffer;

    int bloque;
    int byte_de_bloque;
    int bloque_para_leer;
    int bytes_leidos = 0;;
    int en_proceso = 1;
    bloque = (file_desc -> byte_leido) / (256 * 32);
    byte_de_bloque = (file_desc -> byte_leido) - (bloque * 32 * 256);
    // printf("byte_de_bloque: %i\n", byte_de_bloque);
    /////////Buscar el bloque indice
    FILE *ptr;
    ptr = fopen(PATH, "rb");
    while (en_proceso) {
      // printf("puntero: %i\n", file_desc -> puntero_a_bloque);
      fseek(ptr, 32*256*(file_desc -> puntero_a_bloque) + 12 + bloque * 4, SEEK_SET);
      ////// Buscar bloque para comenzar a leer
      unsigned char buffer_b[4];
      fread(buffer_b, sizeof(buffer_b), 1, ptr);
      bloque_para_leer = buffer_b[0] << 24 | buffer_b[1] << 16|buffer_b[2] << 8|buffer_b[3];
      ///////////
      fseek(ptr, 32*256*bloque_para_leer + byte_de_bloque, SEEK_SET);
      //leer
      unsigned char buff[1];
      //minimo entre nbytes, lo que queda de este archivo y lo que queda de bloque
      int i = 0;
      // printf("nbytes - bytes_leidos %i\n", nbytes - bytes_leidos);
      // printf("file_desc -> size - file_desc -> byte_leido - bytes_leidos %i\n", file_desc -> size - file_desc -> byte_leido - bytes_leidos);
      // printf("32*256 - byte_de_bloque %i\n", 32*256 - byte_de_bloque);
      for (i = 0; i < nbytes - bytes_leidos && i < file_desc -> size - file_desc -> byte_leido - bytes_leidos && i < 32*256 - byte_de_bloque; i++)
      {
          fread(buff, sizeof(buff), 1, ptr);
          bufferr[i + bytes_leidos] = buff[0];
          // printf("%c", buff[0]);
      }
      bytes_leidos += i;
      // printf("nbytes %i\n", nbytes);
      // printf("bytes_leidos = %i\n", bytes_leidos);
      file_desc -> byte_leido += bytes_leidos;
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
    return 0;
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
    unsigned char aux_buffer[1]="0";
    fprintf(stderr,"Puntero indeice: %d\n",puntero_indice);
    unsigned int bloque_inicio_rev=(disk-1)*pow(2,29)+pow(2,13);

    FILE* file = fopen(PATH,"rb");
    fseek(file,bloque_inicio_rev,SEEK_SET);//Bitmap de particion
    //fprintf(stderr,"inicio: %d,byte inicio rev %d, final; %d\n",inicio,bloque_inicio_rev, final);
    for (int i=0;i<65536;i++){
        byte_leido=fread(&aux_buffer,1,1,file);
        unsigned char valor_byte;
        valor_byte = (unsigned char) aux_buffer[0];
        fprintf(stderr,"BYTE LEIDO: %u \n",valor_byte);

        if (valor_byte<255){
            int indice_bloque=0;
            //tiene algun 0 en alguna parte
            for(int j=7;j>=0;j--){
                indice_bloque=((int) aux_buffer[0])&(int)pow(2,j);
                if(indice_bloque==0){
                    bloque_libre=inicio+7-j;
                    fclose(file);
                    //fprintf(stderr,"Bloque libre: %u\n",bloque_libre);
                    return bloque_libre;
                }
            }
        }
    }
    fclose(file);
    fprintf(stderr,"Bloque libre: %u\n",bloque_libre);
    return bloque_libre;
}

int escribir_bloque_dir_simple(unsigned int dir_bloque_dir_simple, unsigned int bloque_a_escribir, crFILE*file_desc,FILE* file){
    int bloque = (int)(file_desc->size/8192)+1-2044;// bloque "entero" usado de dir simple
    unsigned char aux_buffer;
    unsigned dir_a_escribir;
    dir_a_escribir=(file_desc->puntero_a_bloque+8192-16);
    fseek(file,dir_a_escribir,SEEK_SET);
    fread(&aux_buffer,1,2,file);
    dir_a_escribir=aux_buffer+bloque;
    fseek(file,dir_a_escribir,SEEK_SET);
    fwrite(&bloque_a_escribir,2,1,file);
    return 1;
}
int actualizar_bitmap(unsigned int bloque_a_escribir,FILE* file){
    unsigned int bloque_relativo=bloque_a_escribir;
    unsigned int bloque_bitmap;
    int disk=0;
    if (bloque_a_escribir < 65536){
        disk=1;
        bloque_relativo=bloque_a_escribir;
        bloque_bitmap=1;
    }else if(bloque_a_escribir < 131072){
        disk=2;
        bloque_relativo=bloque_a_escribir-65536;
        bloque_bitmap=65538;
    }else if(bloque_a_escribir < 196608){
        disk=3;
        bloque_relativo=bloque_a_escribir-131072;
        bloque_bitmap=131074;
    }else{
        disk=4;
        bloque_relativo=bloque_a_escribir-196608;
        bloque_bitmap=196610;
    }
    unsigned int byte_escritura;
    unsigned char byte_lectura;
    unsigned int saltos_bytes=bloque_relativo;
    unsigned int contador_saltos=1;
    while(saltos_bytes>7){
        saltos_bytes= saltos_bytes/8;
        contador_saltos++;
    }
    unsigned char byte_actualizado;
    fprintf(stderr,"Bloque a relativo: %u %u\n",bloque_relativo, contador_saltos);
    // byte_escritura=(disk-1)*pow(2,29)+pow(2,13) + (int)bloque_a_escribir/8;
    byte_escritura=(disk-1)*pow(2,29)+pow(2,13) + contador_saltos;
    fseek(file,byte_escritura,SEEK_SET);
    fread(&byte_lectura,1,1,file);
    // fprintf(stderr,"numero %u\n",bloque_a_escribir%8);
    // fprintf(stderr,"byte_old %u\n",byte_lectura);
    byte_actualizado=byte_lectura|(int)pow(2,7-bloque_a_escribir%8);
    // fprintf(stderr,"byte actualziado %u\n",byte_actualizado);
    fseek(file,-1,SEEK_CUR);
    fwrite(&byte_actualizado,1,1,file);
    return 1;
}
int escribir_bloque_indice(unsigned int bloque_a_escribir,crFILE*file_desc,FILE* file){
    unsigned int bloque = (int)(file_desc->size /8192)+1;// bloque "entero" usado
    unsigned int dir_a_escribir;
    unsigned char aux_escritura[4];
    dir_a_escribir=(file_desc->puntero_a_bloque+bloque)*pow(2,13);
    fseek(file,dir_a_escribir,SEEK_SET);
    for(int i=0; i < 4; i++){
      aux_escritura[i]=(bloque_a_escribir>>(8*(3-i))&255);
      fprintf(stderr," Byte escrito dir : %u\n",aux_escritura[i]);
      fwrite(&aux_escritura[i],1,sizeof(aux_escritura),file);
    }
    fprintf(stderr,"dir a escribir: %u\nBloque a escribir: %u\n",dir_a_escribir,bloque_a_escribir);
    fseek(file,-4,SEEK_CUR);
    for(int i=0;i<4;i++){
      fread(&aux_escritura[0],1,1,file);
      fprintf(stderr,"dir bloque scrita: %u\n",aux_escritura[0]);
    }

    return 1;
}
void actualizar_tamano_archivo(unsigned long bytes,crFILE*file_desc, FILE* file){
    file_desc->size+=bytes;
    unsigned char byte_aux;
    unsigned char byte_esc[8];
    fprintf(stderr,"Tamano archivo :%lu\n",bytes);
    fseek(file,(file_desc->puntero_a_bloque*8192)+4,SEEK_SET);//Bloque indice
    for (int i=7;i>-1;i--){
      byte_aux=bytes>>(i*8)&255;
      byte_esc[i]=byte_aux;

      fprintf(stderr,"----NUmuero1: %u\n",byte_aux);
      fwrite(&byte_aux,1,1,file);
      fseek(file,-1,SEEK_CUR);
      fread(&byte_aux,1,1,file);
      fprintf(stderr,"----NUmuero2: %u\n",byte_aux);
    }


    return;
}

void escribir_dir(unsigned int puntero,FILE* ptr,char* filename){
    int disk=0;
    if (puntero < 65536){
        disk=1;
    }else if(puntero < 131072){
        disk=2;
    }else if(puntero < 196608){
        disk=3;
    }else{
        disk=4;
    }
    fseek(ptr, 32*256*65536*(disk-1), SEEK_SET);
      unsigned char buffd[32];
      for (int i = 0; i <= 256; i++)
      {
        if (i == 256)
        {
          printf("No se pueden agregar mas archivos a disco\n");
          return;
        }
        fread(buffd, sizeof(buffd), 1, ptr);
        uint32_t para_escribir;
        if (buffd[0] <= 0x7F)
        {
          int ocupado = 128;
          para_escribir = ocupado << 16 | puntero;

          fseek(ptr, 32*256*65536*(disk-1) + i*32, SEEK_SET);
          //fwrite('128', sizeof(int), 1, ptr);
          unsigned char bytes[3];
          unsigned long n = para_escribir;

          bytes[0] = (n >> 16) & 0xFF;
          bytes[1] = (n >> 8) & 0xFF;
          bytes[2] = n & 0xFF;
          for (int g = 0; g < 3; g++)
          {
             fwrite(&bytes[g], sizeof(bytes[g]), 1, ptr);
          }
          for (int l = 0; filename[l]; l++)
          {
            fwrite(&filename[l], sizeof(filename[l]), 1, ptr);
          }
          break;
        }
      }
    return;


}
int cr_write(crFILE* file_desc, char* buffer, int nbytes){
    int n_max=0;
    if (file_desc->size+nbytes <=4092*pow(2,13)){
        n_max=nbytes;
    }else{
        n_max=4092*pow(2,13)-file_desc->size;
    }
    int cant_escribir_indice=0;
    int cant_escribir_dir_simple=0;
    for(int j =0;j<10;j++){
      fprintf(stderr,"J: %c\n",buffer[j]);
    }
    //calcular espacio libre para archivo
    unsigned int espacio_libre_archivo=4092*pow(2,13)-file_desc->size;
    int byte_a_escribir=0;
    //void **byte_buffer = malloc(sizeof(void *)*nbytes);

    //Ver si: cabe
    if (nbytes <= espacio_libre_archivo){
        if(file_desc->size>=2044*pow(2,13)){
            cant_escribir_indice=0;
        }else{
            //cant_escribir_indice=2044*pow(2,13) - file_desc->size;
            cant_escribir_indice=nbytes;
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
    fprintf(stderr,"Cantidad simple: %u\n Cantidad indice: %d\n",cant_escribir_dir_simple,cant_escribir_indice);
    FILE* file = fopen(PATH,"rb+");
    //Cuanto escribir en cada parte
    //Si hay espacio para escribir en indice,
    unsigned long escritos_indice=0;
    if(cant_escribir_indice>0){
        //Escribir en indice
        fseek(file,file_desc->puntero_a_bloque*8192,SEEK_SET);
        unsigned int valor;
        valor=1;
        unsigned char bytes[4];
        bytes[0] = 0;
        bytes[1] = 0;
        bytes[2] = 0;
        bytes[3] = 1;
        for(int k=0;k<4;k++){
          fwrite(&bytes[k],1,1,file);
        }


        //Bytes a escbirir

        while (escritos_indice<cant_escribir_indice){
            unsigned int bloque_a_escribir=0;
            bloque_a_escribir=get_free_block(file_desc->puntero_a_bloque);
            // fprintf(stderr,"Bloque a escribir: %d\n",bloque_a_escribir);
            fprintf(stderr,"Bloque libre: %u\n",bloque_a_escribir);
            //Si puedo escribir
            if(bloque_a_escribir!=-1){
                //Escribir en cada byte del bloque
                fseek(file,bloque_a_escribir*8192,SEEK_SET);//Ponerse en byte a escribir, no en bloque
                for (int j=0;j<8192;j++){
                    //Escribir buf
                    unsigned char a_escribir;
                    a_escribir=buffer[byte_a_escribir];
                    fprintf(stderr,"A escribir, %c\n",a_escribir);
                    //fprintf(stderr,"escribir indice: %dEscritos indice %d\n",cant_escribir_indice,escritos_indice);
                    fwrite(&a_escribir,1,1,file);
                    fseek(file,-1,SEEK_CUR);
                    fread(&a_escribir,1,1,file);
                    fprintf(stderr,"escrito, %c\n",a_escribir);
                    // fseek(file,1,SEEK_CUR);
                    byte_a_escribir++;
                    escritos_indice++;
                    if (byte_a_escribir>=nbytes){
                        //fprintf(stderr,"i ciclo%d\n",j);
                        break;//Termine de escribir el archivo
                    }
                }
                escribir_bloque_indice(bloque_a_escribir,file_desc,file);
                actualizar_bitmap(bloque_a_escribir,file);
                //Escribir en bloque indice dir de bloque (bloque_a_escribir)
                //Actualizar bitmap
            }else{
                break;
            }
        }
    }
    unsigned long escritos_simple=0;
    if(cant_escribir_dir_simple>0){
        //Escribir en bloque dir simple
        unsigned int dir_bloque_dir_simple=file_desc->puntero_a_bloque+8188;//pos del bloque de dir simple; 8192-4
        while(escritos_simple<cant_escribir_dir_simple){
            unsigned int bloque_a_escribir=get_free_block(file_desc->puntero_a_bloque);//Dentro de la partición; rango de disk
            if(bloque_a_escribir!=-1){
                fseek(file,bloque_a_escribir*8192,SEEK_SET);
                //Escribir el bloque
                for(int j=0;j<8192;j++){
                    unsigned char a_escribir;
                    a_escribir=buffer[byte_a_escribir];
                    //char a_escribir[0]=buffer[byte_a_escribir];
                    fprintf(stderr,"649a escribir: %c\n",a_escribir);
                    fwrite(&a_escribir,1,1,file);
                    byte_a_escribir++;
                    escritos_simple++;
                    if(byte_a_escribir>=nbytes){
                        break;
                    }
                }
                escribir_bloque_dir_simple(dir_bloque_dir_simple, bloque_a_escribir,file_desc,file);
                actualizar_bitmap(bloque_a_escribir,file);
                //Escribir en bloque dir simple dir de bloque (bloque_a_escribir)
                //Actualizar bitmap
            }else{
                break;
            }
        }
    }
    //Si se escribe se actualiza
    if(escritos_simple+escritos_indice>0) {
        actualizar_tamano_archivo(escritos_simple+escritos_indice,file_desc, file);
        escribir_dir(file_desc->puntero_a_bloque,file,file_desc->nombre);
    }

    //free(byte_buffer);
    fclose(file);
    return escritos_indice+escritos_simple;
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
    ptr = fopen(PATH, "rb");
    fseek(ptr, (disk-1)*32*256*65536, SEEK_SET);
    int aux;
    int puntero;
    int aux_puntero_vaciar = 0;
    uint32_t puntero_vaciar = (disk-1)*32*256*65536;
    unsigned char nombre[29];
    unsigned char buffer[32];
    unsigned char buffer_destino[3];
    for (int i = 0; i < 256; i++){
      fread(buffer, sizeof(buffer), 1, ptr);
      if(buffer[0] > 0x7f){
        memcpy(nombre, &buffer[3], 29 * sizeof(unsigned char));
        int j;
        for (j = 0; nombre[j] || filename[j]; j++){
          aux = 1;
          if (nombre[j] != filename[j]){
            aux = 0;
            break;
          }
        }
        if (aux == 1){
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
    if (is_softlink_file(filename) == 0){
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
        byte_escritura=((int)puntero - (disk-1)*65536)/8 + (disk-1)*(int)pow(2,29)+(int)pow(2,13);
        fseek(ptr,byte_escritura,SEEK_SET);
        fread(&byte_lectura,1,1,ptr);
        byte_actualizado=(int)byte_lectura & (255 - (int)pow(2, 7 - (int)(puntero - (disk-1)*65536) % 8));
        fseek(ptr,-1,SEEK_CUR);
        fwrite(&byte_actualizado,1,1,ptr);
        punto += 4;
        unsigned char tamano[8];
        fseek(ptr, punto ,SEEK_SET);
        fread(tamano, sizeof(tamano), 1, ptr);
        uint64_t num2 = calculo_numero(tamano, 8);
        int porte;
        porte = num2/(8192);
        punto += 8;
        if (porte * 8192 != num2){
          porte += 1;
        }
        printf("El disco tiene %d bloques\n", porte);
        int i;
        if(porte <= 2044 ){
          for (i = 0; i < porte; i++){
            unsigned char bit_a_borrar[4];
            fseek(ptr, punto, SEEK_SET);
            fread(bit_a_borrar, sizeof(bit_a_borrar), 1, ptr);
            int n_bit_a_borrar = (int)bit_a_borrar[0]<<24| (int)bit_a_borrar[1]<<16 | (int)bit_a_borrar[2]<<8 |(int)bit_a_borrar[3];
            byte_escritura=(n_bit_a_borrar - (disk-1)*(int)pow(2,16))/8 + (disk-1)*(int)pow(2,29)+(int)pow(2,13);
            fseek(ptr,byte_escritura,SEEK_SET);
            fread(&byte_lectura,1,1,ptr);
            unsigned char  int_qliao[1];
            int_qliao[0] = (255 -(int)pow(2, 7 - (int)((n_bit_a_borrar - (disk-1)*(int)pow(2,16)) % 8)));
            byte_actualizado= byte_lectura & int_qliao[0];
            fseek(ptr,byte_escritura,SEEK_SET);
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
            byte_escritura=(n_bit_a_borrar - (disk-1)*(int)pow(2,16))/8 + (disk-1)*(int)pow(2,29)+(int)pow(2,13);
            fseek(ptr,byte_escritura,SEEK_SET);
            fread(&byte_lectura,1,1,ptr);
            unsigned char  intq[1];
            intq[0] = (255 -(int)pow(2, 7 - (int)((n_bit_a_borrar - (disk-1)*(int)pow(2,16)) % 8)));
            byte_actualizado= byte_lectura & intq[0];
            fseek(ptr,byte_escritura,SEEK_SET);
            fwrite(&byte_actualizado,1,1,ptr);
            punto += 4;
          }
          fseek(ptr, punto, SEEK_SET);
          unsigned char indireccionamiento[4];
          fread(indireccionamiento, sizeof(indireccionamiento), 1, ptr);
          uint32_t n_indireccionamiento = (int)indireccionamiento[0]<<24| (int)indireccionamiento[1]<<16
          | (int)indireccionamiento[2]<<8 |(int)indireccionamiento[3];
          for (i = 0; i < porte - 2044; i++){
            unsigned char bit_a_borrar[4];
            fseek(ptr, n_indireccionamiento, SEEK_SET);
            fread(bit_a_borrar, sizeof(bit_a_borrar), 1, ptr);
            int n_bit_a_borrar = (int)bit_a_borrar[0]<<24| (int)bit_a_borrar[1]<<16 | (int)bit_a_borrar[2]<<8 |(int)bit_a_borrar[3];
            byte_escritura=(n_bit_a_borrar - (disk-1)*(int)pow(2,16))/8 + (disk-1)*(int)pow(2,29)+(int)pow(2,13);
            fseek(ptr,byte_escritura,SEEK_SET);
            fread(&byte_lectura,1,1,ptr);
            byte_actualizado=byte_lectura & (255 -(int)pow(2, 7 - (int)(n_bit_a_borrar - (disk-1)*(int)pow(2,16)) % 8));
            fseek(ptr,-1,SEEK_CUR);
            fwrite(&byte_actualizado,1,1,ptr);
            n_indireccionamiento += 4;
          }
        }
      }
      fclose(ptr);
      return 1;
    }
    else{
      fclose(ptr);
      return 1;
    }
  }
  else{
    printf("El archivo %s no existe.\n", filename);
    return 0;
  }
}
int cr_hardlink(unsigned disk, char* orig, char* dest){
  if(cr_exists(disk, orig)){
    printf("%s\n", orig );
    printf("True\n");
    FILE *ptr;
    if (strchr(orig, '/') != 0)
    {
    printf("El archivo %s es un softlink\n", orig);
    return 0;
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
    for (int i = 0; i < 256; i++){
      fread(buffer, sizeof(buffer), 1, ptr);
      if(buffer[0] > 0x7f)
      {
        memcpy(nombre, &buffer[3], 29 * sizeof(unsigned char));
        int j;
        for (j = 0; nombre[j] || orig[j]; j++){
          aux = 1;
          if (nombre[j] != orig[j]){
            aux = 0;
            break;
          }
        }
        if (aux == 1){
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
      return 0;
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
    return 0;
  }
}
int cr_softlink(unsigned disk_orig, unsigned disk_dest, char* orig, char* dest){
  if(orig != dest){
    printf("El archivo %s es distinto de %s\n", orig, dest);
    return 0;
  }
  else if (strchr(orig, '/') != NULL)
  {
  printf("El archivo %s es un softlink\n", orig);
  return 0;
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
    return 0;
  }
}
unsigned get_files(char** file_names, unsigned long* file_pointers, unsigned total, unsigned long disk_start)
{
    FILE* file = fopen(PATH,"rb");
    fseek(file, disk_start, SEEK_SET);

    unsigned char aux;
    unsigned char splited_pointer[3];
    unsigned long pointer = 0;
    int validity;
    char name[29];

    // Por que?
    for (int i = 0; i < max_files; i++)
    {
        pointer  = 0;
        aux      = getc(file);
        validity = aux >> 7;
        splited_pointer[0] = aux & 127;  //127 = 01111111
        splited_pointer[1] = getc(file);
        splited_pointer[2] = getc(file);
        for (int j = 0; j<29; j++){
          name[j] =  getc(file);

          //Ciclo se ejecuta 8192/32 * 29 veces
          // Por que?
          // quizas con un fseek en el byte inicial del nombre
        }
      if (validity == 1)
      {
          pointer = pointer | (unsigned long) splited_pointer[0] << 8*2;
          pointer = pointer | (unsigned long) splited_pointer[1] << 8*1;
          pointer = pointer | (unsigned long) splited_pointer[2];

          strcpy(file_names[i], name);
          fprintf(stderr,"1042, file names: %s\n",file_names[i]);
          file_pointers[i] = pointer;
          total++;

      }
    }
    fclose(file);
    fprintf(stderr,"1056 TOTAL: %d\n",total);
    return total;
}

void read_write_data_block(unsigned long pointer, FILE* file, FILE* file_copy, unsigned long used, unsigned long leftover, int offset)
{
    unsigned char byte;
    unsigned long bytes_used, data_pointer;
    //printf("used %lu leftover %lu\n ", used, leftover);

    for (int i = 0; i < used; i++)
    {
      fprintf(stderr,"Senal9, i: %d, used: %ld\n",i, used);
        bytes_used   = block_size;
        data_pointer = 0;

        fseek(file, pointer * block_size + offset + 4*i, SEEK_SET);

        for (int j = 3; j > -1; j--){
            data_pointer = data_pointer | (unsigned long) getc(file) << 8*j;}

        fseek(file, data_pointer * block_size, SEEK_SET);
        fprintf(stderr,"--Bloque usado: %lu\n",data_pointer);

        if (i == used - 1 & leftover != 0){
            bytes_used =  leftover;}
        fprintf(stderr,"byte used: %ld\n",bytes_used);
        for (int j = 0; j < bytes_used; j++)
        {
            byte = getc(file);
            //fprintf(stderr,"char a escribir en archivo: %u\n",byte);//4294967295
            fwrite(&byte, 1, sizeof(char), file_copy);
        }
    }
    fprintf(stderr,"Senal10\n");
}

void write_file(char* name, unsigned long pointer, char* dest)
{
    FILE* file = fopen(PATH,"rb");
    fprintf(stderr,"Senal5\n");
    fseek(file, pointer * block_size + 4, SEEK_SET);
    unsigned long size = 0;
    unsigned int tamano_aux;
    for (int i = 7; -1 < i; i--){
            tamano_aux=getc(file);
            fprintf(stderr,"Tamanoooo: %u\n",tamano_aux);
            size = size | (unsigned long) tamano_aux << 8*i;}// Saca el siguiente char de file y
            //lo mueve en bloques de un byte para crear un numero que tiene los 8 primeros chars de file unidos. Por que

    unsigned long module      = size % block_size;
    unsigned long leftover      = (module == 0) ? block_size : module;
    unsigned long blocks_used   = (module == 0) ? size / block_size : (size - module) / block_size + 1;
    unsigned long direct_used   = (blocks_used > 2044) ? 2044 : blocks_used;
    unsigned long indirect_used = (blocks_used - direct_used > 0) ? blocks_used - direct_used : 0;
    fprintf(stderr,"size: %lu\n",size);
    char dest_name[250];
    strcpy(dest_name, dest);

    strcat(dest_name, name);

    FILE* file_copy = fopen(dest_name,"wb");
    fprintf(stderr,"Senal6\n");

    unsigned long leftover_direct = (direct_used == 2044) ? block_size : leftover;
    fprintf(stderr,"Senal7\n");
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
            if (!is_softlink_file(file_names[i])){
                write_file(file_names[i], file_pointers[i], dest);}
        }

        else if (strcmp(orig, file_names[i]) == 0){
            if (!is_softlink_file(file_names[i])){
                write_file(file_names[i], file_pointers[i], dest);}
        }
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
        fprintf(stderr,"Senal1");
        transfer_files(disk_start, orig, dest);
    }

    else
    {
        for (int i = 0; i < 4; i++){
            transfer_files(n_blocks_part * i, orig, dest);}
    }

    return 0;
}
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
    fseek(file, 0, SEEK_SET);
    printf("size of %s is %u\n", filename, size);

    size = (size > max_file_size) ? max_file_size : size;

    crFILE* cr_file = cr_open(disk, filename, "w");
    //Nombre ok

    char* buffer = malloc(block_size * sizeof(char));

    unsigned bytes_used  = block_size;
    unsigned module      = size % block_size;
    unsigned leftover    = (module == 0) ? block_size : module;
    unsigned blocks_used = (module == 0) ? size / block_size : (size - module)/ block_size + 1;


    for (int i = 0; i < blocks_used; i++)
    {
        if (i == blocks_used - 1){
            bytes_used = leftover;}
        for (int j = 0; j < bytes_used; j++){
            unsigned char f;
            f = getc(file);
            buffer[j]=f;

            //fprintf(stderr,"Char_a_escribir: %c\n",buffer[j]);
            }

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
                    printf("1242 %s\n", dir);
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
