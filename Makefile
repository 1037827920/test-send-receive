CFLAGS = -Wall -O2

all: send-test receive-test

send-test: send-test.c
	$(CC) $(CFLAGS) $< -o $@

receive-test: receive-test.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f send-test receive-test

.PHONY: all clean