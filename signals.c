#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <readline/readline.h>

#include "signals.h"
#include "jobs.h"
#include "fork.h"

//signal handlers
void sigint_handler(int sig){
    //printf("SIGINT detected\n");
    //TODO
    pid_t id = child_pid;//getpid();
//    pid_t shell = getpgrp();
//    printf("\n%d\n%d\n", id, shell);
    if(id != 0) {
        kill(id, SIGINT);
        printf("\n");
    }
}

void sigtstp_handler(int sig){
    //printf("SIGTSTP detected\n");
    //TODO
    pid_t id = child_pid;
    if(id != 0){
        kill(id, SIGINT);
        printf("\n");
        waitpid(-id, 0, WUNTRACED|WNOHANG);
    }
}

void sigchld_handler(int sig){
    //printf("SIGCHLD detected\n");
    //tcsetpgrp(STDIN_FILENO, getpgrp());

    pid_t id = child_pid;//getpid();
    pid_t shell = getpgrp();
    //printf("\n%d\n%d\n", id, shell);

    int status;
    int ret = waitpid(-(child_pid), &status, WNOHANG|WUNTRACED);
    if(WIFSTOPPED(status) && ret>0){
        printf("got here\n");
        tcsetpgrp(STDIN_FILENO, getpgrp());
    }
    else{
        //printf("%d\n", ret);
    }
}