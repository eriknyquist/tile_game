# Native compilation flags
CFLAGS = -Wall -pedantic $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs) -lSDL2_ttf -lSDL2_image -lm

# Win32 cross compilation flags
WIN_CC = i686-w64-mingw32-gcc
SDL_WIN_DIR = /usr/local/cross-tools/i686-w64-mingw32/bin
WIN_CFLAGS = -Wall -pedantic $(shell $(SDL_WIN_DIR)/sdl2-config --cflags)
WIN_LDFLAGS = $(shell $(SDL_WIN_DIR)/sdl2-config --libs) -lSDL2_ttf -lSDL2_image -lm

SRCS=$(wildcard src/*.c)
OBJS=$(SRCS:.c=.o)
PROG = game

all: $(PROG)

win32: CC = $(WIN_CC)
win32: CFLAGS = $(WIN_CFLAGS)
win32: LDFLAGS = $(WIN_LDFLAGS)
win32: PROG := $(PROG).exe
win32: $(PROG)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(PROG): $(OBJS)
	$(CC) $(OBJS) -o $(PROG) $(LDFLAGS)

clean:
	rm -f $(PROG) $(PROG).exe src/*.o
