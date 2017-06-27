#include "defs.h"
#include "tile_collisions.h"

#define SCREEN_TO_XTILE(ctrl, x) ((x - ctrl->offset) / TILE_SIZE)
#define SCREEN_TO_YTILE(y) (y / TILE_SIZE)

/* tile_obstacle_left: returns the distance (in pixels) to the nearest map tile
 * to the left of a point on the screen */
int tile_obstacle_left (ctrl_t *ctrl, int x, int y)
{
    int i;
    unsigned int xM, yM;

    /* x/y of tile containing the rect's right edge */
    xM = SCREEN_TO_XTILE(ctrl, x);
    yM = SCREEN_TO_YTILE(y);

    /* Loop through tiles to the left of point, find the first obstacle */
    for (i = xM - 1; i >= 0; --i) {
        if (ctrl->map.data[yM][ctrl->pos + i]) {
            break;
        }
    }

    /* No tiles to the right of rect */
    if (i < 0) {
        return NO_TILES;
    }

    /* Return distance between rect and tile */
    return x - (ctrl->colliders[yM][i].x + TILE_SIZE);
}

/* tile_obstacle_right: returns the distance (in pixels) to the nearest map
 * tile to the right of a point on the screen */
int tile_obstacle_right (ctrl_t *ctrl, int x, int y)
{
    int i;
    unsigned int xM, yM;

    /* x/y of tile containing the rect's right edge */
    xM = SCREEN_TO_XTILE(ctrl, x);
    yM = SCREEN_TO_YTILE(y);

    /* Loop through tiles to the right of rect, find the first obstacle */
    for (i = xM + 1; i < XTILES_WIDTH; ++i) {
        if (ctrl->map.data[yM][ctrl->pos + i]) {
            break;
        }
    }

    /* No tiles to the right of rect */
    if (i == XTILES_WIDTH) {
        return NO_TILES;
    }

    /* Return distance between rect and tile */
    return ctrl->colliders[yM][i].x - x;
}

/* tile_obstacle_up: returns the distance (in pixels) to the nearest map tile
 * above a point on the screen */
int tile_obstacle_up (ctrl_t *ctrl, int x, int y)
{
    int i;
    unsigned int xM, yM;

    /* x/y of tile containing rect's top edge */
    xM = SCREEN_TO_XTILE(ctrl, x);
    yM = SCREEN_TO_YTILE(y);

    /* Loop through tiles above rect, find the first obstacle */
    for (i = yM - 1; i >= 0; --i) {
        if (ctrl->map.data[i][xM + ctrl->pos]) {
            break;
        }
    }

    /* No tiles above rect */
    if (i < 0) {
        return y - ctrl->colliders[0][xM].y;
    }

    /* Return distance between rect and tile */
    return y - (ctrl->colliders[i][xM].y + TILE_SIZE);
}

/* tile_obstacle_down: returns the distance (in pixels) of the nearest map
 * tile below a point on the screen */
int tile_obstacle_down (ctrl_t *ctrl, int x, int y)
{
    int i;
    unsigned int xM, yM;

    /* x/y of tile containing the rect's bottom edge */
    xM = SCREEN_TO_XTILE(ctrl, x);
    yM = SCREEN_TO_YTILE(y);

    /* No hope; rect has fallen below the map */
    if (yM >= YTILES_HEIGHT) {
        return BELOW_MAP;
    }

    /* Loop through tiles below rect, find the first obstacle */
    for (i = yM + 1; i < YTILES_HEIGHT; ++i) {
        if (ctrl->map.data[i][xM + ctrl->pos]) {
            break;
        }
    }

    /* No tiles below rect */
    if (i == YTILES_HEIGHT) {
        return NO_TILES;
    }

    /* Return distance between rect and tile */
    return ctrl->colliders[i][xM].y - y;
}
