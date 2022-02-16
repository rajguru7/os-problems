#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>

int* fibo = NULL;
int count = 0;

void* generate(void* var) {
    int* size = (int*)var;
    fibo[0] = 0;
    fibo[1] = 1;
    printf("%d \n", count);
    count = count + 2;
    for(int i=2; i<*size; i++) {
        fibo[i] = fibo[i-1]+fibo[i-2];
        count++;
    }
    return NULL;
}

int main() {
    int values;
    scanf("%d", &values);
    // printf("%d\n", values);
    fibo = (int*)calloc(values, sizeof(int));
    pthread_t tid;
    pthread_create(&tid, NULL, generate,(void*)&values);

    for(int i=0; i<values;) {
        if(count>i) {
            printf("%d ", fibo[i]);
            i++;
        }
    }
    pthread_exit(NULL);
}


