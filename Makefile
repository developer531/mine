# Console Minesweeper
# Makefile
# Builds program
CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror -Wpedantic

all: bin/mine

debug: CFLAGS += -O0 -g
debug: bin/mine

release: CFLAGS += -O3
release: bin/mine

bin/mine: obj/main.o obj/grid.o obj/ui.o
	$(CC) $(CFLAGS) -o bin/mine obj/main.o obj/grid.o obj/ui.o -lncurses

obj/main.o: src/main.c
	$(CC) $(CFLAGS) -o obj/main.o src/main.c -c

obj/grid.o: src/grid.c
	$(CC) $(CFLAGS) -o obj/grid.o src/grid.c -c

obj/ui.o: src/ui.c
	$(CC) $(CFLAGS) -o obj/ui.o src/ui.c -c
	
clean:
	rm obj/* bin/*
