#include <inc/lib.h>

#define GREEN_COLOR "\033[0;32m"
#define RED_COLOR "\033[0;31m"
#define POOP_COLOR "\033[0;33m"
#define PINK_COLOR "\033[0;35m"
#define RESET "\033[0m"

#define LIMIT 50

void parent_p1() {
        int prio;
        sys_env_get_priority(
            thisenv->env_id,
            &prio
        );
        sys_env_set_priority(
            thisenv->env_id,
            30
        );
        cprintf(RED_COLOR"child(%d) setting prio to: %d\n"RESET, thisenv->env_id, thisenv->env_priority);
        sys_env_get_priority(
            thisenv->env_id,
            &prio
        );
        for (int j = 0; j <= LIMIT; j++) {
            cprintf(RED_COLOR"%d-"RESET, j);
            if (j == 20) {
                sys_env_set_priority(
                    thisenv->env_id,
                    90
                ); // This is boring, lets go faster! 
            }
        }
        cprintf(GREEN_COLOR"\nfinished child(%d) with prio: %d\n"RESET, thisenv->env_id, prio);
}

void child_p2() {
        int prio;
        sys_env_get_priority(
            thisenv->env_id,
            &prio
        );
        sys_env_set_priority(
            thisenv->env_id,
            20
        );
        cprintf(POOP_COLOR"child(%d) setting prio to: %d\n"RESET, thisenv->env_id, thisenv->env_priority);
        sys_env_get_priority(
            thisenv->env_id,
            &prio
        );
        for (int j = 0; j <= LIMIT; j++)
            cprintf(POOP_COLOR"%d-"RESET, j);
        cprintf(GREEN_COLOR"\nfinished child(%d) with prio: %d\n"RESET, thisenv->env_id, prio);
}

void grandchild_p3() {
        int prio;
        sys_env_get_priority(
            thisenv->env_id,
            &prio
        );
        sys_env_set_priority(
            thisenv->env_id,
            40
        );
        cprintf(PINK_COLOR"child(%d) setting prio to: %d\n"RESET, thisenv->env_id, thisenv->env_priority);
        sys_env_get_priority(
            thisenv->env_id,
            &prio
        );
        for (int j = 0; j <= LIMIT; j++)
            cprintf(PINK_COLOR"%d-"RESET, j);
        cprintf(GREEN_COLOR"\nfinished child(%d) with prio: %d\n"RESET, thisenv->env_id, prio);
}

void
umain(int argc, char **argv) {
    int i = fork();
    if (i > 0) {
       parent_p1();
    } else if (i == 0) {
        int j = fork();
        if (j > 0) {
            child_p2();
        } else if (j == 0) {
            grandchild_p3();
        } else if (j < 0) {
            panic("fork failed");
        }
    } else if (i < 0) {
        panic("fork failed");
    }
}