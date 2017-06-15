#include "defs.h"
#include "map.h"
#include "tile_collisions.h"

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

const uint8_t fill[3] = {0, 0, 0};

void draw_player (ctrl_t *ctrl)
{
    SDL_SetRenderDrawColor(ctrl->rend, fill[0], fill[1], fill[2], 255);
    SDL_RenderFillRect(ctrl->rend, &ctrl->player.rect);
}

void init_player (ctrl_t *ctrl)
{
    draw_player(ctrl);
}

static void add_gravity(ctrl_t *ctrl)
{
    if (ctrl->player.yvelocity < MAX_GRAVITY_PIXELS) {
        ctrl->player.yvelocity += GRAVITY_PIXELS;
    }
}

/* Returns the distance (in pixels) to the nearest map tile to the
 * left of the player's position on the screen */
int player_distance_left (ctrl_t *ctrl)
{
    return min_positive(
        tile_obstacle_left(ctrl, ctrl->player.rect.x, ctrl->player.rect.y),
        tile_obstacle_left(ctrl, ctrl->player.rect.x,
            ctrl->player.rect.y + PLAYER_SIZE)
    );
}

/* Returns the distance (in pixels) to the nearest map tile to the
 * right of the player's position on the screen */
int player_distance_right (ctrl_t *ctrl)
{
    return min_positive(
        tile_obstacle_right(ctrl, ctrl->player.rect.x + PLAYER_SIZE,
            ctrl->player.rect.y),
        tile_obstacle_right(ctrl, ctrl->player.rect.x + PLAYER_SIZE,
            ctrl->player.rect.y + PLAYER_SIZE)
    );
}

/* Returns the distance (in pixels) to the nearest map tile above
 * the player's position on the screen */
int player_distance_up (ctrl_t *ctrl)
{
    return min_positive(
        tile_obstacle_up(ctrl, ctrl->player.rect.x, ctrl->player.rect.y),
        tile_obstacle_up(ctrl, ctrl->player.rect.x + PLAYER_SIZE,
            ctrl->player.rect.y)
    );
}

/* Returns the distance (in pixels) of the nearest map tile below
 * the player's position on the screen */
int player_distance_down (ctrl_t *ctrl)
{
    return min_positive(
        tile_obstacle_down(ctrl, ctrl->player.rect.x,
            ctrl->player.rect.y + PLAYER_SIZE),
        tile_obstacle_down(ctrl, ctrl->player.rect.x + PLAYER_SIZE,
            ctrl->player.rect.y + PLAYER_SIZE)
    );
}

static void collisions_top (ctrl_t *ctrl)
{
    int udist;

    udist = player_distance_up(ctrl);
    if (udist >= 0 && -(ctrl->player.yvelocity) > udist) {
        ctrl->player.yvelocity = -(udist - 1);
    }
}

static void collisions_bottom (ctrl_t *ctrl)
{
    int ddist;

    ddist = player_distance_down(ctrl);
    if (ddist > 1) {
        add_gravity(ctrl);

        if (ctrl->player.yvelocity > (ddist - 1)) {
            /* Ensure we don't move the player inside, or past the tile */
            ctrl->player.yvelocity = ddist - 1;
            ctrl->player.grounded = 1;
        } else {
            ctrl->player.grounded = 0;
        }
    } else if (ddist == 1 && ctrl->player.yvelocity > 0) {
        ctrl->player.yvelocity = 0;
        ctrl->player.grounded = 1;
    } else if (ddist == 0) {
        ctrl->player.rect.y -= 1;
        ctrl->player.yvelocity = 0;
        ctrl->player.grounded = 1;
    } else if (ddist == BELOW_MAP) {
        reset_map(ctrl);
    }
}

void calculate_ymovement (ctrl_t *ctrl)
{
    if (ctrl->input.up && ctrl->player.grounded &&
            ctrl->player.yvelocity == 0) {
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
