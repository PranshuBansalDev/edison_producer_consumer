#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>

int main(void)
{
	FILE * fp;
	int fd;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	fp = fopen("fname.txt", "r");
	printf("just opened the file for reading\n");
	if (fp == NULL) {
		fprintf(stderr, "failed to open file\n");
		exit(EXIT_FAILURE);
	}

	fd = fileno(fp);
	flock(fd, LOCK_EX);
	printf("just got the lock for reading the file\n");

	while ((read = getline(&line, &len, fp)) != -1) {
		printf("Retrieved line of length %zu :\n", read);
		printf("%s", line);
	}

	fclose(fp);
	if (line)
		free(line);

	printf("Died.\n");
	exit(EXIT_SUCCESS);
}
