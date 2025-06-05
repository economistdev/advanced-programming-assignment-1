CC=gcc
CFLAGS=-I. -g # IMPORTANT: -g must be here for debugging info

%.o: %.c
	@$(CC) -c -o $@ $< $(CFLAGS)

all: columnar_transposition.o
	@$(CC) -o columnar_transposition columnar_transposition.o $(CFLAGS)
	# This target is for regular builds and runs.
	# It's good to keep the run and clean for standard usage.
	./columnar_transposition
	@$(MAKE) --no-print-directory clean

debug: columnar_transposition.o
	@$(CC) -o columnar_transposition_debug columnar_transposition.o $(CFLAGS)

clean:
	@rm -f *.o columnar_transposition columnar_transposition_debug

.PHONY: all debug_build clean