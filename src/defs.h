#ifndef DEFS_H_
#define DEFS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "SDL2/SDL.h"
#include "utils.h"

/* If enabled, the framerate will by determined by the refresh rate
 * If disabled, the framerare specified by FPS will be enforced */
#define VSYNC 1

/* FPS, if fixed */
#define FPS 60
#define MS_PER_FRAME (1000 / FPS)

/* Physixcs time-step, milliseconds */
#define PHYSICS_DT 20

/* Gravity acceleration, pixels-per-second */
#define GRAVITY_PPS 75

/* Maximum gravity velocity, pixels-per-second */
#define MAX_GRAVITY_PPS 1500

/* Gravity increase, in pixels, for a single physics step */
#define GRAVITY_PIXELS ((PHYSICS_DT / 1000.0) * GRAVITY_PPS)

/* Maximum gravity velocity, in pixels, for a single physics step */
#define MAX_GRAVITY_PIXELS ((PHYSICS_DT / 1000.0) * MAX_GRAVITY_PPS)

/* Jump velocity boost, pixels */
#define JUMP_ACCEL 30

/* Window dimensions */
#define SHEIGHT 1080
#define SWIDTH  1920

/* Map array dimensions */
#define MAX_X 512
#define MAX_Y 16

/* BG scenery array dimensions */
#define BG_MAX_X (MAX_X * 2)
#define BG_MAX_Y (MAX_Y * 2)

/* Tile size in pixels */
#define TILE_SIZE (SHEIGHT / MAX_Y)

/* Size of player (just a cube for now) */
#define PLAYER_SIZE (TILE_SIZE / 2)

/* Height of the screen in map tiles */
#define YTILES_HEIGHT MAX_Y

/* Width of the screen in map tiles */
#define XTILES_WIDTH (SWIDTH / TILE_SIZE)

/* BG scenery tile size in pixels */
#define BG_TILE_SIZE (SHEIGHT / BG_MAX_Y)

/* Width of the screen in BG scenery tiles */
#define BG_XTILES_WIDTH (SWIDTH / BG_TILE_SIZE)

/* Height of the screen in BG scenery tiles */
#define BG_YTILES_HEIGHT BG_MAX_Y

/* X movement speed- pixels per sec. */
#define X_PPS 400

/* No. of pixels of map movement for a physics step */
#define MAP_PIXELS ((PHYSICS_DT / 1000.0) * X_PPS)

/* Background scenery movement speed- pixels per sec. */
#define BG_PPS 200

/* No. of pixels of BG scenery movement for a physics step */
#define BG_PIXELS ((PHYSICS_DT / 1000.0) * BG_PPS)

#define MAP_FILE_NAME "map"
#define BG_FILE_NAME "background"

#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef struct input {
    uint8_t left;
    uint8_t right;
    uint8_t up;
} input_t;

typedef struct map {
    uint8_t data[MAX_Y][MAX_X + XTILES_WIDTH + 1];
    uint8_t bg[BG_MAX_Y][BG_MAX_X];
    int max_x;
    int max_p;
    int bg_max_x;
    int start_x;
    int start_y;
} map_t;

typedef struct moveable {
    SDL_Rect rect;
    int yvelocity;
    int xvelocity;
    uint8_t grounded;
} moveable_t;

typedef struct control {
    map_t map;
    moveable_t player;
    input_t input;
    SDL_Rect colliders[YTILES_HEIGHT][XTILES_WIDTH + 1];
    SDL_Window *win;
    SDL_Renderer *rend;
    unsigned int pos;
    unsigned int bgpos;
    unsigned int lastframe;
    int offset;
    int bgoffset;
} ctrl_t;

typedef struct game {
    unsigned int dt;
    void (*current_scene)(ctrl_t *ctrl, struct game *game);
} game_t;
#endif
