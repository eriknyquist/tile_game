#ifndef DEFS_H_
#define DEFS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#define MAX_BLOCKS 1

/* Number of seconds to display a level banner */
#define LEVEL_BANNER_SECS 2

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

#define MAX_TILE_SIZE (1440 / MAX_Y)

/* Jump velocity boost, pixels */
#define JUMP_ACCEL (TILE_SIZE * 0.8)

/* Gravity increase, in pixels, for a single physics step */
#define GRAVITY_PIXELS (TILE_SIZE / 15.0)

/* Maximum gravity velocity, in pixels, for a single physics step */
#define MAX_GRAVITY_PIXELS (GRAVITY_PIXELS * 8.0)

/* Size of player (just a cube for now) */
#define PLAYER_SIZE (TILE_SIZE / 2.0)

/* Height of the screen in map tiles */
#define YTILES_HEIGHT MAX_Y

/* Width of the screen in map tiles */
#define XTILES_WIDTH (ctrl->screen_width / TILE_SIZE)

#define MAX_XTILES_WIDTH (2560 / MAX_TILE_SIZE)

/* BG scenery tile size in pixels */
#define BG_TILE_SIZE (ctrl->screen_height / BG_MAX_Y)

/* Width of the screen in BG scenery tiles */
#define BG_XTILES_WIDTH (ctrl->screen_width / BG_TILE_SIZE)

/* Height of the screen in BG scenery tiles */
#define BG_YTILES_HEIGHT BG_MAX_Y

/* No. of pixels of map movement for a physics step */
#define MAP_PIXELS (TILE_SIZE / 6.0)

/* No. of pixels of BG scenery movement for a physics step */
#define BG_PIXELS (MAP_PIXELS / 2)

#define MAP_FILE_NAME "map"
#define BG_FILE_NAME "background"

#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define SCREEN_TO_XTILE(ctrl, x) ((int)(x - ctrl->offset) / TILE_SIZE)
#define SCREEN_TO_YTILE(y) ((int)y / TILE_SIZE)

typedef struct input {
    uint8_t left;
    uint8_t right;
    uint8_t up;
    uint8_t down;
    uint8_t enter;
    uint8_t shift;
    uint8_t space;
} input_t;

typedef struct map {
    uint8_t data[MAX_Y][MAX_X + MAX_XTILES_WIDTH + 1];
    uint8_t bg[BG_MAX_Y][BG_MAX_X];
    int max_x;
    int max_p;
    int bg_max_x;
    int start_x;
    int start_y;
    int finish_x;
    int finish_y;
    float x_accel;
    float bg_x_accel;
} map_t;

typedef struct moveable {
    SDL_Rect rect;
    float yvelocity;
    float xvelocity;
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
    unsigned int blocks;
    unsigned int pos;
    unsigned int bgpos;
    unsigned int lastframe;
    unsigned int level;
    int screen_height;
    int screen_width;
    uint8_t vsync;
    float offset;
    float bgoffset;
} ctrl_t;

typedef struct game {
    SDL_TimerID timer;
    char scene_text[256];
    int (*current_scene)(ctrl_t *ctrl, struct game *game);
    int (*return_scene)(ctrl_t *ctrl, struct game *game);
    uint32_t rflags;
    unsigned int fps;
    unsigned int scene_text_len;
    unsigned int dt;
} game_t;
#endif
