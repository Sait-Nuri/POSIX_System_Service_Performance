SOURCE = main_task.c idle_task.c
CFLAGS = -Wall 
LIBS = 
OBJS = main_task.o idle_task.o
CC = gcc
OUT_DIR := ../../build/task_switch_test
EXE = main_task idle_task

all: file $(EXE) 

file:
	mkdir -p $(OUT_DIR)

main_task: main_task.o 
	$(CC) main_task.o -o main_task $(LIBS)
	mv main_task $(OUT_DIR)/main_task

idle_task: idle_task.o 
	$(CC) idle_task.o -o idle_task $(LIBS)
	mv idle_task $(OUT_DIR)/idle_task

main_task.o: main_task.c 
	$(CC) $(CFLAGS) -c main_task.c

idle_task.o: idle_task.c
	$(CC) $(CFLAGS) -c idle_task.c

clean:
	rm -f *.o 
	rm -f $(OUT_DIR)/*