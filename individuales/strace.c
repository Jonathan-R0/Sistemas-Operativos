#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <syscall.h>
#include <sys/ptrace.h>
#include <linux/ptrace.h>
#include <sys/user.h>
#include <stdbool.h>
#include "strace.h"
#include "syscallent.h"

void
safe_waitpid(int pid)
{
	if (waitpid(pid, 0, 0) == -1) {
		perror(WAIT_FAILED);
		exit(1);
	}
}

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		errno = EINVAL;
		perror(INVALID_N_ARGS);
		return 1;
	}

	int pid = fork();

	if (pid < 0) {
		perror(FORK_FAILED);
		return 1;
	} else if (pid == 0) {
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		execvp(argv[1], argv + 1);
		perror(EXEC_FAILED);
		return 1;
	}

	safe_waitpid(pid);
	ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD);

	while (true) {
		if (ptrace(PTRACE_SYSCALL, pid, 0, 0) == -1) {
			fprintf(stderr, "%s\n", EXIT_0_MSG);
			return 1;
		}
		safe_waitpid(pid);
		struct user_regs_struct reg;
		ptrace(PTRACE_GETREGS, pid, 0, &reg);
		if (reg.orig_rax == SYS_exit_group) {
			printf("%s\n", EXIT_0_MSG);
			return 0;
		}
		if (ptrace(PTRACE_SYSCALL, pid, 0, 0) == -1) {
			fprintf(stderr, "%s\n", EXIT_0_MSG);
			return 1;
		}
		safe_waitpid(pid);
		struct ptrace_syscall_info info;
		ptrace(PTRACE_GET_SYSCALL_INFO, pid, sizeof(info), &info);
		if (reg.orig_rax >= 335)
			continue;
		printf("%s = %lld\n", syscall_names[reg.orig_rax], info.exit.rval);
	}
}