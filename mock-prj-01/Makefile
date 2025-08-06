CC = gcc
CFLAGS = -Wall -pthread

SRCS = main.c connection.c command.c
OBJS = $(SRCS:.c=.o)

TARGET = chatp2p

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)
