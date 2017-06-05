CFLAGS = -Wall -pedantic -Werror $(shell sdl-config --cflags)
LDFLAGS = $(shell sdl-config --libs)

SRCS=$(wildcard src/*.c)
OBJS=$(SRCS:.c=.o)
PROG = game

all: $(PROG)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(PROG): $(OBJS)
	$(CC) $(OBJS) -o $(PROG) $(LDFLAGS)

clean:
	rm $(PROG) src/*.o
