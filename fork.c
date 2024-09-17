#include "fork.h"
#include "signals.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <readline/readline.h>

//TODO: more inputs
pid_t giveBirth(proc_t *fetus){
    pid_t id = fork();


    if(id == 0){

        //signal(SIGINT, sigint_handler);
        //TODO: add support for pipe and pgid
        //printf("test1");



        setpgid(0, 0);

        if(execvp(fetus->cmd, fetus->args) == -1){
            exit(1);
        }
    }
    else if(id > 0){
      int status;
      waitpid(id, &status, 0);
      //printf("# ");
    }
    //printf("\ntest\n");
    return id;
}

pid_t oneChildPolicy(proc_t *fetus){
    pid_t id = giveBirth(fetus);
    return id;
}
tpid_t urPregnantWithTwins(proc_t *fetus1, proc_t *fetus2){
    tpid_t twins;
    twins.child1 = giveBirth(fetus1);
    twins.child2 = giveBirth(fetus2);
    return twins;
}

