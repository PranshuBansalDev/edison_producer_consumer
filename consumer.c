#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <unistd.h>

int process_file(const char *fname)
{
	char delete_command[1024];
	FILE *fp;
	int fd;
	
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	// clear out the buffer
	memset(delete_command, 0, 1024);

	// open the file to inspect
	printf("\tOpening file \'%s\'\n", fname);
	fp = fopen(fname, "r");
	if (fp == NULL) {
		fprintf(stderr, "Failed to open file \'%s\'. Exiting.\n", fname);
		exit(EXIT_FAILURE);
	}

	// acquire the lock for the file (incase the producer is still writing to it)
	fd = fileno(fp);
	flock(fd, LOCK_EX);
	
	// print contents of file to stdout
	printf("\tAcquired lock. Printing file contents of \'%s\':\n", fname);
	while ((read = getline(&line, &len, fp)) != -1) {
		printf("\t\t%s", line);
	}
	fclose(fp);

	if (line) 
		free(line);

	printf("\tDeleting file \'%s\'\n", fname);
	
	// delete this file so we don't process it again in the future
	sprintf(delete_command, "rm %s", fname);
	system(delete_command);
	return 1;
}

int main()
{
	FILE * fp;
	int fd;
	char * line = NULL;
	char * fname;
	size_t len = 0;
	ssize_t read;

	while (1) {
		// tell the user what linux command we are executing
		printf("system(\"ls -1 file_*.csv > fnames.txt\")\n");
		system("ls -1 file_*.csv > fnames.txt");

		// open the file that contains the file names that we need to inspect
		fp = fopen("fnames.txt", "r");
		printf("Opening \'fnames.txt\' to read each file.\n");
		if (fp == NULL) {
			fprintf(stderr, "failed to open file\n");
			exit(EXIT_FAILURE);
		}

		// extract each file name by reading the file line-by-line
		while ((read = getline(&line, &len, fp)) != -1) {
			printf("Discovered file \'%s\'", line);
			
			// strip newline from end of line read from file
			fname = line;
			fname[strlen(line)-1] = 0;

			// process file with fname
			printf("Processing file \'%s\'", fname);
			process_file(fname);

			sleep(1);
		}

		// close the file that contains other filenames
		fclose(fp);

		// delete this file from the system
		printf("system(\"rm fnames.txt\")\n");
		system("rm fnames.txt");

		sleep(2);
	}

	return EXIT_SUCCESS;
}
