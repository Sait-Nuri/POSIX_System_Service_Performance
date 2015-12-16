SOURCE = sender.c receiver.c
CFLAGS = -Wall 
LIBS = -lrt
OBJS = sender.o receiver.o
CC = gcc
OUT_DIR := ../../build/mq_test
EXE = sender receiver

all: file $(EXE)

file:
	mkdir -p $(OUT_DIR)

sender: sender.o 
	$(CC) sender.o -o sender $(LIBS)
	mv sender $(OUT_DIR)/sender

receiver: receiver.o 
	$(CC) receiver.o -o receiver $(LIBS)
	mv receiver $(OUT_DIR)/receiver

sender.o: sender.c
	$(CC) $(CFLAGS) -c sender.c

receiver.o: receiver.c
	$(CC) $(CFLAGS) -c receiver.c

clean:
	rm -f *.o
	rm -f $(OUT_DIR)/*
