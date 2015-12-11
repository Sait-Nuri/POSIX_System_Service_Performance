SOURCE = writer.c reader.c
CFLAGS = -Wall 
LIBS = -lrt
OBJS = writer.o reader.o
CC = gcc
OUT_DIR := ../../build/fifo_test
EXE = writer reader

all: $(EXE) file

file:
	mkdir -p $(OUT_DIR)

writer: writer.o 
	$(CC) writer.o -o writer $(LIBS)
	mv writer $(OUT_DIR)/writer

reader: reader.o 
	$(CC) reader.o -o reader $(LIBS)
	mv reader $(OUT_DIR)/reader

writer.o: writer.c 
	$(CC) $(CFLAGS) -c writer.c

reader.o: reader.c
	$(CC) $(CFLAGS) -c reader.c

clean:
	rm -f *.o 
	rm -f $(OUT_DIR)/*