#ifndef DEFS_H_
#define DEFS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

/* Physixcs time-step, milliseconds */
#define PHYSICS_DT 20

/* Map array dimensions */
#define MAX_X 512
#define MAX_Y 16

/* BG scenery array dimensions */
#define BG_MAX_X (MAX_X * 2)
#define BG_MAX_Y (MAX_Y * 2)

/* Tile size in pixels */
#define TILE_SIZE (ctrl->screen_height / MAX_Y)

#define MAX_TILE_SIZE (1080 / MAX_Y)

/* Jump velocity boost, pixels */
#define JUMP_ACCEL (TILE_SIZE * 0.8)

/* Gravity acceleration, pixels-per-second */
#define GRAVITY_PPS (TILE_SIZE * 3.0)

/* Maximum gravity velocity, pixels-per-second */
#define MAX_GRAVITY_PPS (GRAVITY_PPS * 10.0)

/* Gravity increase, in pixels, for a single physics step */
#define GRAVITY_PIXELS ((PHYSICS_DT / 1000.0) * GRAVITY_PPS)

/* Maximum gravity velocity, in pixels, for a single physics step */
#define MAX_GRAVITY_PIXELS ((PHYSICS_DT / 1000.0) * MAX_GRAVITY_PPS)

/* Size of player (just a cube for now) */
#define PLAYER_SIZE (TILE_SIZE / 2.0)

/* Height of the screen in map tiles */
#define YTILES_HEIGHT MAX_Y

/* Width of the screen in map tiles */
#define XTILES_WIDTH (ctrl->screen_width / TILE_SIZE)

#define MAX_XTILES_WIDTH (1920 / MAX_TILE_SIZE)

/* BG scenery tile size in pixels */
#define BG_TILE_SIZE (ctrl->screen_height / BG_MAX_Y)

/* Width of the screen in BG scenery tiles */
#define BG_XTILES_WIDTH (ctrl->screen_width / BG_TILE_SIZE)

/* Height of the screen in BG scenery tiles */
#define BG_YTILES_HEIGHT BG_MAX_Y

/* No. of pixels of map movement for a physics step */
#define MAP_PIXELS (TILE_SIZE / 4.0)

/* No. of pixels of BG scenery movement for a physics step */
#define BG_PIXELS (MAP_PIXELS / 2)

#define MAP_FILE_NAME "map"
#define BG_FILE_NAME "background"

#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef struct input {
    uint8_t left;
    uint8_t right;
    uint8_t up;
    uint8_t down;
    uint8_t enter;
} input_t;

typedef struct map {
    uint8_t data[MAX_Y][MAX_X + MAX_XTILES_WIDTH + 1];
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
    SDL_Rect bg_rect;
    SDL_Rect colliders[YTILES_HEIGHT][MAX_XTILES_WIDTH + 1];
    SDL_Window *win;
    SDL_Renderer *rend;
    SDL_Texture *bg_texture;
    unsigned int pos;
    unsigned int bgpos;
    unsigned int lastframe;
    int screen_height;
    int screen_width;
    uint8_t vsync;
    int offset;
    int bgoffset;
} ctrl_t;

typedef struct game {
    SDL_TimerID timer;
    int (*current_scene)(ctrl_t *ctrl, struct game *game);
    uint32_t rflags;
    unsigned int fps;
    unsigned int dt;
} game_t;
#endif
