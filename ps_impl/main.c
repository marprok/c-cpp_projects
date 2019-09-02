#include "readProcessInfo.h"
#include <stdio.h>

int main(int argc, char** argv)
{
    if (argc == 2)
    {
        readProcStat(argv[1]);
        readProcStatm(argv[1]);
        readProcCmdLine(argv[1]);
    }
    else
        readAllProcesses();
    // printf("%s size: %i\n", path, s);
    return 0;
}
