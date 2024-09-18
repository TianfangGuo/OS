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
    //char *in, *out, *err;
    int leftArm, rightArm;
} proc_t;

typedef struct rp_sigs{
    int sbadinput;
    int sredir_out;
    int sredir_in;
    int sredir_err;
    int spipe;
    char *sinfile, *soutfile, *serrfile;
}rp_sigs;

//pid_t oneChildPolicy(proc_t *fetus);
pid_t giveBirth(proc_t *fetus, rp_sigs insigs, int fds[2], pid_t pid);
//tpid_t urPregnantWithTwins(proc_t *fetus1, proc_t *fetus2);

#endif