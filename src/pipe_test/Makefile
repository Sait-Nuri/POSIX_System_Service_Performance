SOURCE = main.c 
CFLAGS = -Wall 
LIBS = -lrt
OBJS = main.o
CC = gcc
OUT_DIR := ../../build/pipe_test
EXE = pipe_test

all: file $(EXE) 

file:
	mkdir -p $(OUT_DIR)

pipe_test: main.o 
	$(CC) main.o -o pipe_test $(LIBS)
	mv pipe_test $(OUT_DIR)/pipe_test

main.o: main.c 
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o 
	rm -f $(OUT_DIR)/*