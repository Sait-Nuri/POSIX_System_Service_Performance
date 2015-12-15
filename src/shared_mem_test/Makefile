SOURCE = main.c 
CFLAGS = -Wall 
LIBS = -lrt
OBJS = main.o
CC = gcc
OUT_DIR := ../../build/shared_mem_test
EXE = shared_mem_test

all: file $(EXE) 

file:
	mkdir -p $(OUT_DIR)

shared_mem_test: main.o 
	$(CC) main.o -o shared_mem_test $(LIBS)
	mv shared_mem_test $(OUT_DIR)/shared_mem_test

main.o: main.c 
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o 
	rm -f $(OUT_DIR)/*