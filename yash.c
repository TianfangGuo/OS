#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <readline/readline.h>

#include "fork.h"
#include "jobs.h"
#include "pipes.h"
#include "redirection.h"
#include "signals.h"

//GLOBALS
#define MAXJOBS 20
#define MAXTOKENS 67 // 2000/30 = 66.666 = 67
volatile int fg_sig, bg_sig, jobs_sig, bgjobs_sig;

//signal handlers
void sigint_handler(int sig){
    printf("SIGINT detected\n");
    //TODO
}

void sigtstp_handler(int sig){
    printf("SIGTSTP detected\n");
    //TODO
}


int main(void)
{

    while (1) {
        // 0. Register signal handlers
        signal(SIGINT, sigint_handler);
        signal(SIGTSTP, sigtstp_handler);

        // 1. Print the prompt (#)
        printf("# ");

        // 2. Grab and parse the input - NOTE: Make sure to remove the newline
        // character from the input string (otherwise, you'll pass "ls\n" to
        // execvp and there is no executable called "ln\n" just "ls")

        char *input = readline("");
        //check for EOF
        if(input == NULL){
            printf("# Exiting YASH, goodbye :)\n");
            exit(0);
        }
        char **parsed_input = (char **) malloc(sizeof(char *) * MAXTOKENS);
        char *curtok;
        char *saveptr;
        //i = total number of tokens
        int i = 0;

        curtok = strtok_r(input, " ", &saveptr);
        while(curtok != NULL && i < MAXTOKENS){
            parsed_input[i] = curtok;
            //printf("Token: %s\n", curtok);
            parsed_input[i] = curtok;
            i++;
            curtok = strtok_r(NULL, " ", &saveptr);
        }

        //edge cases
        if(i == 0){
            printf("\n# Exiting YASH, goodbye :)\n");
            exit(0);
        }
        else if(i >= MAXTOKENS){
            printf("Warning: max token limit reached, only the first 67 tokens were recorded, which may lead to unexpected behavior! \n");
        }
        //for debugging
        /*
        else if(!strcmp(parsed_input[0], "exit")){
            printf("Goodbye :)\n");
            exit(0);
        }
         */

        //for debugging
        /*
        printf("parsed tokens: ");
        for(int j = 0; j < i; j++) {
            printf("%s ", parsed_input[j]);
        }
        printf("\n");
         */

        // 3. Check for job control tokens (fg, bg, jobs, &) (for now just
        // ignore those commands)
        fg_sig = strcmp(parsed_input[0], "fg") ? 0 : 1;
        bg_sig = strcmp(parsed_input[0], "bg") ? 0 : 1;
        jobs_sig = strcmp(parsed_input[0], "jobs") ? 0 : 1;
        //TODO: why is the strcmp not working?
        //char last_char = parsed_input[i-1][strlen(parsed_input[i-1])-1];
        //bgjobs_sig = strcmp(&last_char, "&") ? 0 : 1;
            //if & always has space infront of it then it's much easier
            bgjobs_sig = strcmp(parsed_input[i-1], "&") ? 0 : 1;

        //for debugging
        /*
        printf("%s\n", &last_char);
        printf("%d\n", strcmp(&last_char, "&"));

        printf("%s\n", fg_sig ? "fg detected!" : "");
        printf("%s\n", bg_sig ? "bg detected!" : "");
        printf("%s\n", jobs_sig ? "jobs detected!" : "");
        printf("%s\n", bgjobs_sig ? "& detected!" : "");
         */


        // 4. Determine the number of children processes to create (number of
        // times to call fork) (call fork once per child) (right now this will
        // just be one)

        // 5. Execute the commands using execvp or execlp - e.g. execOneChild()
        // or execTwoChildren()

        // 6. NOTE: There are other steps for job related stuff but good luck
        // we won't spell it out for you

        //More...

        free(input);
        free(parsed_input);
    }

    return 0;
}