SOURCE = writer.c reader.c
CFLAGS = -Wall 
LIBS = -lrt
OBJS = writer.o reader.o
CC = gcc
OUT_DIR := ../../build/fifo_test
EXE = fifo_writer fifo_reader

all: file $(EXE) 

file:
	mkdir -p $(OUT_DIR)

fifo_writer: writer.o 
	$(CC) writer.o -o fifo_writer $(LIBS)
	mv fifo_writer $(OUT_DIR)/fifo_writer

fifo_reader: reader.o 
	$(CC) reader.o -o fifo_reader $(LIBS)
	mv fifo_reader $(OUT_DIR)/fifo_reader

writer.o: writer.c 
	$(CC) $(CFLAGS) -c writer.c

reader.o: reader.c
	$(CC) $(CFLAGS) -c reader.c

clean:
	rm -f *.o 
	rm -f $(OUT_DIR)/*