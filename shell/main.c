#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

/*
  Another pretty basic shell that I made to test the tokenizers I made.
  Does not support I/O redirection just yet.
*/

#define IN 0
#define OUT 1

int execute(const bucket_t *program, size_t index, size_t size, int pipe[6][2])
{
    int ret;
    if ((ret = fork()) < 0)
    {
        fprintf(stderr, "fork failed: %s\n", strerror(errno));
    }
    else if (ret)
    {
        int status;
        /* Close the pipe when all reading is done. */
        if (pipe && index > 0)
        {
            close(pipe[index-1][IN]);
            close(pipe[index-1][OUT]);
        }
        waitpid(ret, &status, 0);
        
    }else
    {
        if (pipe && index > 0)
        {
            dup2(pipe[index - 1][IN], 0);
            close(pipe[index - 1][OUT]);
        }

        if (pipe && index != size - 1)
        {
            dup2(pipe[index][OUT], 1);
        }
        
        if ( (ret = execvp(program->m_bucket[0], program->m_bucket)) < 0)
        {
            fprintf(stderr, "execvp failed: %s\n", strerror(errno));
        }
    }
    return ret;
}

char* show_prompt(char *buffer, int size, const char *prompt)
{
    
    printf("%s", prompt);
    return fgets(buffer, size, stdin);
}

#define MAX_LENGTH 256
int main(int argc, char **argv)
{
    (void)argc; (void)argv;
    bucket_t commands;
    bucket_t program;
    char cmd[MAX_LENGTH];

    while (show_prompt(cmd, MAX_LENGTH, "shell>"))
    {
        create_bucket(&commands, cmd, "|");
        if (commands.m_size > 1)
        {            
            size_t i;
            int pipe_array[6][2];
            for (i = 0; commands.m_bucket[i]; ++i)
            {
                int ret;
                if (i != commands.m_size-1)
                    pipe(pipe_array[i]);
                create_bucket(&program, commands.m_bucket[i], " ");
                if ((ret = execute(&program, i, commands.m_size, pipe_array)) < 0)
                {
                    exit(EXIT_FAILURE);
                }
                reset_bucket(&program);
            }
        }else
        {
            int ret;
            create_bucket(&program, commands.m_storage[0], " ");
            if ((ret = execute(&program, 0, 1, NULL)) < 0)
            {
                exit(EXIT_FAILURE);
            }
            reset_bucket(&program);
        }
        reset_bucket(&commands);
    }
    return 0;
}
