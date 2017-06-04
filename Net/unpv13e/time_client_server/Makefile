CFLAGS += -O0 -g3 -gdwarf-4 -Wall -Werror
TARGET = a.out.client a.out.server a.out.client.ipv6 a.out.server.ipv6

all: $(TARGET)

a.out.client: time_client.c
	$(CC) $(CFLAGS) time_client.c -o a.out.client

a.out.server: time_server.c
	$(CC) $(CFLAGS) time_server.c -o a.out.server

a.out.client.ipv6: time_client_ipv6.c
	$(CC) $(CFLAGS) time_client_ipv6.c -o a.out.client.ipv6

a.out.server.ipv6: time_server_ipv6.c
	$(CC) $(CFLAGS) time_server_ipv6.c -o a.out.server.ipv6

clean:
	rm $(TARGET)

.PHONY: all clean
