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

//signal handlers
void sigint_handler(int sig){
    printf("SIGINT detected\n");
    //TODO
    //kill(getpid(), )
}

void sigtstp_handler(int sig){
    printf("SIGTSTP detected\n");
    //TODO
}