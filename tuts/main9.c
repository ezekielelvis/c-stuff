#include <stdio.h>
#include <stdlib.h>


struct struct_name{
    int age;
    char name;
} example; 


int main(void){
    example.age = 10;
    example.name = 'a';
    printf("My name is %c and I am %d years old", example.name, example.age);
    return 0;

}