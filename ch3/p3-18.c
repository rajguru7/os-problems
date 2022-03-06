#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main() {

	pid_t pid=fork();

	if (pid<0) {
		printf("fork failed");
		exit(0);
	}
	else if (pid==0) {
		sleep(10);
		printf("child over");
	}
	else {
		sleep(20);
		printf("parent over");
	}
}
