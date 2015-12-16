SOURCE = main.c client.c
CFLAGS = -Wall 
LIBS = -lrt
OBJS = main.o client.o
CC = gcc
OUT_DIR := ../../build/shared_mem_test
EXE = main client

all: file $(EXE)

file:
	mkdir -p $(OUT_DIR)

main: main.o 
	$(CC) main.o -o main $(LIBS)
	mv main $(OUT_DIR)/main

client: client.o 
	$(CC) client.o -o client $(LIBS)
	mv client $(OUT_DIR)/client

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

clean:
	rm -f *.o
	rm -f $(OUT_DIR)/*


