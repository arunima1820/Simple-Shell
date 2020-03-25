// Arunima Mittra
// AXM170025
// CS 3377.0W1
// Programming Assignment 3

#include <iostream>
#include <string>
#include <unistd.h>
#include <string.h>

using namespace std;

char **split(char *line)
{
    string arg;
    arg = strtok(line, " ");
    // TO DO create a string array in char**
    // return pointer to first index
    return nullptr;
}

int execute()
{
    // FML
}

void run()
{
    char *line;  // string
    char **args; // string array
    int status;  //exit status

    do
    {
        printf("> ");
        cin >> line;
        args = split(line);
        status = execute();

        line = nullptr;
        args = nullptr;
    } while (status);
}

int main(int argc, char **argv)
{
    printf("******** Custom shell ********\n");
    sleep(1);
    run();

    return 0;
}
