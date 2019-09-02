/*
  This program is a simple shell that supports I/O redirection and piping.
  It is based on a team project that I had to do during my Operating Systems
  course at the AUEB but this version was written from scratch with the intent
  of making it more readable. I tried to fit all the code in a single source
  file since it is not a big project at all.

  Author: Marios Prokopakis
*/

#include <ctype.h> /* For isspace */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

char** tokenize(char* str, char* delim, int* total)
{
    size_t token_num = 0; // command length
    char** tokens;
    char* token;
    size_t i;
    char* temp = malloc((strlen(str) + 1) * sizeof(char));

    if (!temp)
    {
        fprintf(stderr, "Error: could not allocate memory for temporay variable!\n");
        return NULL;
    }
    /* copy str into temp because strtok modifies the first argument */
    strcpy(temp, str);
    token = strtok(temp, delim);
    /* take the length of the command */
    while (token)
    {
        token_num++;
        token = strtok(NULL, delim);
    }

    /*const char *runner = str;
      int t = 0;
      while ((runner = strstr(runner, delim)))
      {
      runner++;
      t++;
      }

      t++;
    */
    //  if (token_num == t)
    //{
    // printf("token_num %zu t %i\n", token_num, t);
    //}

    if (total)
        *total = token_num;

    /* +1 because the list must end with NULL */
    tokens = malloc((token_num + 1) * sizeof(char*));
    if (!tokens)
    {
        fprintf(stderr, "Error: could not allocate memory for the tokens!\n");
        free(temp);
        return NULL;
    }
    /* copy str into temp because strtok modifies the first argument */
    strcpy(temp, str);
    token = strtok(temp, delim);

    for (i = 0; i < token_num; ++i)
    {
        tokens[i] = malloc((strlen(token) + 1) * sizeof(char));
        strcpy(tokens[i], token);
        token = strtok(NULL, delim);
    }

    tokens[i] = NULL;
    free(temp);
    return tokens;
}

void tokenize_cleanup(char** tokens)
{
    if (!tokens)
        return;

    char* token;
    int i = 0;

    while ((token = tokens[i++]))
        free(token);

    free(tokens);
}

int io_redirection(char** cmd)
{
    int fds;
    char* filename;

    if (strstr(*cmd, ">>") != NULL)
    {
        /* Take the actual command. */
        *cmd = strtok(*cmd, ">>");
        filename = strtok(NULL, ">>");
        /* Remove leading spaces */
        while (isspace(*filename))
            filename++;

        size_t len = strlen(filename);
        /* Remove trailing spaces */
        while (len > 0 && isspace(filename[len - 1]))
            filename[--len] = '\0';

        fds = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0664);

        if (fds == -1)
        {
            fprintf(stderr, "Could not open file: %s\n", filename);
            return -1;
        }
        dup2(fds, 1);
        close(fds);
    }

    if (strstr(*cmd, ">") != NULL)
    {
        /* Take the actual command. */
        *cmd = strtok(*cmd, ">");
        filename = strtok(NULL, ">");
        /* Remove leading spaces */
        while (isspace(*filename))
            filename++;

        size_t len = strlen(filename);
        /* Remove trailing spaces */
        while (len > 0 && isspace(filename[len - 1]))
            filename[--len] = '\0';

        fds = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0664);

        if (fds == -1)
        {
            fprintf(stderr, "Could not open file: %s\n", filename);
            return -1;
        }
        dup2(fds, 1);
        close(fds);
    }

    char* index = strstr(*cmd, "<");
    if (index)
    {
        /* index - 1 >= 0  TODO */
        if (index - 1 && *(index - 1) != '<' && index + 1 && *(index + 1) != '<')
        {
            /* Take the actual command. */
            *cmd = strtok(*cmd, "<");
            filename = strtok(NULL, "<");
            /* Remove leading spaces */
            while (isspace(*filename))
                filename++;

            size_t len = strlen(filename);
            /* Remove trailing spaces */
            while (len > 0 && isspace(filename[len - 1]))
                filename[--len] = '\0';

            fds = open(filename, O_RDONLY, 0664);
            if (fds == -1)
            {
                fprintf(stderr, "Could not open file: %s\n", filename);
                return -1;
            }
            dup2(fds, 0);
            close(fds);
        }
        else
            return -1;
    }

    return 0;
}

int exec_command(char* cmd, int cid, int ctotal, int** pipes)
{
    int pid;

    if ((pid = fork()) == 0)
    {
        if (pipes)
        {
            if (cid > 0)
            {
                dup2(pipes[cid - 1][0], 0);
                close(pipes[cid - 1][1]);
                close(pipes[cid - 1][0]);
            }

            if (cid != ctotal - 1)
            {
                dup2(pipes[cid][1], 1);
                close(pipes[cid][0]);
                close(pipes[cid][1]);
            }
        }

        if (io_redirection(&cmd) == -1)
        {
            fprintf(stderr, "Error during the redirection.\n");
            exit(EXIT_FAILURE);
        }

        char** command = tokenize(cmd, " ", NULL);

        if (command == NULL)
        {
            fprintf(stderr, "Could not tokenize the command: %s\n", cmd);
            exit(EXIT_FAILURE);
        }

        if (execvp(command[0], command) == -1)
        {
            fprintf(stderr, "Could not excecute the %s command.\n", command[0]);
            tokenize_cleanup(command);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        if (pipes)
        {
            if (cid > 0)
                close(pipes[cid - 1][0]);

            if (cid != ctotal - 1)
                close(pipes[cid][1]);
        }

        int status;
        wait(&status);
    }

    return 0;
}

void parse_command(char* cmd)
{
    if (!cmd)
    {
        fprintf(stderr, "Null cmd\n");
        return;
    }

    int i = 0, ctotal = 0;
    char** commands = tokenize(cmd, "|", &ctotal);
    int** pipes = NULL;

    if (ctotal > 1)
    {
        /* Reserve memory for the pipes */
        pipes = malloc(sizeof(int*) * (ctotal - 1));
        for (int j = 0; j < ctotal - 1; j++)
        {
            pipes[j] = malloc(sizeof(int) * 2);
            pipe(pipes[j]);
        }
    }

    while (commands[i])
    {
        exec_command(commands[i], i, ctotal, pipes);
        i++;
    }

    if (pipes)
    {
        for (int j = 0; j < ctotal - 1; j++)
            free(pipes[j]);

        free(pipes);
    }

    tokenize_cleanup(commands);
}

int main(void)
{
    const int cmdsize = 256;
    char* cmd = malloc(sizeof(char) * cmdsize);
    char* pointer = NULL;

    while (fgets(cmd, cmdsize, stdin))
    {
        if ((pointer = strstr(cmd, "\n")))
            *pointer = '\0';
        parse_command(cmd);
    }

    free(cmd);
    return 0;
}
