CC = gcc
CFLAGS = -Wall -Wextra -Wconversion -g

build: test

test: test.o linkedlist.o

test.o: test.c

linkedlist.o: linkedlist.c

.PHONY: clean

clean:
	rm -f *.o *~ test