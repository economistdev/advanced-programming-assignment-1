CC=gcc
CFLAGS=-I.

%.o: %.c
	@$(CC) -c -o $@ $< $(CFLAGS)

all: columnar_transposition.o
	@$(CC) -o columnar_transposition columnar_transposition.o $(CFLAGS)
	./columnar_transposition
	@$(MAKE) --no-print-directory clean

clean:
	@rm -f *.o columnar_transposition

.PHONY: all clean
