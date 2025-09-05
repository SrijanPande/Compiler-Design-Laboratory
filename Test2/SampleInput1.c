#include <stdio.h>
#include <stdlib.h>

int AddIntegers(int a, int b){
    return a + b;
}

int main (int argc, char *argv[]){
    int x;
    puts("Enter a value for x :");
    scanf("%d",&x);
    
    int y;
    puts("Enter a value for y :");
    scanf("%d",&y);
    
    printf("%d",AddIntegers(x, y));
    return EXIT_SUCCESS;
}