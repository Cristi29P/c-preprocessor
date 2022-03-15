CC = gcc
CFLAGS = -Wall -Wextra -g

#build: test

#test: test.o linkedlist.o hashmap.o

#test.o: test.c

build: so-cpp

so-cpp: so-cpp.o linkedlist.o hashmap.o

so-cpp.o: so-cpp.c

linkedlist.o: linkedlist.c

hashmap.o: hashmap.c

.PHONY: clean

clean:
	rm -f *.o *~ test so-cpp