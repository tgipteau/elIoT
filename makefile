CC = clang

# BROKER
broker : broker.o ressources/mongoose.o
	clang -o broker broker.o ressources/mongoose.o
broker.o : broker.c
	clang -c broker.c -o broker.o

# BRIDGE
bridge : bridge.o ressources/mongoose.o
	$(CC) -o bridge bridge.o ressources/mongoose.o

bridge.o : bridge.c mongoose.h
	$(CC) -c bridge.c -o bridge.o

clean :
	rm *.o
