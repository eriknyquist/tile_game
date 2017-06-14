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

static void draw_player (ctrl_t *ctrl)
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

static void collisions_top (ctrl_t *ctrl)
{
    int udist;

    udist = tile_obstacle_up(ctrl, &ctrl->player.rect);

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
    ddist = tile_obstacle_down(ctrl, &ctrl->player.rect);

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
