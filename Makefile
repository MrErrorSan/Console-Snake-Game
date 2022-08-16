#variable for compiler
CC=gcc
all:	snake.o random.o terminal.o
	$(CC) snake.o random.o terminal.o -o snake 
snake.o:	snake.c terminal.h random.h
	$(CC) -c snake.c
random.o:	random.c random.h
	$(CC) -c random.c
terminal.o:	terminal.c terminal.h
	$(CC) -c terminal.c