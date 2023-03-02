#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#define NO_COMMAND_SENT "No action was sent. You must include more arguments."
#define UNABLE_TO_EXECUTE "Unable to execute command with code error: %d."
#define UNABLE_TO_READ "Unable to read from stdin."
#define BAD_CHILD_EXIT "Error waiting for child process with code error: %d"
#define BAD_FORK "Error: fork failed."
#define ERROR_CODE_EXIT -1

#ifndef NARGS
#define NARGS 4

int getData(char *buffer[NARGS + 2], int *argsRead);
void freeData(char *buffer[NARGS + 2], int argsRead);
void run(char *command);

int
getData(char *buffer[NARGS + 2], int *argsRead)
{
	char *recv = NULL;
	size_t stdinArgSize = 0;
	int bytesRead = (int) getline(&recv, &stdinArgSize, stdin);
	if (bytesRead != -1) {
		recv[bytesRead - 1] = 0;
		buffer[*argsRead + 1] = recv;
		*argsRead += 1;
		recv = NULL;
	}
	free(recv);
	return bytesRead;
}

void
freeData(char *buffer[NARGS + 2], int argsRead)
{
	for (int i = 1; i <= argsRead; i++) {
		if (buffer[i] != NULL)
			free(buffer[i]);
	}
}

void
run(char *command)
{
	char *args[NARGS + 2] = { NULL };
	args[0] = command;
	int bytesRead;
	int argsRead = 0;
	while ((bytesRead = getData(args, &argsRead))) {
		if (argsRead == NARGS || bytesRead == -1) {
			for (int i = argsRead + 1; i <= NARGS; i++)
				args[i] = NULL;
			pid_t pid = fork();
			if (pid == -1) {
				fprintf(stderr, BAD_FORK "\n");
				exit(ERROR_CODE_EXIT);
			}
			if (pid == 0 && execvp(command, args) == -1) {
				fprintf(stderr, UNABLE_TO_EXECUTE "\n", errno);
				exit(ERROR_CODE_EXIT);
			}
			if (wait(NULL) == -1) {
				fprintf(stderr, BAD_CHILD_EXIT "\n", errno);
				exit(ERROR_CODE_EXIT);
			}
			freeData(args, argsRead);
			argsRead = 0;
			if (bytesRead == -1)
				break;
		}
	}
}

int
main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, NO_COMMAND_SENT "\n");
		return ERROR_CODE_EXIT;
	}
	run(argv[1]);
	return 0;
}

#endif  // NARGS
