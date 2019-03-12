#ifndef READ_PROCESSES_INFO_H
#define READ_PROCESSES_INFO_H

int readProcStat(const char *pid);
int readProcStatm(const char *pid);
int readProcCmdLine(const char *pid);
int readAllProcesses();

#endif
