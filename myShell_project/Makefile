CFLAGS=-Wall -Wextra -pedantic -std=c99 -g
CC=gcc


myShell: myShell.o
	$(CC) myShell.o -o myShell
myShell.o: myShell.c
	$(CC) $(CFLAGS) -c myShell.c
clean:
	rm *.o myShell
