CFLAGS = -Wall -pedantic -Werror $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs)

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
