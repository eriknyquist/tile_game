#include "defs.h"
#include "tile_physics.h"

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

void calculate_ymovement (ctrl_t *ctrl)
{
    if (ctrl->input.up && ctrl->player.grounded &&
            ctrl->player.yvelocity == 0) {
        ctrl->player.yvelocity -= JUMP_ACCEL;
        ctrl->input.up = 0;
    }

    tile_collisions_top(ctrl, &ctrl->player);
    tile_collisions_bottom(ctrl, &ctrl->player);

}

void do_player (ctrl_t *ctrl)
{
    calculate_ymovement(ctrl);
    ctrl->player.rect.y += ctrl->player.yvelocity;
    draw_player(ctrl);
}
