#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>

#define MAX_LINE 80 /* The maximum length command */

int main(void) {

	char *args[MAX_LINE/2 + 1]; /* command line arguments */
	int should_run=1; /* flag to determine when to exit program */
	char* prev=NULL;
	int prev_wait_flag=1;

	while (should_run) {

		printf("osh>"); 
		fflush(stdout);
		int count=0;
		int wait_flag=1;

		//reading user input
		do {
			args[count]=(char*)malloc(80*sizeof(char));
			scanf("%s", args[count++]);
		} while(getchar()!='\n');

		if (strcmp(args[count-1],"&")==0) {
			wait_flag=0;
			count--;
			free(args[count]);
		}
		args[count]=NULL;
		
		
		//debugging
		//for (int i=0; i<count; i++) {
			//printf("%lu\n", strlen(args[i]));
		//}

		//exiting if user enters exit
		if (strcmp(args[0],"exit")==0) {
			break;
		}
		else if (strcmp(args[0],"!!")==0) {
			if (prev==NULL) {
				printf("no command in history\n");
				continue;
			}
			memcpy(args[0], prev, strlen(prev));
			wait_flag=prev_wait_flag;
			printf("osh>");
			int i=0;
			do {
				printf("%s ", args[i++]);
			} while(args[i]);
			if (wait_flag==0)
				printf("&");
			printf("\n");
		}
		pid_t child=fork();
		if (child<0) {
			fprintf(stderr, "fork failed\n");
			exit(1);
		}
		else if (child==0) {
			execvp(args[0], args);
		}
		else {
			//printf("wait flag is: %d\n", wait_flag);
			if (wait_flag==1)
				while (wait(NULL)>0);
			prev_wait_flag=wait_flag;
			
			if (prev==NULL)
				prev=(char*)malloc(80*sizeof(char));
			memcpy(prev, args[0], strlen(args[0]));
			while (count!=0)
				free(args[--count]);
			
		}
	}

	
	if (prev!=NULL)
		free(prev);
	return 0;
}
