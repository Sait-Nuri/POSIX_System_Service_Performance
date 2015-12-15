SOURCE = main.c 
CFLAGS = -Wall 
LIBS = -lpthread
OBJS = main.o
CC = gcc
OUT_DIR := ../../build/pthread_test
EXE = pthread_test

all: file $(EXE) 

file:
	mkdir -p $(OUT_DIR)

pthread_test: main.o 
	$(CC) main.o -o pthread_test $(LIBS)
	mv pthread_test $(OUT_DIR)/pthread_test

main.o: main.c 
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o 
	rm -f $(OUT_DIR)/*