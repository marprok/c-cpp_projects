CFLAGS=-Wall -Wextra -pedantic -std=c99 -g
CC=gcc


main: main.o tokenizer.o
	$(CC) main.o tokenizer.o -o main
main.o: main.c
	$(CC) $(CFLAGS) -c main.c
tokenizer.o: tokenizer.c tokenizer.h
	$(CC) $(CFLAGS) -c tokenizer.c
clean:
	rm *.o main
