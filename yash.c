#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include <fork.h>
#include <jobs.h>
#include <pipes.h>
#include <redirection.h>
#include <signals.h>

int main(void)
{
    while (1) {
        // 0. Register signal handlers

        // 1. Print the prompt (#)

        // 2. Grab and parse the input - NOTE: Make sure to remove the newline
        // character from the input string (otherwise, you'll pass "ls\n" to
        // execvp and there is no executable called "ln\n" just "ls")

        char *input = readline();
        char **parsed_input;

        // 3. Check for job control tokens (fg, bg, jobs, &) (for now just
        // ignore those commands)

        // 4. Determine the number of children processes to create (number of
        // times to call fork) (call fork once per child) (right now this will
        // just be one)

        // 5. Execute the commands using execvp or execlp - e.g. execOneChild()
        // or execTwoChildren()

        // 6. NOTE: There are other steps for job related stuff but good luck
        // we won't spell it out for you

        //More...
    }

    return 0;
}