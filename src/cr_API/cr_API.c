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

int calculo_numero(unsigned char buffer[], int iii);

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
      //memmove(contents, contents+1, 4);
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

      //tamaño
      unsigned char buffer3[8];
      fread(buffer3, sizeof(buffer3), 1, ptr);

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
int actualizar_bitmap(unsigned int bloque_a_escribir,FILE* file){
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
    // byte_escritura=(disk-1)*pow(2,29)+pow(2,13) + (int)bloque_a_escribir/8;
    byte_escritura=bloque_a_escribir-(disk-1);
    byte_escritura=(int)(bloque_a_escribir-((disk-1)*pow(2,16)+1))*pow(2,13);
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
    if (file_desc->size+nbytes <=4092*pow(2,13)){
        n_max=nbytes;
    }else{
        n_max=4092*pow(2,13)-file_desc->size;
    }
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
                actualizar_bitmap(bloque_a_escribir,file);
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
                actualizar_bitmap(bloque_a_escribir,file);
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
      if (porte * 8192 != num2)
      {
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
