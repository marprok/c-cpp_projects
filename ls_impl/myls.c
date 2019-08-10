#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>

static size_t total;

static void printRights(struct stat *st)
{
    if (!st)
		return;
  
    /* Owner */
    if (st->st_mode & S_IRUSR)
		printf("r");
    else
		printf("-");

    if (st->st_mode & S_IWUSR)
		printf("w");
    else
		printf("-");
  
    if (st->st_mode & S_IXUSR)
		printf("x");
    else
		printf("-");

    /* Group */
    if (st->st_mode & S_IRGRP)
		printf("r");
    else
		printf("-");

    if (st->st_mode & S_IWGRP)
		printf("w");
    else
		printf("-");
  
    if (st->st_mode & S_IXGRP)
		printf("x");
    else
		printf("-");

    /* Others */
    if (st->st_mode & S_IROTH)
		printf("r");
    else
		printf("-");

    if (st->st_mode & S_IWOTH)
		printf("w");
    else
		printf("-");
  
    if (st->st_mode & S_IXOTH)
		printf("x");
    else
		printf("-");

}

static void printUserGroup(struct stat *st)
{
	struct passwd *pw;
    struct group *gr;

	if (!st)
		return;

    pw = getpwuid(st->st_uid);
    if (!pw)
    {
		perror("ERROR: getpwuid\n");
		return;
    }

    gr = getgrgid(st->st_gid);
    if (!gr)
    {
		perror("ERROR: getgrgid\n");
		return;
    }
  
    printf(" %s %s", pw->pw_name, gr->gr_name);
}

static void printtime(struct stat *st)
{
	struct tm *t = localtime(&st->st_mtime);
	char buf[255];
    if (!st)
		return;
    if (!t)
    {
		fprintf(stderr, "ERROR: localtime\n");
		return;
    }
    strftime(buf, 255, "%b %d %H:%M", t);
    printf(" %s", buf);
}

static void printsize(struct stat *st)
{
    if (!st)
		return;
  
    printf(" %ld", st->st_size);
}

static void printhardlinks(struct stat *st)
{
    if (!st)
		return;

    printf(" %lu", st->st_nlink);
}

static void printInfo(const char *name, const char *path, unsigned char isDir)
{
    struct stat buf;
    char fullpath[255];

    sprintf(fullpath, "%s/%s", path, name);
    if (stat(fullpath, &buf) < 0)
    {
		perror("ERROR: stat\n");
		return;
    }
  
    if (isDir)
		printf("d");
    else
		printf("-");

    /*
      Aparently the "total" line in the ls -la command means the total number of 1024 byte 
      in the directory. The st_blocks field is the number of 512 byte blocks, so the division by
      two should calculate the correct total(but it doesn't).
    */
    total += buf.st_blocks/2;
    printRights(&buf);
    printhardlinks(&buf);
    printUserGroup(&buf);
    printsize(&buf);
    printtime(&buf);
    printf(" %s\n", name);
}

int main(int argc, char **argv)
{
    DIR *dir;
    struct dirent *drt;
    unsigned char isDir;

    if (argc != 2)
    {
		fprintf(stderr, "USAGE: ./executable path\n");
		return 1;
    }
    dir = opendir(argv[1]);
    if (!dir)
    {
		fprintf(stderr, "ERROR: opendir\n");
		return 1;
    }

  
    while ((drt = readdir(dir)))
    {
		if (drt->d_type != DT_DIR)
		{
			/* This is not a directory */
			isDir = 0;
		}else
			isDir = 1;

		printInfo(drt->d_name, argv[1], isDir);
    }

    printf("Total %zu\n", total);
  
    closedir(dir);
    return 0;
}
