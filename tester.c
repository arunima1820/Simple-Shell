#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

/* FUNCTION DECLARATIONS */

void parse(char *, char **);
void parseSequence(char *line);
void execute(char **argv);
void runPipe(char *cmd1[], char *cmd2[], char *cmd3[]);
void parsePipe();

/* ------------------ parse function ----------------- */

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

/* ------------------- parse pipe -------------------- */
void parsePipe(char line[])
{

    // initialize 3 command arrays
    // char *cmd1[3] = {"ls", "-l", NULL};
    // char *cmd2[3] = {"grep", "myshell.c", NULL};
    // char *cmd3[3] = {"awk", "{print $1}", NULL};

    char *cmd1[3];
    char *cmd2[3];
    char *cmd3[3];

    char *cmd;
    cmd = strtok(line, "|");
    while (cmd != NULL)
    {
        cmd =
    }

    // after parsing both pipe commands, parse
    runPipe(cmd1, cmd2, cmd3);
}

/* ------------------ run pipe ----------------------- */

void runPipe(char *cmd1[], char *cmd2[], char *cmd3[])
{
    int status;
    pid_t pid;

    int pipefds[4];
    pipe(pipefds);
    pipe(pipefds + 2);

    if (fork() == 0)
    {
        dup2(pipefds[1], 1);

        close(pipefds[0]);
        close(pipefds[1]);
        close(pipefds[2]);
        close(pipefds[3]);

        execvp(*cmd1, cmd1);
    }
    else
    {
        if (fork() == 0)
        {
            dup2(pipefds[0], 0);
            dup2(pipefds[3], 1);

            close(pipefds[0]);
            close(pipefds[1]);
            close(pipefds[2]);
            close(pipefds[3]);

            execvp(*cmd2, cmd2);
        }
        else
        {
            if (fork() == 0)
            {
                dup2(pipefds[2], 0);

                close(pipefds[0]);
                close(pipefds[1]);
                close(pipefds[2]);
                close(pipefds[3]);

                execvp(*cmd3, cmd3);
            }
        }
    }

    close(pipefds[0]);
    close(pipefds[1]);
    close(pipefds[2]);
    close(pipefds[3]);

    for (int i = 0; i < 3; i++)
    {
        wait(&status);
    }
}

/* ------------------- execute ----------------------- */

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

/* ------------------- main program ------------------ */

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
        if (strchr(line, '|') != NULL)
            parsePipe(line);
        else if (strchr(line, ';') != NULL)
            parseSequence(line);
        else
            execute(argv);
    }
}
