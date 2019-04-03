all: a.out

a.out: test.cpp

.PHONY: clean

clean:
	rm -f a.out
