#ifndef DEFS_H_
#define DEFS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SDL2/SDL.h"

/* Window dimensions */
#define SHEIGHT 1080
#define SWIDTH  1920

/* Map array dimensions */
#define MAX_X 512
#define MAX_Y 32

/* Tile size in pixels */
#define TILE_SIZE (SHEIGHT / MAX_Y)

typedef struct control {
    uint8_t map[MAX_Y][MAX_X];
    SDL_Window *win;
    SDL_Renderer *rend;
} ctrl_t;

extern ctrl_t ctrl;

#endif
