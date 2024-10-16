#include <stdio.h>

int main(void)
{
    int numerator;
    int  denominator;

    printf("Enter the numerator : ");
    scanf("%d", &numerator);

    printf("Enter the denominator : ");
    scanf("%d", &denominator);

    if (denominator == 0)
    {
        printf("Cannot divide by zero");
    }

    if (numerator % denominator == 0)
    {
        printf("There is no remander when dividing %d by %d", numerator, denominator);
    }
    else
    {
        printf("The remainder when dividing %d by %d is %d", numerator, denominator, numerator % denominator);
    }
}