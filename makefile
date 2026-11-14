CC = clang

bridge : bridge.o ressources/mongoose.o
	$(CC) -o bridge bridge.o ressources/mongoose.o

bridge.o : bridge.c mongoose.h
	$(CC) -c bridge.c -o bridge.o

clean :
	rm *.o
