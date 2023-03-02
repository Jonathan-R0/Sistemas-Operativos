#ifndef _STRACE_H_
#define _STRACE_H_

#define RED "\033[0;31m"
#define RESET "\033[0m"
#define INVALID_N_ARGS RED "Invalid number of arguments\n" RESET
#define EXEC_FAILED RED "Execution failed\n" RESET
#define FORK_FAILED RED "Fork failed\n" RESET
#define WAIT_FAILED RED "Wait failed\n" RESET
#define EXIT_0_MSG "+++ exited with 0 +++"
#define GETREGS_FAILED RED "Get registers failed\n" RESET

void safe_waitpid(int pid);

#endif  // _STRACE_H_