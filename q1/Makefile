CC=gcc
CFLAGS=-I.

%.o: %.c
	@$(CC) -c -o $@ $< $(CFLAGS)

all: howManyDays.o
	@$(CC) -o howManyDays howManyDays.o $(CFLAGS)
	./howManyDays
	@$(MAKE) --no-print-directory clean

clean:
	@rm -f *.o howManyDays

.PHONY: all clean
