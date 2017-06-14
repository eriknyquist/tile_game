#include "defs.h"
#include "tile_collisions.h"

/* Returns the distance (in pixels) to the nearest map tile to the
 * right of the rect's x position on the screen */
int tile_obstacle_right (ctrl_t *ctrl, SDL_Rect *rect)
{
    int i;
    unsigned int x, y, right;

    /* x/y of tile containing the rect's right edge */
    right = rect->x + PLAYER_SIZE;
    x = right / TILE_SIZE;
    y = rect->y / TILE_SIZE;

    /* Loop through tiles to the right of rect, find the first obstacle */
    for (i = x + 1; i < XTILES_WIDTH; ++i) {
        if (ctrl->map.data[y][ctrl->pos + i]) {
            break;
        }
    }

    /* No tiles to the right of rect */
    if (i == XTILES_WIDTH) {
        return NO_TILES;
    }

    /* Return distance between rect and tile */
    return ctrl->colliders[y][i].x - right;
}

/* Returns the distance (in pixels) to the nearest map tile above
 * the rect's x position on the screen */
int tile_obstacle_up (ctrl_t *ctrl, SDL_Rect *rect)
{
    int i;
    unsigned int x, y;

    /* x/y of tile containing rect's top edge */
    x = (rect->x / TILE_SIZE) - 1;
    y = rect->y / TILE_SIZE;

    /* Loop through tiles above rect, find the first obstacle */
    for (i = y - 1; i >= 0; --i) {
        if (ctrl->map.data[i][x + ctrl->pos]) {
            break;
        }
    }

    /* No tiles above rect */
    if (i < 0) {
        return NO_TILES;
    }

    /* Return distance between rect and tile */
    return rect->y - (ctrl->colliders[i][x].y + TILE_SIZE);
}

/* Returns the distance (in pixels) of the nearest map tile below
 * the rect's x position on the screen */
int tile_obstacle_down (ctrl_t *ctrl, SDL_Rect *rect)
{
    int i;
    unsigned int x, y, bottom;

    /* x/y of tile containing the rect's bottom edge */
    bottom = rect->y + PLAYER_SIZE;
    x = (rect->x / TILE_SIZE) - 1;
    y = bottom / TILE_SIZE;

    /* No hope; rect has fallen below the map */
    if (y >= YTILES_HEIGHT) {
        return BELOW_MAP;
    }

    /* Loop through tiles below rect, find the first obstacle */
    for (i = y + 1; i < YTILES_HEIGHT; ++i) {
        if (ctrl->map.data[i][x + ctrl->pos]) {
            break;
        }
    }

    /* No tiles below rect */
    if (i == YTILES_HEIGHT) {
        return NO_TILES;
    }

    /* Return distance between rect and tile */
    return ctrl->colliders[i][x].y - bottom;
}

