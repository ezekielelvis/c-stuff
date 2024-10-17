#include <stdio.h>
#include <stdlib.h>

int main(void){
    char *ptr;

    ptr = malloc(sizeof(char) * 10);

    if(ptr == NULL){
        printf("Memory can not be allocated\n");

        return 1;
    }else{
        printf("Memory was allocted");
        free(ptr);

        return 0;


    }
}