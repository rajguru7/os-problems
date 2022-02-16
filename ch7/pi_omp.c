#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h> // Randmax
#include <time.h>
#include <pthread.h>


#define TOTAL 200
#define NUM_THREADS 4

int cir_pts = 0;
void* worker() {
    int temp = 0;
    for(int i=0; i<TOTAL; i++) {
        double x = ((double)rand()/RAND_MAX * 2) - 1;
        double y = ((double)rand()/RAND_MAX * 2) - 1;
        double val = pow(x,2)+pow(y,2);
        if(val<1)
            temp++;
    }
    #pragma omp critical 
    {
        cir_pts += temp;
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t tid[NUM_THREADS];
    for(int i=0; i<NUM_THREADS; i++) {
        pthread_create(&tid[i], NULL, worker, NULL);
    }
    for(int i=0; i<4; i++) {
        pthread_join(tid[i], NULL);
    }
    double pi = 4*(double)cir_pts/(TOTAL*NUM_THREADS);
    printf("%f", pi);
}