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

    //for testing
    jobs_t a = {1, 0, 0, "placeholder a", NULL};
    jobs_t b = {2, 0, 1, "placeholder b", NULL};
    jobs_t c = {3, 0, 2, "placeholder c", NULL};

    insert_job(&a, jobs_list_head);
    insert_job(&b, jobs_list_head);
    insert_job(&c, jobs_list_head);

    while (1) {
        // 0. Register signal handlers
        signal(SIGINT, sigint_handler);
        signal(SIGTSTP, sigtstp_handler);
        signal(SIGCHLD, sigchld_handler);

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
        int badinput = 0;
        int redir_out = 0;
        int redir_in = 0;
        int redir_err = 0;
        int pipes = 0;
        char *infile, *outfile, *errfile;
        int i = 0;
        int i2 = 0;
        int fds[2];

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


        //delete_job(jobs_list_head, NULL, 2);


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

        //proc_t proc1 = {parsed_input[0], parsed_input, NULL, NULL, NULL, 0, 0};

        //look for redirects and pipes
        for(int j = 0; j < i; j++){
            if(!strcmp(parsed_input[j], "<")){
                redir_in = 1;
                parsed_input[j] = NULL;
                if(j+1 >= i){
                    badinput = 1;
                }
                else{
                    infile = parsed_input[j+1];
                }
            }
            else if(!strcmp(parsed_input[j], ">")){
                redir_out = 1;
                parsed_input[j] = NULL;
                if(j+1 >= i){
                    badinput = 1;
                }
                else{
                    outfile = parsed_input[j+1];
                }
            }
            else if(!strcmp(parsed_input[j], "2>")){
                redir_err = 1;
                parsed_input[j] = NULL;
                if(j+1 >= i){
                    badinput = 1;
                }
                else{
                    errfile = parsed_input[j+1];
                }
            }
            else if(!strcmp(parsed_input[j], "|")){
                if(j+1 >= i){
                    badinput = 1;
                }
                else{
                    i2 = j+1;
                    pipes = 1;
                    parsed_input[j] = NULL;
                    break;
                }
            }
        }

        //printf("a");

        rp_sigs input_sigs = {badinput, redir_out, redir_in, redir_err, pipes, infile, outfile, errfile};

        //TODO: need to put this in an actual function (works)
//        if(badinput == 0){
//            if(redir_in){
//                open(infile, O_RDONLY);
//            }
//            if(redir_out){
//                dup2(open(outfile, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH), STDOUT_FILENO);
//            }
//        }
        //execvp(parsed_input[0], parsed_input);

        // 4. Determine the number of children processes to create (number of
        // times to call fork) (call fork once per child) (right now this will
        // just be one)
        proc_t proc1 = {parsed_input[0], parsed_input,pipes, 0};
        proc_t proc2 = {(parsed_input+i2)[0], parsed_input+i2,0, pipes};
        rp_sigs input_sigs2 = {0, 0, 0, 0, 0, NULL, NULL, NULL};
        if(pipes){
            pipe(fds);
            //printf("aaa");
            for(int j = i2; j < i; j++){
                if(!strcmp(parsed_input[j], "<")){
                    input_sigs2.sredir_in = 1;
                    parsed_input[j] = NULL;
                    if(j+1 >= i){
                        input_sigs2.sbadinput = 1;
                    }
                    else{
                        input_sigs2.sinfile = parsed_input[j+1];
                    }
                }
                else if(!strcmp(parsed_input[j], ">")){
                    input_sigs2.sredir_out = 1;
                    parsed_input[j] = NULL;
                    if(j+1 >= i){
                        input_sigs2.sbadinput = 1;
                    }
                    else{
                        input_sigs2.soutfile = parsed_input[j+1];
                    }
                }
                else if(!strcmp(parsed_input[j], "2>")){
                    input_sigs2.sredir_err = 1;
                    parsed_input[j] = NULL;
                    if(j+1 >= i){
                        input_sigs2.sbadinput = 1;
                    }
                    else{
                        input_sigs2.serrfile = parsed_input[j+1];
                    }
                }
                else if(!strcmp(parsed_input[j], "|")){
                    printf("aaa");
                    if(j+1 >= i){
                        input_sigs2.sbadinput = 1;
                    }
                    else{
                        i2 = j+1;
                        input_sigs2.spipe = 1;
                        //parsed_input[j] = NULL;
                        break;
                    }
                }
            }
        }
        if(badinput | input_sigs2.sbadinput){
            continue;
        }

        // 5. Execute the commands using execvp or execlp - e.g. execOneChild()
        // or execTwoChildren()

        if(fg_sig){exec_fg();}
        if(bg_sig){exec_bg();}
        if(jobs_sig){exec_jobs();}
        if(bgjobs_sig){exec_bgjobs();}

        pid_t proc1_id = giveBirth(&proc1, input_sigs, fds, -1), proc2_id;

        //printf("after proess 1\n%d\n%d\n%d\n%d\n", getpgrp(), getpid(), tcgetpgrp(STDIN_FILENO), proc1_id);
        setpgid(proc1_id, 0);
        if(pipes){
            close(fds[1]);
            proc2_id = giveBirth(&proc2, input_sigs2, fds, proc1_id);
        }
        //printf("after proess 2\n%d\n%d\n%d\n%d\n", getpgrp(), getpid(), tcgetpgrp(STDIN_FILENO), proc1_id);

        //pid_t proc2_id = pipes ? giveBirth(&proc2, input_sigs2, fds, proc1_id) : -1;

        // 6. NOTE: There are other steps for job related stuff but good luck
        // we won't spell it out for you

        //More...

        free(input);
        free(parsed_input);
        //close(fds[0]);
        //close(fds[1]);
        int status;
        waitpid(-proc1_id, &status, WUNTRACED);
    }

    return 0;
}