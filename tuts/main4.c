#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(){
    int lower, upper; 
    lower = 1;
    upper= 2;
    srand(time(0));
    int count;
    printf("Enter how many times you'll like to flip a coin? \n");
    scanf("%d", &count);
    int result;
    int heads = 0;
    int tails = 0;
    for(int i = 0; i < count; i ++){
        result = (rand() % (upper - lower + 1)) + lower;
        if (result == 1){
            heads++;
        }else if (result == 2){
            tails++;
        }
    }
    printf("After %d flips, there were %d heads and %d tails.\n", count, heads, tails);
}