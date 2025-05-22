CC=gcc
CFLAGS=-I.

%.o: %.c
	@$(CC) -c -o $@ $< $(CFLAGS)

all: removeDigit.o
	@$(CC) -o removeDigit removeDigit.o $(CFLAGS)
	./removeDigit
	@$(MAKE) --no-print-directory clean

clean:
	@rm -f *.o removeDigit

.PHONY: all clean
