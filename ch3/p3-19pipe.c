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
    #define READ_END 0
    #define WRITE_END 1
    pid_t pid;
    /* fork a child process */
    int fd[2];
    if(pipe(fd) == -1) {
        fprintf(stderr,"Pipe failed");
        return 1;
    }
    pid = fork();
    if (pid < 0) { /* error occurred */ 
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0) { /* child process */
        struct timeval currenttime;
        
        printf("cmd is: %s \n", cmd);
        
  
        gettimeofday(&currenttime, NULL);//gettimeofday returns 0 for success and -1 for failure
        long int begin = currenttime.tv_sec*1000000 + currenttime.tv_usec;
        close(fd[READ_END]);
        /* write to the pipe */
        write(fd[WRITE_END], &begin, sizeof(begin));
        /* close the write end of the pipe */
        close(fd[WRITE_END]);
        printf("begin is: %ld \n", begin);

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
        long int begin;
        close(fd[WRITE_END]);
        /* write to the pipe */
        read(fd[READ_END], &begin, sizeof(begin));
        /* close the write end of the pipe */
        close(fd[READ_END]);

        
        printf("time elapsed: %ld \n", (end-begin));
        
    }
    return 0;
}