all: a.out

CC=g++
CPPFLAGS=-O3

a.out: test.cpp
	$(CC) $(CPPFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f a.out
