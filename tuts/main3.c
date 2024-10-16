#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Initial_cap 3

int main (){
    char answer[] = "y";
    double *scores = malloc(Initial_cap * sizeof(double));
    if(!scores){
        printf("Memory allocation failed\n");
        return 1;
    }
    int cap = Initial_cap;
    int numscores;
    for(numscores = 0; strcmp(answer, "y")==0; ++numscores){
        if (numscores == cap ){
            cap *= 2;
            scores = realloc(scores, cap * sizeof(double));
            if(!scores){
                fprintf(stderr, "failed to allocate scores array. \n ");
                return 1;
            }
        }
        printf("Enter a test score: ");
        scanf("%lf", &scores[numscores]);
        printf("Would you like to enter another score (y/n)? ");
        scanf("%s", answer);
    }
    double sum = 0;
    for (int loop = 0; loop < numscores; loop++){
        sum += scores[loop];
    }
    printf("%.2f is the average score. \n", sum / numscores);
    free(scores);
    return 0;
}

