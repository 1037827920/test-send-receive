CFLAGS = -Wall -O2

all: send-test receive-test tcp-server tcp-client

send-test: send-test.c
	$(CC) $(CFLAGS) $< -o $@

receive-test: receive-test.c
	$(CC) $(CFLAGS) $< -o $@

tcp-server: tcp-server.c
	$(CC) $(CFLAGS) $< -o $@

tcp-client: tcp-client.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f send-test receive-test tcp-server tcp-client

.PHONY: all clean