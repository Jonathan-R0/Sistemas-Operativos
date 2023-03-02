#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#define NO_NUMBERS_SENT "No number was sent. You must include more arguments."
#define BAD_CHILD_EXIT "Error waiting for child process with code error: %d"
#define UNABLE_TO_WRITE "Unable to write from pipe."
#define BAD_FORK "Error: fork failed."
#define PIPE_ERROR "Error creating pipe."
#define UNABLE_TO_READ "Unable to read from stdin."
#define INPUT_NUMBER_ERROR "Error: n must be an integer greater than 1."
#define PRIME_FLAG "primo %d\n"
#define ERROR_CODE_EXIT -1

void filterFromPrime(int readFd);
void genNumbers(int n);

void
filterFromPrime(int readFd)
{
	int prime;
	int bytesRead;
	if ((bytesRead = read(readFd, &prime, sizeof(prime))) == -1) {
		fprintf(stderr, UNABLE_TO_READ "\n");
		exit(ERROR_CODE_EXIT);
	}
	if (bytesRead == 0) {
		close(readFd);
		return;
	}
	printf(PRIME_FLAG, prime);
	int p, fds[2];
	if ((p = pipe(fds)) == -1) {
		fprintf(stderr, PIPE_ERROR "\n");
		exit(ERROR_CODE_EXIT);
	}
	pid_t pid = fork();
	if (pid == -1) {
		fprintf(stderr, BAD_FORK "\n");
		exit(ERROR_CODE_EXIT);
	}
	if (pid == 0) {
		close(fds[1]);
		close(readFd);
		filterFromPrime(fds[0]);
	} else {
		close(fds[0]);
		int newNumber;
		while ((bytesRead = read(readFd, &newNumber, sizeof(newNumber))) >
		       0) {
			if ((newNumber % prime != 0) &&
			    (write(fds[1], &newNumber, sizeof(newNumber)) == -1)) {
				fprintf(stderr, UNABLE_TO_WRITE "\n");
				exit(ERROR_CODE_EXIT);
			}
		}
		close(readFd);
		close(fds[1]);
		if (wait(NULL) == -1) {
			fprintf(stderr, BAD_CHILD_EXIT "\n", errno);
			exit(ERROR_CODE_EXIT);
		}
	}
}

void
genNumbers(int n)
{
	int p, fds[2];
	if ((p = pipe(fds)) == -1) {
		fprintf(stderr, PIPE_ERROR "\n");
		exit(ERROR_CODE_EXIT);
	}
	pid_t pid = fork();
	if (pid == -1) {
		fprintf(stderr, BAD_FORK "\n");
		exit(ERROR_CODE_EXIT);
	}
	if (pid == 0) {
		close(fds[1]);
		filterFromPrime(fds[0]);
	} else {
		close(fds[0]);
		for (int i = 2; i <= n; i++) {
			if (write(fds[1], &i, sizeof(i)) == -1) {
				fprintf(stderr, UNABLE_TO_WRITE "\n");
				exit(ERROR_CODE_EXIT);
			}
		}
		close(fds[1]);
		if (wait(NULL) == -1) {
			fprintf(stderr, BAD_CHILD_EXIT "\n", errno);
			exit(ERROR_CODE_EXIT);
		}
	}
}

int
main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, NO_NUMBERS_SENT "\n");
		return ERROR_CODE_EXIT;
	}
	int n = (int) strtol(argv[1], NULL, 10);
	if (n < 2) {
		fprintf(stderr, INPUT_NUMBER_ERROR "\n");
		return ERROR_CODE_EXIT;
	}
	genNumbers(n);
	return 0;
}