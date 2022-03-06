#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE *file1, *file2;
	char filename[100], c;
	printf("Enter source file name \n");
	scanf("%s",filename);
	file1 = fopen(filename, "r");
	if (file1==NULL) {
		printf("invalid file \n");
		exit(0);
	}

	printf("Enter destination file name \n");
	scanf("%s", filename);
	file2 = fopen(filename, "w");
	if (file2==NULL) {
		printf("invalid file \n");
		exit(0);
	}
	c = fgetc(file1);
	while (c != EOF) {
		fputc(c, file2);
		c = fgetc(file1);
	}
	printf("contents copied");
	fclose(file1);
	fclose(file2);
	return 0;	
}
