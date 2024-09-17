#ifndef FORK_H
#define FORK_H

#include <unistd.h>
#include <sys/wait.h>

typedef struct tpid_t{
    int child1;
    int child2;
} tpid_t;

typedef struct proc_t{
    char *cmd;
    char **args;
    char *in, *out, *err;
    int leftArm, rightArm;
} proc_t;

pid_t oneChildPolicy(proc_t *fetus);
tpid_t urPregnantWithTwins(proc_t *fetus1, proc_t *fetus2);

#endif