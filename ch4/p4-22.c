#include <pthread.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int min, max, length; /* this data is shared by the thread(s) */
float avg;
//void *average(void *param); /* threads call this function */
/* The thread will execute in this function */ 
void *average(void *param)
{
    int i;
    avg = 0;
    for (i = 1; i <= length; i++) {
        avg += atoi(param);
        param = param + strlen(param)+1;
    }
    
    avg = avg/length;
    pthread_exit(0); 
}

void *minimum(void* param) {
    int i;
    min = INT32_MAX;
    for(i=1; i<length; i++) {
        int temp = atoi(param);
        if(temp<min) {
            min = temp;
        }
        param = param + strlen(param)+1;
    }
    pthread_exit(0);
}

void *maximum(void* param) {
    int i;
    max = INT32_MIN;
    for(i=1; i<length; i++) {
        int temp = atoi(param);
        if(temp>max) {
            max = temp;
        }
        param = param + strlen(param)+1;
    }
    pthread_exit(0);
}
int main(int argc, char *argv[])
{
    length = argc;
    pthread_t tid[3]; /* the thread identifier */
    pthread_attr_t attr[3]; /* set of thread attributes */
    /* set the default attributes of the thread */ 
    
    void* func[3] = {average, minimum, maximum};
    /* create the thread */
    for(int i=0; i<3; i++) {
        pthread_attr_init(attr + i);
        pthread_create(tid + i, attr + i, func[i], argv[1]);

    }
    /* wait for the thread to exit */ 
    for(int i=0; i<3; i++) {
        pthread_join(tid[i],NULL);
    }
    printf("average = %f\n",avg); 
    printf("minimum = %d\n",min); 
    printf("maximum = %d\n",max); 
}
