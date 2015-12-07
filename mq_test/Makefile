SOURCE = sender.c receiver.c
EXE = sender receiver
CFLAGS = -Wall 
LIBS = -lrt
OBJS = sender.o receiver.o
CC = gcc

all: $(EXE)

sender: sender.o
	$(CC) sender.o -o sender $(LIBS)

receiver: receiver.o
	$(CC) receiver.o -o receiver $(LIBS)

sender.o: sender.c
	$(CC) $(CFLAGS) -c sender.c

receiver.o: receiver.c
	$(CC) $(CFLAGS) -c receiver.c

clean:
	rm -f *.o $(EXE)
