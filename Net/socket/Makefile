pc:
	cc client.c -o client
	cc server.c -o server

arm:
	/Application/arm-2009q1/bin/arm-none-linux-gnueabi-gcc client.c -o arm-client
	/Application/arm-2009q1/bin/arm-none-linux-gnueabi-gcc server.c -o arm-server

install:
	cp arm-client /NFS/
	cp arm-server /NFS/

clean:
	rm -f arm-client
	rm -f arm-server
	rm -f client
	rm -f server

.PHONY: pc arm install clean

