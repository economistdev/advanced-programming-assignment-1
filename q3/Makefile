CC=gcc
CFLAGS=-I.

%.o: %.c
	@$(CC) -c -o $@ $< $(CFLAGS)

all: redact_words.o
	@$(CC) -o redact_words redact_words.o $(CFLAGS)
	./redact_words
	@$(MAKE) --no-print-directory clean

clean:
	@rm -f *.o redact_words

.PHONY: all clean
