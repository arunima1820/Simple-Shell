#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/* FUNCTION DECLARATIONS */

void parse(char *, char **);
void execute(char **argv);
void pipeFunc(/* char *cmd1[], char *cmd2[] */);
void parsePipe();

/* -------------------------------------------------- */

void parse(char *line, char **argv)
{

    // while not end of line
    while (*line != 0)
    {

        // replace whitespaces with 0
        while (*line == ' ' || *line == '\t' || *line == '\n')
        {
            *line++ = '\0';
        }

        // save argument position
        *argv++ = line;

        // skip the argument until ...
        while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n')
        {
            line++;
        }
    }

    // end of argument list
    *argv = '\0';
}

/* -------------------------------------------------- */
void parsePipe(char *line)
{

    // initialize 3 command arrays
    char *cmd1[3];
    char *cmd2[3];
    char *cmd3[3];

    // after parsing both pipe commands, parse
    pipeFunc(cmd1, cmd2);
}

/* -------------------------------------------------- */

void pipeFunc(/* char *cmd1[], char *cmd2[] */)
{
    pid_t pid1, pid2, pid3;
    int old_pipe[2];
    int new_pipe[2];

    //test command

    //char *argv1[] = {"ls", "-1", "-h", NULL};
    char *argv1[] = {"find", ".", "-name", "*.*", NULL};
    char *argv2[] = {"sed", "s/.*\\.//", NULL};
    char *argv3[] = {"sort", NULL};

    //creates a pipe
    pipe(old_pipe);
    pipe(new_pipe);

    pid1 = fork();
    if (pid1 == 0)
    {
        dup2(old_pipe[1], 1);
        close(old_pipe[0]);

        execvp(argv1[0], argv1);
        perror("exec");
        exit(1);
    }

    pid2 = fork();
    if (pid2 == 0)
    {
        dup2(old_pipe[0], 0);
        close(old_pipe[1]);
        dup2(new_pipe[1], 1);
        close(new_pipe[0]);

        execvp(argv2[0], argv2);
        perror("exec");
        exit(1);
    }

    pid3 = fork();
    if (pid3 == 0)
    {
        dup2(new_pipe[0], 0);
        close(new_pipe[1]);

        execvp(argv3[0], argv3);
        perror("exec");
        exit(1);
    }

    close(old_pipe[0]);
    close(old_pipe[1]);

    close(new_pipe[0]);
    close(new_pipe[1]);

    waitpid(pid1);
    waitpid(pid2);
    waitpid(pid3);

    return;
}

/* -------------------------------------------------- */

void execute(char **argv)
{
    pid_t pid;
    int status;

    if ((pid = fork()) < 0)
    {
        // fork a child process
        printf("ERROR: Forking child process failed.\n");
        exit(1);
    }
    else if (pid == 0)
    {
        // child process
        if (execvp(*argv, argv) < 0)
        {
            printf("ERROR: Execution failed.\n");
            exit(1);
        }
    }
    else
    {
        // parent process
        while (wait(&status) != pid)
            ;
    }
}

/* --------------- main program ------------------ */

void main(void)
{
    char line[1024]; // input line
    char *argv[64];  // command line argument

    // get hostname
    char hostn[1204] = "";
    gethostname(hostn, sizeof(hostn));

    printf("******** Custom shell ********\n");

    while (1) // repeat till "exit"
    {
        // print "user@hostname >" for shell prompt
        printf("%s@%s~$ ", getenv("LOGNAME"), hostn);
        gets(line);
        printf("\n");
        parse(line, argv);

        // check if user entered "exit"
        if (strcmp(argv[0], "exit") == 0)
            exit(0);

        // execute command(s)
        // if (/* TODO line contains | */)
        //     parsePipe(line);
        // else
        execute(argv);
    }
}
