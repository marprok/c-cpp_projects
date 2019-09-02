#include "readProcessInfo.h"
#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct infostring
{
	char* str;
	int size;
};

void appendinfo(struct infostring* info, const char* str, int size)
{
	int t = info->size + size;
	char* temp;
	/* Realloc an extra byte for the NULL termination char. */
	temp = realloc(info->str, t + 1);
	if (!temp)
		return;
	memcpy(temp + info->size, str, size);
	info->size = t;
	temp[t] = '\0';
	info->str = temp;
}

void freeinfostring(struct infostring* info)
{
	info->size = 0;
	if (info->str)
		free(info->str);
	info->str = NULL;
}

enum fields
{
	PID = 1,
	EXECNAME,
	STATE,
	UTIME = 14,
	STIME
};

int readAllProcesses()
{
	DIR* dir = opendir("/proc");

	if (!dir)
	{
		perror("Cant read the /proc directory");
		return -1;
	}

	struct dirent* entry;

	while ((entry = readdir(dir)) != NULL)
	{
		if (isdigit(*(entry->d_name)))
		{
			readProcCmdLine(entry->d_name);
			readProcStat(entry->d_name);
			readProcStatm(entry->d_name);
		}
	}

	closedir(dir);
	return 0;
}

int readProcCmdLine(const char* pid)
{
	FILE* file;
	struct infostring path = { NULL, 0 };

	appendinfo(&path, "/proc/", 6);
	appendinfo(&path, pid, strlen(pid));
	appendinfo(&path, "/cmdline", 8);

	file = fopen(path.str, "r");

	if (!file)
	{
		perror("Can't open the process file(cmdline)");
		return -1;
	}

	struct infostring info = { NULL, 0 };
	char c;

	while (!feof(file))
	{
		fread(&c, 1, sizeof(char), file);
		// if (c == '\0')
		// c = ' ';
		appendinfo(&info, &c, 1);
	}

	printf("Process command: %s\n", info.str);

	freeinfostring(&path);
	freeinfostring(&info);
	fclose(file);

	return 0;
}

/* Returns 0 on success and -1 on failure. */
int readProcStatm(const char* pid)
{
	FILE* file;
	struct infostring path = { NULL, 0 };

	appendinfo(&path, "/proc/", 6);
	appendinfo(&path, pid, strlen(pid));
	appendinfo(&path, "/statm", 6);

	file = fopen(path.str, "r");

	if (!file)
	{
		perror("Can't open the process file(statm)");
		return -1;
	}

	struct infostring info = { NULL, 0 };
	char c;

	while (!feof(file))
	{
		fread(&c, 1, sizeof(char), file);
		if (c == ' ')
			break;
		appendinfo(&info, &c, 1);
	}

	printf("Virtual memory size: %s\n", info.str);

	freeinfostring(&path);
	freeinfostring(&info);
	fclose(file);

	return 0;
}

/* Returns 0 on success and -1 on failure. */
int readProcStat(const char* pid)
{
	FILE* file;
	struct infostring path = { NULL, 0 };

	appendinfo(&path, "/proc/", 6);
	appendinfo(&path, pid, strlen(pid));
	appendinfo(&path, "/stat", 5);

	file = fopen(path.str, "r");

	if (!file)
	{
		perror("Can't open the process file(stat)");
		return -1;
	}

	struct infostring info = { NULL, 0 };
	int element = 0;
	char c;

	while (!feof(file))
	{
		fread(&c, 1, sizeof(char), file);
		appendinfo(&info, &c, 1);
		if (info.str[info.size - 1] == ' ')
		{
			element++;
			switch (element)
			{
			case PID:
				printf("PID: %s\n", info.str);
				break;
			case EXECNAME:
				printf("Process name: %s\n", info.str);
				break;
			case STATE:
				printf("Process state: %s\n", info.str);
				break;
			case UTIME:
				printf("Process user jiffies: %s\n", info.str);
				break;
			case STIME:
				printf("Process system jiffies: %s\n", info.str);
				break;
			}
			freeinfostring(&info);
		}
	}

	freeinfostring(&path);
	freeinfostring(&info);
	fclose(file);

	return 0;
}
