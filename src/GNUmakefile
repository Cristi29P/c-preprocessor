CC = gcc
CFLAGS = -Wall -Wextra -g

build: so-cpp

so-cpp: so-cpp.o linkedlist.o hashmap.o utils.o

so-cpp.o: so-cpp.c

linkedlist.o: linkedlist.c

hashmap.o: hashmap.c

utils.o: utils.c

.PHONY: clean

clean:
	rm -f *.o *~ so-cpp