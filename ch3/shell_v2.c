#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>

#define MAX_LINE 80 /* The maximum length command */

int main(void) {

	char *args[MAX_LINE/2 + 1]; /* command line arguments */
	char *prev[MAX_LINE/2 + 1];
	prev[0]=NULL;
	int should_run=1; /* flag to determine when to exit program */
	int prev_wait_flag=1;
	int prev_redirect_flag=0;
	int prev_count=0;
	int prev_pipe_pos=-1;
	int prev_pipe_flag=0;

	while (should_run) {

		printf("osh>"); 
		fflush(stdout);
		int count=0;
		int wait_flag=1;
		int redirect_flag=0;
		int pipe_flag=0;
		int pipe_pos=-1;

		//reading user input
		do {
			args[count]=(char*)malloc(80*sizeof(char));
			scanf("%s", args[count++]);
			if (strcmp(args[count-1], "<")==0) {
				redirect_flag=1;
			}
			else if (strcmp(args[count-1], ">")==0) {
				redirect_flag=2;
			}
			else if (strcmp(args[count-1], "|")==0) {
				pipe_flag=1;
				pipe_pos=count-1;
			}
		} while(getchar()!='\n');
		if (strcmp(args[count-1],"&")==0) {
			wait_flag=0;
			count--;
			free(args[count]);
		}
		args[count]=NULL;

		if (strcmp(args[0],"!!")==0) {
			if (prev[0]==NULL) {
				printf("no command in history\n");
				continue;
			}
			free(args[0]);
			count=prev_count;
			for (int i=0; i<count; i++) {
				args[i]=(char*)malloc(80*sizeof(char));
				memcpy(args[i], prev[i], strlen(prev[i]));
				free(prev[i]);

			}
			args[count]=NULL;
			wait_flag=prev_wait_flag;
			redirect_flag=prev_redirect_flag;
			pipe_pos=prev_pipe_pos;
			pipe_flag=prev_pipe_flag;
			printf("osh>");
			int i=0;
			do {
				printf("%s ", args[i++]);
			} while(args[i]);
			if (wait_flag==0)
				printf("&");
			printf("\n");
		}

		//exiting if user enters exit
		if (strcmp(args[0],"exit")==0) {
			break;
		}
		
		pid_t child=fork();
		if (child<0) {
			fprintf(stderr, "fork failed\n");
			exit(1);
		}
		else if (child==0) {
			if (pipe_flag==0) {
				if (redirect_flag==1) {
					int fd=open(args[count-1], O_RDONLY);
					dup2(fd, STDIN_FILENO);
					close(fd);
					args[count-2]=NULL;
				}
				else if (redirect_flag==2) {
					int fd=open(args[count-1], O_CREAT | O_TRUNC | O_WRONLY);
					printf("fd is %d\n", fd);
					fflush(stdout);
					dup2(fd, STDOUT_FILENO);
					close(fd);
					printf("fd is %d\n", fd);
					fflush(stdout);
					args[count-2]=NULL;
				}
				execvp(args[0], args);
			}
			else {
				int fd[2];
				if (pipe(fd)==-1) {
					fprintf(stderr, "pipe failed\n");
				}
				args[pipe_pos]=NULL;
				pid_t pipe_child=fork();
				if (pipe_child<0) {
					fprintf(stderr, "fork failed\n");
				}
				else if (pipe_child==0) {
					close(fd[0]);
					dup2(fd[1], STDOUT_FILENO);
					close(fd[1]);
					execvp(args[0], args);
				}
				else {
					wait(NULL);
					close(fd[1]);
					dup2(fd[0], STDIN_FILENO);
					close(fd[0]);
					execvp(args[pipe_pos+1], &args[pipe_pos+1]);
				}
			}
		}
		else {
			//printf("wait flag is: %d\n", wait_flag);
			if (wait_flag==1)
				while (wait(NULL)>0);
			prev_wait_flag=wait_flag;
			prev_redirect_flag=redirect_flag;
			prev_pipe_flag=pipe_flag;
			prev_pipe_pos=pipe_pos;

			for (int i=0; i<count; i++) {
				prev[i]=(char*)malloc(80*sizeof(char));
				memcpy(prev[i], args[i], strlen(args[i]));
				prev_count=count;
				free(args[i]);
			}
		}
	}

	
	while (prev_count!=0)
		free(prev[--prev_count]);

	return 0;
}
