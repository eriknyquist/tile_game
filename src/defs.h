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
#define MAX_Y 24

/* Tile size in pixels */
#define TILE_SIZE (SHEIGHT / MAX_Y)

typedef struct map {
    uint8_t data[MAX_Y][MAX_X];
    int max_x;
} map_t;

typedef struct control {
    map_t map;
    SDL_Window *win;
    SDL_Renderer *rend;
    int offset;
} ctrl_t;

extern ctrl_t control;

#endif
