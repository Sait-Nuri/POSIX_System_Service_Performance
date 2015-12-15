SOURCE = main.c 
CFLAGS = -Wall 
LIBS = 
OBJS = main.o
CC = gcc
OUT_DIR := ../../build/mem_alloc_test
EXE = mem_alloc_test

all: file $(EXE) 

file:
	mkdir -p $(OUT_DIR)

mem_alloc_test: main.o 
	$(CC) main.o -o mem_alloc_test $(LIBS)
	mv mem_alloc_test $(OUT_DIR)/mem_alloc_test

main.o: main.c 
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o 
	rm -f $(OUT_DIR)/*