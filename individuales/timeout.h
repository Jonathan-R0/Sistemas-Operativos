#ifndef _TIMEOUT_H_
#define _TIMEOUT_H_

#define RED "\033[0;31m"
#define RESET "\033[0m"
#define INVALID_N_ARGS RED "Invalid number of arguments\n" RESET
#define INVALID_DURATION RED "Invalid duration\n" RESET
#define EXEC_FAILED RED "Execution failed\n" RESET
#define FORK_FAILED RED "Fork failed\n" RESET

void kill_process(int);
void create_action(void);
void create_event(int duration);

#endif  // _TIMEOUT_H_
