#include <stdio.h>
#include <stdlib.h>

int main() {
    int* p = (int*)(calloc(2, sizeof(int)));
    p[1] = 1;
    p[0] = 3;
    printf("%d", p[10000]);
    free(p);
    printf("tried to understand pid_manager.c from github repo");
}
