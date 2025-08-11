CC=gcc
CFLAGS=-Wall -Wextra -pthread
SRCDIR=.
OBJS=main.o peer.o chatapp.o commands.o utils.o

all: p2pchat

p2pchat: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) p2pchat
