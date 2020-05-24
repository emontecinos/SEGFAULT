#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "cr_API.h"

int main(int argument_count, char** arguments){
    for(int i=0;i>argument_count;i++){
        printf("%s\n",arguments[i]);
    }
    return 1;
}