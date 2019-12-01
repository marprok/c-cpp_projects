#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
/*
  This program is a scheduler. It creates 3 processes and shedules them so that
  they are executed in the order: proc3, proc2, proc1. The duration that each
  process
  is executed for, is specified by the command line arguments in the order:
  proc1_time,
  proc2_time, proc3_time.

*/

#define PROC_NUM 3

static int id = 1; /* The id of each of child process(Not the pid). */
static int time[PROC_NUM]; /* The duration that the parent process must sleep. */
static int start = 0; /* The flag that indicates whether to start working or not. */

/* The signal handler. */
void sig_handler(int sid)
{
    if (sid == SIGUSR1)
        start = 1;
    else if (sid == SIGUSR2)
        start = 0;
    signal(sid, sig_handler);
}

int main(int argc, char** argv)
{
    int pid = 0;
    int pids[] = { 0, 0, 0 };
    int total_work = 10;
    /* Argument validation test. */
    if (argc != 4)
    {
        const char* msg = "Usage: ./a.out int1 int2 int3\n";
        write(2, (void*)msg, strlen(msg) + 1);
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i <= PROC_NUM; ++i)
    {
        char* c = argv[i];
        while (*c != '\0')
        {
            if (*c > '9' || *c < '0')
            {
                const char* msg = "All arguments must be integers!\n";
                write(2, (void*)msg, strlen(msg) + 1);
                exit(EXIT_FAILURE);
            }
            c++;
        }
        time[i - 1] = atoi(argv[i]);
    }

    /* Setup the signal callbacks. */
    signal(SIGUSR1, sig_handler);
    signal(SIGUSR2, sig_handler);
    /* Start the processes. */
    for (; id <= PROC_NUM; id++)
    {
        if ((pid = fork()) != 0)
        {
            /* Parent */
            pids[id - 1] = pid;
        }
        else
        {
            /* Child */
            while (total_work > 0)
            {
                if (!start)
                    pause();
                printf("%d\n", id);
                total_work--;
                sleep(1);
            }
            exit(EXIT_SUCCESS);
        }
    }

    if (pid)
    {
        printf("Sleeping for 3 secs...");
        fflush(stdout);
        sleep(3);
        printf("Done!\n");
        /* Parent */
        int ids[] = { PROC_NUM, PROC_NUM - 1, PROC_NUM - 2 };
        int remain[] = { total_work, total_work, total_work };
        int j = 0;
        unsigned char remaining_porcs = 0;
        /* While there is at least one process still having work to do. */
        while (1)
        {
            if (remain[j] > 0)
            {
                kill(pids[ids[j] - 1], SIGUSR1);
                sleep(time[ids[j] - 1]);
                remain[j] -= time[ids[j] - 1];
                kill(pids[ids[j] - 1], SIGUSR2);
            }
            else
                remaining_porcs |= 0x1 << j;

            if (remaining_porcs == 7)
                break;
            j = (j + 1) % PROC_NUM;
        }
    }
    exit(EXIT_SUCCESS);
}
