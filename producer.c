#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/file.h>

static volatile int run_flag = 1;

void int_handler(int sig)
{
	run_flag = 0;
}

FILE* open_file(void)
{
	FILE *fp;
	char *fname;
	int fd;

	fname = malloc(sizeof(char) * 1024);
	memset(fname, 0, 1024);
	sprintf(fname, "file_%ld.csv", time(NULL));
	printf("Opening file \'%s\'.\n", fname);
	
	fp = fopen(fname, "w");

	if (fp == NULL) {
		fprintf(stderr, "Failed to open file \'%s\'. Exiting.\n", fname);
		exit(EXIT_FAILURE);
	}

	fd = fileno(fp);
	flock(fd, LOCK_EX);

	printf("Acquired lock for file \'%s\'.\n", fname);

	free(fname);
	return fp;
}

int write_data_to_file(FILE *fp)
{
	int i;
	printf("Started writing to file.\n");
	for(i = 0; i < 10; i++) {
		fprintf(fp, "%ld\n", i*time(NULL));
	}
	printf("Finished writing to file.\n");
	fclose(fp);
	return 1;
}

int main() 
{
	FILE *fp;

	signal(SIGINT, int_handler);

	while (run_flag) {
		fp = open_file();
		write_data_to_file(fp);
		printf("Hello\n");
		sleep(2);
	}

	exit(EXIT_SUCCESS);
}
