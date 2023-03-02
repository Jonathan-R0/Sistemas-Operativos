#define _GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <sys/wait.h>
#include "timeout.h"

int pid = 0;

struct sigevent event = {
	.sigev_notify = SIGEV_SIGNAL,
	.sigev_signo = SIGUSR1,
};

void
kill_process(__attribute__((unused)) int s)
{
	kill(pid, SIGKILL);
}

void
create_action(void)
{
	struct sigaction action;
	action.sa_handler = &kill_process;
	sigaction(SIGUSR1, &action, NULL);
}

void
create_event(int duration)
{
	struct itimerspec timer;
	timer.it_value.tv_sec = duration;
	timer.it_value.tv_nsec = 0;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_nsec = 0;

	timer_t timer_id;
	timer_create(CLOCK_REALTIME, &event, &timer_id);
	timer_settime(timer_id, 0, &timer, NULL);
}

int
main(int argc, char *argv[])
{
	int duration;

	if (argc < 3) {
		errno = EINVAL;
		perror(INVALID_N_ARGS);
		return 1;
	} else if ((duration = strtol(argv[1], NULL, 10)) < 0) {
		errno = EINVAL;
		perror(INVALID_DURATION);
		return 1;
	}

	create_action();
	create_event(duration);

	pid = fork();
	if (pid < 0) {
		perror(FORK_FAILED);
		return 1;
	} else if (pid == 0) {
		execvp(argv[2], argv + 2);
		perror(EXEC_FAILED);
		return 1;
	}

	wait(NULL);

	return 0;
}
