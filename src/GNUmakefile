CC = gcc
CFLAGS = -Wall -Wextra -g

build: so-cpp

so-cpp: so-cpp.o linkedlist.o hashmap.o

so-cpp.o: so-cpp.c

linkedlist.o: linkedlist.c

hashmap.o: hashmap.c

.PHONY: clean

clean:
	rm -f *.o *~ test so-cpp