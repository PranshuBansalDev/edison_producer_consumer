#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/file.h>

static volatile int run_flag = 1;

void int_handler(int sig)
{
	run_flag = 0;
}

int main() 
{
	FILE *fp;
	int i = 0;
	int fd;

	signal(SIGINT, int_handler);

	fp = fopen ("fname.txt", "w");
	printf("Just opened the file\n");
	if (fp == NULL) {
		fprintf(stderr, "fname.txt failed to open. Exiting.");
		exit(EXIT_FAILURE);
	}

	while (run_flag) {
		fprintf(fp, "Hello World! %d\n", i);
		fprintf(stdout, "Hello World! %d\n", i);
		sleep(1);
		i++;
	}

	fclose(fp);
	exit(EXIT_SUCCESS);
}
