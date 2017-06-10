#include "defs.h"
#include "collision.h"

#define MAX_YVELOCITY 15

const uint8_t fill[3] = {0, 0, 0};

static void draw_player (ctrl_t *ctrl)
{
    SDL_SetRenderDrawColor(ctrl->rend, fill[0], fill[1], fill[2], 255);
    SDL_RenderFillRect(ctrl->rend, &ctrl->player.rect);
}

/* Returns the distance (in pixels) of the nearest map tile below
 * the player's x position on the screen */
static int obstacle_down (ctrl_t *ctrl)
{
    int i;
    unsigned int x, y, bottom;

    /* x/y of tile containing the player's bottom edge */
    bottom = ctrl->player.rect.y + TILE_SIZE;
    x = ctrl->player.rect.x / TILE_SIZE;
    y = bottom / TILE_SIZE;

    /* No hope; player has fallen below the map */
    if (y >= YTILES_HEIGHT) {
        return -1;
    }

    /* Loop through tiles below player, find the first obstacle */
    for (i = y + 1; i < YTILES_HEIGHT; ++i) {
        if (ctrl->map.data[i][x + ctrl->pos]) {
            break;
        }
    }

    /* No tiles below player */
    if (i == YTILES_HEIGHT) {
        return -1;
    }

    /* Return distance between player and tile */
    return ctrl->colliders[i][x].y - bottom;
}

void init_player (ctrl_t *ctrl)
{
    draw_player(ctrl);
}

void add_gravity(ctrl_t *ctrl)
{
    if (ctrl->player.yvelocity < MAX_YVELOCITY)
        ctrl->player.yvelocity += 1;
}

void calculate_ymovement(ctrl_t *ctrl, int dist)
{
    if (dist > 1) {
        if (ctrl->player.yvelocity > dist) {
            ctrl->player.yvelocity = dist - 1;
            ctrl->player.grounded = 1;
        } else {
            add_gravity(ctrl);
            ctrl->player.grounded = 0;
        }
    } else if (dist < 1) {
        add_gravity(ctrl);
    } else {
        ctrl->player.yvelocity = 0;
    }
}

void do_player (ctrl_t *ctrl)
{
    int ddist;

    ddist = obstacle_down(ctrl);
    calculate_ymovement(ctrl, ddist);

    ctrl->player.rect.y += ctrl->player.yvelocity;
    draw_player(ctrl);
}
