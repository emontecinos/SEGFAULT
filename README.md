**Struct rmFILE: Que debe tener?**
* size
* posición/bloque inicial
* posición/bloque final
* Que más (o menos)?

**Documentación**  [aqui](https://docs.google.com/document/d/1mAX-Yt7YYmibCu53DxbZXm0bplpG2ivJRbbj-PVcJCU/edit?usp=sharing)

**Funciones Proyecto**

*Funciones Generales*

```c
void cr_mount(char* diskname) OK
pone como var. global la ruta del archivo .bin del disco.

void cr_bitmap(unsigned disk, bool hex) Chelo
Imprime el bitmap del disco disk en formato binario o hex. debe poder imprimir el bitmap completo

int cr_exists(unsigned disk, char* filename) Jesu
1 si filename existe en disk, 0 eoc.

void cr_ls(unsigned disk) Jesu
Lista los elementos de la partición. Imprime los archivos de disk.

```
*Funciones de manejo de archivos*
```c
crFILE* cr_open(unsigned disk, char* filename, char mode) Jesu
Abre un archivo filename. si mode es r, busca en la ruta path y lo retorna; si es w, revisa que no esté en path y retorna un nuevo archivo que lo represente.

int cr_read (crFILE* file_desc, void* buffer, int nbytes) Jesu
Lee Archivos. desde archivo file_dect, lee nbytes hacia adelante y los guarda en la dir apuntada por buffer. Retorna la cant de bytes leidos. (nbytes puede ser mayor a los bytes restantes). Lee desde la ultima ultima posición leida por un llamado a la función.

int cr_write(crFILE* file_desc, void* buffer, int nbytes) Chelo
Escribe Archivos. Escribe en file_desc los nbytes de la dir buffer. Retorna la cantidad de bytes escritos (puede ser >= 0)

int cr_close(crFILE* file_desc) Chelo
Cierra archivos. Cuando retorne, el archivo debe quedar actualizado en disco.

int cr_rm(unsigned disk, char* filename) Eyza
Borra archivo referencuado por path del directorio respectivo. Los bloques se liberan si y solo si no tienen ninguna referencia.

int cr_hardlink(unsigned disk, char* orig, char* dest) Eyza
crea un hardlink del archivo orig en una nueva ruta dest, aumentando la cantidad de referencias dl archivo orig.

int cr_softlink(unsigned disk_orig, unsigned disk_dest, char* orig, char* dest) Eyza
Crea un softlink del archivo referenciado por orig en ruta dest (pueden ser particioens distintas)

int cr_unload(unsigned disk, char* orig, char* dest) Pili
Copia un archivo orig desde disk (o el disco completo si disk=0) hacia un nuevo archivo o ruta dest.

int cr_load(unsigned disk, char* orig) Pili
Copia un archivo o el contenido de una carpeta (orig) de disk. Si un archivo es muy grande para el disco se copia lo que cabe. Si es una carpeta, se copian los archivos (no las subcarpetas).

```
