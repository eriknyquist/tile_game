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
#define MAX_Y 16

/* Tile size in pixels */
#define TILE_SIZE (SHEIGHT / MAX_Y)

/* Height of the screen in tiles */
#define YTILES_HEIGHT MAX_Y

/* Width of the screen in tiles*/
#define XTILES_WIDTH (SWIDTH / TILE_SIZE)

typedef struct input {
    uint8_t left;
    uint8_t right;
} input_t;

typedef struct map {
    uint8_t data[MAX_Y][MAX_X];
    int max_x;
} map_t;

typedef struct control {
    map_t map;
    input_t input;
    SDL_Rect colliders[YTILES_HEIGHT * XTILES_WIDTH];
    SDL_Window *win;
    SDL_Renderer *rend;
    unsigned int cpos;
    unsigned int lastframe;
    unsigned int pos;
    int offset;
} ctrl_t;

extern ctrl_t control;

#endif
