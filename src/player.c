#include "defs.h"
#include "tile_physics.h"

const uint8_t fill[3] = {0, 0, 0};

/* calculate_ymovement: check & correct for collisions between the player and
 * the tile map on the Y-axis, and add acceleration for a jump (NOTE: collision
 * detection/correction for the X-axis is handled by do_map() */
static void calculate_ymovement (ctrl_t *ctrl)
{
    if (ctrl->input.up && ctrl->player.grounded &&
            ctrl->player.yvelocity == 0) {
        ctrl->player.yvelocity -= JUMP_ACCEL;
        ctrl->input.up = 0;
    }

    tile_collisions_top(ctrl, &ctrl->player);
    tile_collisions_bottom(ctrl, &ctrl->player);

}

/* draw_player: draws the player on the screen. Just a boring rectangle right
 * now... */
void draw_player (ctrl_t *ctrl)
{
    SDL_SetRenderDrawColor(ctrl->rend, fill[0], fill[1], fill[2], 255);
    SDL_RenderFillRect(ctrl->rend, &ctrl->player.rect);
}

/* init_player: for now we just call 'draw_player'.
 * More stuff will go here eventually */
void init_player (ctrl_t *ctrl)
{
    draw_player(ctrl);
}

/* do_player: perform a full step of physics calculations for the player
 * and draw the player at the new position */
void do_player (ctrl_t *ctrl)
{
    calculate_ymovement(ctrl);
    ctrl->player.rect.y += ctrl->player.yvelocity;
    draw_player(ctrl);
}
