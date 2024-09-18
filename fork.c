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
pid_t giveBirth(proc_t *fetus, rp_sigs insigs, int fds[2], pid_t pid){
    pid_t id = fork();

    //signal(SIGINT, sigint_handler);
    //signal(SIGINT, SIG_DFL);

    if(id == 0){
        //printf("%d\n", id);
        //signal(SIGINT, sigint_handler);

        //printf("test1");
        if(pid != -1){
            setpgid(0, pid);
        }

        if(insigs.sbadinput == 0){
            if(insigs.sredir_in){
                int result = open(insigs.sinfile, O_RDONLY);
                if(result == -1){
                    printf("file doesn't exist\n");
                    return 0;
                }
                else{
                    dup2(result, STDIN_FILENO);
                }
            }
            if(insigs.sredir_out){
                dup2(open(insigs.soutfile, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH), STDOUT_FILENO);
            }
            if(insigs.sredir_err){
                dup2(open(insigs.serrfile, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH), STDERR_FILENO);
            }
        }

        if(fetus->leftArm){
            close(fds[0]);
            dup2(fds[1], STDOUT_FILENO);
            //perror("dup2");
        }
        else if(fetus->rightArm){
            close(fds[1]);
            dup2(fds[0], STDIN_FILENO);
            //perror("dup2");
        }

        setpgid(0, 0);

        if(execvp(fetus->cmd, fetus->args) == -1){
            exit(1);
        }
    }
    else if(id > 0){
        //printf("%d\n", id);
      int status;
      waitpid(id, &status, WUNTRACED);
        //fflush(stdout);
      //printf("# ");
    }
    //printf("\ntest\n");
    return id;
}

//pid_t oneChildPolicy(proc_t *fetus){
//    pid_t id = giveBirth(fetus);
//    return id;
//}
//tpid_t urPregnantWithTwins(proc_t *fetus1, proc_t *fetus2){
//    tpid_t twins;
//    twins.child1 = giveBirth(fetus1);
//    twins.child2 = giveBirth(fetus2);
//    return twins;
//}

