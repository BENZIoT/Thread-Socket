all: server.o client.o
	gcc server.o -o server
	gcc client.o -o client

server.o: server.c
	gcc -c server.c -o server.o

client.o: client.c
	gcc -c client.c -o client.o

clean:
	rm -f *.o server client echo.txt