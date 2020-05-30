#pragma once 

static unsigned      block_size     = 8192;
static unsigned      max_files      = 8192 / 32;
static unsigned      max_file_size  = 8192 * (2048 + 2044);
static unsigned long partition_size = 536870912;
static unsigned long n_blocks_part  = 65536;
static unsigned long partitions[4]  = {0, 65536, 131072, 196608};

typedef struct crFILE
{
    int size;
    int* bytes;
    char* nombre;
    int puntero_a_bloque;
    int cant_hardlinks;
    char* modo;
    int max_size_bytes;
    int pos_en_array;

}crFILE;

char* PATH;

void cr_mount(char* diskname);
void cr_bitmap(unsigned disk, bool hex);
int cr_exists(unsigned disk, char* filename);
int cr_write(crFILE* file_desc, void* buffer, int nbytes);
int cr_close(crFILE* file_desc);


void cr_ls(unsigned disk);
crFILE* cr_open(unsigned disk, char* filename, char mode);
int cr_read (crFILE* file_desc, void* buffer, int nbytes);


int cr_rm(unsigned disk, char* filename);
int cr_hardlink(unsigned disk, char* orig, char* dest);
int cr_softlink(unsigned disk_orig, unsigned disk_dest, char* orig, char* dest);

int cr_unload(unsigned disk, char* orig, char* dest);
int cr_load(unsigned disk, char* orig);

