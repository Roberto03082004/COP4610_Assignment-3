CC = gcc
CFLAGS = -Wall -Wextra

all: memory_allocation

memory_allocation: memory_allocation.c
	$(CC) $(CFLAGS) memory_allocation.c -o memory_allocation

run: memory_allocation
	./memory_allocation

clean:
	rm -f memory_allocation

