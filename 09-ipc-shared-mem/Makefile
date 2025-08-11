CC = gcc
CFLAGS = -Wall -g

PRODUCER = producer
CONSUMER = consumer

all: $(PRODUCER) $(CONSUMER)

$(PRODUCER): posix_shm_producer.c
	$(CC) $(CFLAGS) -o $(PRODUCER) posix_shm_producer.c

$(CONSUMER): posix_shm_consumer.c
	$(CC) $(CFLAGS) -o $(CONSUMER) posix_shm_consumer.c

clean:
	rm -f $(PRODUCER) $(CONSUMER)
