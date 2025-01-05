make:
	cc server.c -o server
	cc client.c -o client
clean:
	rm -f server client
