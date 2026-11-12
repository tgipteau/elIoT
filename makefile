CFLAGS = -Wall -Wextra


client : client.o
	clang -o client client.o
server : server.o
	clang -o server server.o
server.o : server.c
	clang -c server.c -o server.o
client.o : client.c
	clang -c client.c -o client.o
