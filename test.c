#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char* argv[]) {
    // void* param = argv[1];
    // printf("%lu" ,strlen(param));
    // printf("the number is: %d, ,%d\n", atoi(param), atoi(param + (int)strlen(param)+1));


    for(int i=0;i<=10;i++) {
        printf("%f ", (double)rand() / (double)RAND_MAX+1);
    }
}