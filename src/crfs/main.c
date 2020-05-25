#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../cr_API/cr_API.h"

int main(int argument_count, char** arguments){
    // Modo de uso 
    // ./crfs simdisk.bin

    char* ROUTE= arguments[1];
    printf("%s\n",ROUTE);

    return 1;
}