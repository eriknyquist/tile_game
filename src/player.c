#include "defs.h"
#include "map.h"

/* Gravity acceleration, pixels-per-second */
#define GRAVITY_PPS 100

/* Gravity increase, in pixels, for a single physics step */
#define GRAVITY_PIXELS ((PHYSICS_DT / 1000.0) * GRAVITY_PPS)

/* Jump velocity boost, pixels */
#define JUMP_ACCEL 30

/* Maximum gravity velocity, pixels-per-second */
#define MAX_YVELOCITY ((PHYSICS_DT / 1000.0) * 1000)

#define BELOW_MAP -1
#define NO_TILES  -2

const uint8_t fill[3] = {0, 0, 0};

static void draw_player (ctrl_t *ctrl)
{
    SDL_SetRenderDrawColor(ctrl->rend, fill[0], fill[1], fill[2], 255);
    SDL_RenderFillRect(ctrl->rend, &ctrl->player.rect);
}

/* Returns the distance (in pixels) to the nearest map tile to the
 * right of the player's x position on the screen */
static int obstacle_right (ctrl_t *ctrl)
{
    int i;
    unsigned int x, y, right;

    /* x/y of tile containing the player's right edge */
    right = ctrl->player.rect.x + PLAYER_SIZE;
    x = right / TILE_SIZE;
    y = ctrl->player.rect.y / TILE_SIZE;

    /* Loop through tiles to the right of player, find the first obstacle */
    for (i = x + 1; i < XTILES_WIDTH; ++i) {
        if (ctrl->map.data[y][ctrl->pos + i]) {
            break;
        }
    }

    /* No tiles to the right of player */
    if (i == XTILES_WIDTH) {
        return NO_TILES;
    }

    /* Return distance between player and tile */
    return ctrl->colliders[y][i].x - right;
}

/* Returns the distance (in pixels) to the nearest map tile above
 * the player's x position on the screen */
static int obstacle_up (ctrl_t *ctrl)
{
    int i;
    unsigned int x, y;

    /* x/y of tile containing the player's top edge */
    x = (ctrl->player.rect.x / TILE_SIZE) - 1;
    y = ctrl->player.rect.y / TILE_SIZE;

    /* Loop through tiles above player, find the first obstacle */
    for (i = y - 1; i >= 0; --i) {
        if (ctrl->map.data[i][x + ctrl->pos]) {
            break;
        }
    }

    /* No tiles above player */
    if (i < 0) {
        return NO_TILES;
    }

    /* Return distance between player and tile */
    return ctrl->player.rect.y - (ctrl->colliders[i][x].y + TILE_SIZE);
}

/* Returns the distance (in pixels) of the nearest map tile below
 * the player's x position on the screen */
static int obstacle_down (ctrl_t *ctrl)
{
    int i;
    unsigned int x, y, bottom;

    /* x/y of tile containing the player's bottom edge */
    bottom = ctrl->player.rect.y + PLAYER_SIZE;
    x = (ctrl->player.rect.x / TILE_SIZE) - 1;
    y = bottom / TILE_SIZE;

    /* No hope; player has fallen below the map */
    if (y >= YTILES_HEIGHT) {
        return BELOW_MAP;
    }

    /* Loop through tiles below player, find the first obstacle */
    for (i = y + 1; i < YTILES_HEIGHT; ++i) {
        if (ctrl->map.data[i][x + ctrl->pos]) {
            break;
        }
    }

    /* No tiles below player */
    if (i == YTILES_HEIGHT) {
        return NO_TILES;
    }

    /* Return distance between player and tile */
    return ctrl->colliders[i][x].y - bottom;
}

void init_player (ctrl_t *ctrl)
{
    draw_player(ctrl);
}

static void add_gravity(ctrl_t *ctrl)
{
    if (ctrl->player.yvelocity < MAX_YVELOCITY) {
        ctrl->player.yvelocity += GRAVITY_PIXELS;
    }
}

static void collisions_top (ctrl_t *ctrl)
{
    int udist;

    udist = obstacle_up(ctrl);

    if (udist >= 1) {
        if (-(ctrl->player.yvelocity) > udist) {
            ctrl->player.yvelocity = -(udist - 1);
        }
    } else if (udist == 0) {
        ctrl->player.yvelocity = 0;
    }
}

static void collisions_bottom (ctrl_t *ctrl)
{
    int ddist;

    /* Get distance to nearest tile below player */
    ddist = obstacle_down(ctrl);

    if (ddist > 1) {
        add_gravity(ctrl);

        if (ctrl->player.yvelocity > ddist) {
            /* Ensure we don't move the player inside, or past the tile */
            ctrl->player.yvelocity = ddist - 1;
            ctrl->player.grounded = 1;
        } else {
            ctrl->player.grounded = 0;
        }
    } else if (ddist == NO_TILES) {
        add_gravity(ctrl);
        ctrl->player.grounded = 0;
    } else if (ddist == BELOW_MAP) {
        reset_map(ctrl);
    } else if (ctrl->player.yvelocity > 0) {
        ctrl->player.yvelocity = 0;
        ctrl->player.grounded = 1;
    }
}

void calculate_ymovement (ctrl_t *ctrl)
{
    if (ctrl->input.up && ctrl->player.grounded) {
        ctrl->player.yvelocity -= JUMP_ACCEL;
        ctrl->input.up = 0;
    }

    collisions_top(ctrl);
    collisions_bottom(ctrl);

}

void do_player (ctrl_t *ctrl)
{
    calculate_ymovement(ctrl);

    ctrl->player.rect.y += ctrl->player.yvelocity;
    draw_player(ctrl);
}
