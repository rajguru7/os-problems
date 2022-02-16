#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h> // memset
#include <stdint.h> // uint64_t
#include <stdlib.h> 
#include <pthread.h>
#include <time.h>



#define MIN_PID 3
#define MAX_PID 7

int allocate_map();
int allocate_pid();
void release_pid();

int h, base, end, size;
uint64_t* bitarray = NULL;
int npid = MAX_PID - MIN_PID + 1;
pthread_mutex_t lock;

void clear_value(int index) {
    int x = (int)(index % (8*sizeof(uint64_t)));
    int y = (int)(index/(8*sizeof(uint64_t)));
    bitarray[y] = bitarray[y] & ~(1UL<<x);
}

int get_value(int index) {
    int x = (int)(index % (8*sizeof(uint64_t)));
    int y = (int)(index/(8*sizeof(uint64_t)));
    return (int)((bitarray[y] & (1UL<<x)) > 0);
}

void set_value(int index) {
    int x = (int)(index % (8*sizeof(uint64_t)));
    int y = (int)(index/(8*sizeof(uint64_t)));
    bitarray[y] = bitarray[y] | (1UL<<x);
}

void print_bits() {
    for(int i=0; i<=end; i++){
        printf("%d ",get_value(i));
    }
    printf("\n");
}

int allocate_map() {
    h = (int)(ceil(log2(npid)));
    int bits = pow(2,h+1)-1;
    size = bits/64;
    bitarray = (uint64_t*)calloc(size, sizeof(uint64_t));
    base = pow(2,h)-1;
    end = base + npid - 1;
    print_bits();
    printf("base: %d end: %d\n", base, end);
    return 1;
}

int allocate_pid() {
    pthread_mutex_lock(&lock);
    int index = 0;
    // printf("hi\n");

    if(get_value(index)==1)
        return -1;
    // printf("hi\n");
    do {
        int left = index*2+1;
        int right = index*2+2;
        if(get_value(left)==0)
            index = left;
        else if(get_value(right)==0)
            index = right;
        // printf("%d\n",index);
    } while(!(index<=end && index>=base));

    set_value(index);
    int parent = index;
    while(parent>0) {
        parent = (parent-1)/2;
        int left = parent*2+1;
        int right = parent*2+2;
        if(get_value(left)==1 && get_value(right)==1)
            set_value(parent);
    }
    printf("using pid: %d\n", index-base+MIN_PID);
    // print_bits();
    pthread_mutex_unlock(&lock);
    return index - base;
}

void release_pid(pid_t pid) {
    pthread_mutex_lock(&lock);
    int index = base + pid - MIN_PID;
    clear_value(index);
    int parent = index;
    while(parent>0) {
        parent = (parent-1)/2;
        int left = parent*2+1;
        int right = parent*2+2;
        if(get_value(left)==0 || get_value(right)==0)
            clear_value(parent);
    }
    printf("releasing pid: %d\n", pid);
    print_bits();
    pthread_mutex_unlock(&lock);
}

void* use_pid() {
    pid_t pid = allocate_pid()+MIN_PID;
    // printf("using pid: %d \n", pid);
    int ts = (((double)rand()/RAND_MAX)*2)+1;
    sleep(0);
    // printf("slept for %d second\n releasing pid: %d \n", ts, pid);
    release_pid(pid);
    return NULL;
}
int main() {
    printf("allocation map done: %d\n", allocate_map());
    printf("allocating pids to threads\n");
    // for(int idx=0; idx<npid; idx++)
    //     printf("pid allocated %d \n" ,allocate_pid()+MIN_PID);

    // printf("\n Releasing pids \n");

    // for(int idx=MIN_PID; idx<=MAX_PID; idx++){
    //     release_pid(idx);
    //     printf("released pid: %d \n", idx);
    // }
    pthread_mutex_init(&lock, NULL);
    srand(time(NULL));
    pthread_t tid[npid];
    for(int idx=MIN_PID; idx<=MAX_PID; idx++)
        pthread_create(&tid[idx], NULL, use_pid, NULL);

    for(int idx=MIN_PID; idx<=MAX_PID; idx++)
        pthread_join(tid[idx], NULL);
    
    return 0;
}