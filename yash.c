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
#define MAXTOKENS 67 // 2000/30 = 66.666 = 67

volatile int fg_sig, bg_sig, jobs_sig, bgjobs_sig;
 //= malloc(sizeof(jobs_t) * MAXJOBS);

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
        char **parsed_input = (char **) malloc(sizeof(char *) * MAXTOKENS+1);
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
        parsed_input[i] = NULL;

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

        //for debugging
        /*
        jobs_t a = {1, 0, 0, NULL};
        jobs_t b = {2, 0, 1, NULL};
        jobs_t c = {3, 0, 2, NULL};

        insert_job(&a, jobs_list_head);
        insert_job(&b, jobs_list_head);
        insert_job(&c, jobs_list_head);
        delete_job(jobs_list_head, NULL, 2);
         */

        // 3. Check for job control tokens (fg, bg, jobs, &) (for now just
        // ignore those commands)
        fg_sig = strcmp(parsed_input[0], "fg") ? 0 : 1;
        bg_sig = strcmp(parsed_input[0], "bg") ? 0 : 1;
        jobs_sig = strcmp(parsed_input[0], "jobs") ? 0 : 1;
        bgjobs_sig = strcmp(parsed_input[i-1], "&") ? 0 : 1;
        //TODO: why is the strcmp not working?
        //char last_char = parsed_input[i-1][strlen(parsed_input[i-1])-1];
        //bgjobs_sig = strcmp(&last_char, "&") ? 0 : 1;
        //if & always has space infront of it then it's much easier

        //for debugging
        /*
        printf("%s\n", &last_char);
        printf("%d\n", strcmp(&last_char, "&"));

        printf("%s\n", fg_sig ? "fg detected!" : "");
        printf("%s\n", bg_sig ? "bg detected!" : "");
        printf("%s\n", jobs_sig ? "jobs detected!" : "");
        printf("%s\n", bgjobs_sig ? "& detected!" : "");
         */




        proc_t proc1 = {parsed_input[0], parsed_input, NULL, NULL, NULL, 0, 0};
        proc_t proc2;

        //execvp(parsed_input[0], parsed_input);

        // 4. Determine the number of children processes to create (number of
        // times to call fork) (call fork once per child) (right now this will
        // just be one)

        // 5. Execute the commands using execvp or execlp - e.g. execOneChild()
        // or execTwoChildren()

        if(fg_sig){exec_fg();}
        if(bg_sig){exec_bg();}
        if(jobs_sig){exec_jobs();}
        if(bgjobs_sig){exec_bgjobs();}

        pid_t proc1_id = oneChildPolicy(&proc1);

        // 6. NOTE: There are other steps for job related stuff but good luck
        // we won't spell it out for you

        //More...

        free(input);
        free(parsed_input);
    }

    return 0;
}