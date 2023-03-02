#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define PIPE_ERROR "Error creating pipe."
#define BAD_FORK "Error: fork failed."
#define UNABLE_TO_READ "Unable to read from pipe."
#define UNABLE_TO_WRITE "Unable to write from pipe."
#define TAB "\t- "
#define ERROR_CODE_EXIT -1

int
main(void)
{
	srandom(time(NULL));
	int fds1[2], fds2[2];
	int pipe1 = pipe(fds1);
	int pipe2 = pipe(fds2);
	if (pipe1 == -1 || pipe2 == -1) {
		fprintf(stderr, PIPE_ERROR "\n");
		return ERROR_CODE_EXIT;
	}
	printf("Hola, soy PID %d\n", getpid());
	printf(TAB "primer pipe me devuelve: [%d, %d]\n", fds1[0], fds1[1]);
	printf(TAB "segundo pipe me devuelve: [%d, %d]\n\n", fds2[0], fds2[1]);
	pid_t pid = fork();
	if (pid == -1) {
		fprintf(stderr, BAD_FORK "\n");
		return ERROR_CODE_EXIT;
	}
	if (pid == 0) {
		int recv;
		if (read(fds1[0], &recv, sizeof(recv)) == -1) {
			fprintf(stderr, UNABLE_TO_READ "\n");
			return ERROR_CODE_EXIT;
		}
		printf("Donde fork me devuelve 0:\n");
		printf(TAB "getpid me devuelve: %d\n", getpid());
		printf(TAB "getppid me devuelve: %d\n", getppid());
		printf(TAB "recibo valor %d vía fd=%d\n", recv, fds1[0]);
		printf(TAB "reenvío valor en fd=%d y termino\n\n", fds2[1]);
		if (write(fds2[1], &recv, sizeof(recv)) == -1) {
			fprintf(stderr, UNABLE_TO_WRITE "\n");
			return ERROR_CODE_EXIT;
		}
		close(fds1[0]);
		close(fds1[1]);
		close(fds2[0]);
		close(fds2[1]);
	} else {
		int send = random();
		int recv = 0;
		printf("Donde fork me devuelve %d:\n", pid);
		printf(TAB "getpid me devuelve: %d\n", getpid());
		printf(TAB "getppid me devuelve: %d\n", getppid());
		printf(TAB "random me devuelve: %d\n", send);
		printf(TAB "envío valor %d a través de fd=%d\n\n", send, fds1[1]);
		if (write(fds1[1], &send, sizeof(send)) == -1) {
			fprintf(stderr, UNABLE_TO_WRITE "\n");
			return ERROR_CODE_EXIT;
		}
		if (read(fds2[0], &recv, sizeof(recv)) == -1) {
			fprintf(stderr, UNABLE_TO_READ "\n");
			return ERROR_CODE_EXIT;
		}
		printf("Hola, de nuevo PID %d\n", getpid());
		printf(TAB "recibí valor %d vía fd=%d\n", recv, fds2[0]);
		wait(NULL);
		close(fds1[0]);
		close(fds1[1]);
		close(fds2[0]);
		close(fds2[1]);
	}
	return 0;
}
