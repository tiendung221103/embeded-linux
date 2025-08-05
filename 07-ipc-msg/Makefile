CC = gcc
CFLAGS = -Wall -lrt
TARGETS = task_worker task_dispatcher

all: $(TARGETS)

task_worker: task_worker.c
	$(CC) $(CFLAGS) -o task_worker task_worker.c

task_dispatcher: task_dispatcher.c
	$(CC) $(CFLAGS) -o task_dispatcher task_dispatcher.c

clean:
	rm -f $(TARGETS)
