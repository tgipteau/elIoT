CC = clang

bridge : bridge.o mongoose.o
	$(CC) -o bridge bridge.o mongoose.o

bridge.o : bridge.c mongoose.h
	$(CC) -c bridge.c -o bridge.o

mongoose : mongoose.o mongoose.h
	$(CC) -c mongoose.c -o mongoose.o
