// Arunima Mittra
// AXM170025
// CS 3377.0W1
// Programming Assignment 3

#include <iostream>
#include <string>
#include <unistd.h>
#include <regex>

using namespace std;

vector<string> split(string line)
{
    string command;     // stores the individual command
    vector<string> vec; // array of all the '|' and ';' separated commands

    for (int i = 0; i < line.length(); i++)
    {
        if (line.at(i) == '|' || line.at(i) == ';')
        {
            vec.push_back(command);
            command = nullptr;
            continue;
        }
        else
        {
            command += line.at(i);
        }
    }

    return vec;
}

int execute(vector<string> args)
{
    for (auto i : args)
        printf("%s, ", i);
    return 0;
}

// runs each line of the terminal
void run()
{
    char *line;          // string
    vector<string> args; // string array
    int status;          //exit status

    do
    {
        printf("> ");
        cin >> line;
        args = split(line);
        status = execute(args);

        line = nullptr;
        args.clear();
    } while (status);
}

int main(int argc, char **argv)
{
    printf("******** Custom shell ********\n");
    sleep(1);
    run();

    return 0;
}
