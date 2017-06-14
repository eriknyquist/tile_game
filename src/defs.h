#ifndef DEFS_H_
#define DEFS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SDL2/SDL.h"

/* If enabled, the framerate will by determined by the refresh rate
 * If disabled, the framerare specified by FPS will be enforced */
#define VSYNC 0

/* FPS, if fixed */
#define FPS 60
#define MS_PER_FRAME (1000 / FPS)

/* Physixcs time-step, milliseconds */
#define PHYSICS_DT 20

/* Window dimensions */
#define SHEIGHT 1080
#define SWIDTH  1920

/* Map array dimensions */
#define MAX_X 512
#define MAX_Y 16

/* Tile size in pixels */
#define TILE_SIZE (SHEIGHT / MAX_Y)

/* Size of player (just a cube for now) */
#define PLAYER_SIZE (TILE_SIZE / 2)

/* Height of the screen in tiles */
#define YTILES_HEIGHT MAX_Y

/* Width of the screen in tiles*/
#define XTILES_WIDTH (SWIDTH / TILE_SIZE)

typedef struct input {
    uint8_t left;
    uint8_t right;
    uint8_t up;
} input_t;

typedef struct map {
    uint8_t data[MAX_Y][MAX_X];
    int max_x;
    int start_x;
    int start_y;
    uint8_t canmoveright;
    uint8_t canmoveleft;
} map_t;

typedef struct player {
    SDL_Rect rect;
    int yvelocity;
    uint8_t grounded;
} player_t;

typedef struct control {
    map_t map;
    player_t player;
    input_t input;
    SDL_Rect colliders[YTILES_HEIGHT][XTILES_WIDTH + 1];
    SDL_Window *win;
    SDL_Renderer *rend;
    unsigned int dt;
    unsigned int pos;
    unsigned int lastframe;
    int offset;
} ctrl_t;

#endif
