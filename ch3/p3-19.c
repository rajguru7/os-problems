#include <sys/types.h> 
#include <sys/time.h>
#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>

int main(int argc, char *argv[] ) {
    char* cmd = argv[1];

    pid_t pid;
    /* fork a child process */
    pid = fork();
    if (pid < 0) { /* error occurred */ 
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0) { /* child process */
        struct timeval currenttime;
        const int SIZE = 16;
        /* create the shared memory object */
        const char* shmfile = "shmfile";
        printf("cmd is: %s \n", cmd);
        int fd = shm_open(shmfile, O_CREAT | O_RDWR, 0666);
        /* configure the size of the shared memory object */ 
        ftruncate(fd, SIZE);
        /* memory map the shared memory object */
        long int *begin = (long int*) mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  
        gettimeofday(&currenttime, NULL);//gettimeofday returns 0 for success and -1 for failure
        *begin = currenttime.tv_sec*1000000 + currenttime.tv_usec;
        printf("begin is: %ld \n", *begin);
        //begin += 1;
        char path[15] = "/bin/";
        strcat(path,cmd);
        printf("path is: %s \n", path);
        execlp(path,cmd,NULL); 

    }
    else { /* parent process */
    /* parent will wait for the child to complete */ 
        wait(NULL);
        struct timeval currenttime;
        gettimeofday(&currenttime, NULL);
        printf("Child Complete \n");
        long int end = currenttime.tv_sec*1000000 + currenttime.tv_usec;
        printf("end is: %ld \n", end);
        const int SIZE = 16;
        /* create the shared memory object */
        const char* shmfile = "shmfile";
        int fd = shm_open(shmfile, O_RDONLY, 0666);
        /* configure the size of the shared memory object */ 
        ftruncate(fd, SIZE);
        /* memory map the shared memory object */
        long int *begin = (long int*) mmap(0, SIZE, PROT_READ, MAP_SHARED, fd, 0);

        
        printf("time elapsed: %ld \n", (end-*begin));
        //printf("time taken: %d" , (end-*begin));
        shm_unlink(shmfile);
        
    }
    return 0;
}