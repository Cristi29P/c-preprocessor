CC = gcc
CFLAGS = -Wall -Wextra -g

build: so-cpp

so-cpp: so-cpp.o linkedlist.o hashmap.o utils.o cmd_line.o so_cpp_utils.o

so-cpp.o: so-cpp.c

linkedlist.o: linkedlist.c

hashmap.o: hashmap.c

cmd_line.o: cmd_line.c

so_cpp_utils.o: so_cpp_utils.c

utils.o: utils.c

.PHONY: clean

clean:
	rm -f *.o *~ so-cpp