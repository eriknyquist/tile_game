#include "defs.h"
#include "utils.h"
#include "tile.h"
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

static void do_tile_changes (ctrl_t *ctrl)
{
    int tile_y;

    if (ctrl->input.enter && ctrl->player.grounded &&
            ctrl->blocks < MAX_BLOCKS) {
        tile_y = ctrl->player.rect.y + ctrl->player.rect.h + PLAYER_SIZE;

        if (tile_y < ctrl->screen_height) {
            if (set_tile_by_screen(ctrl, MOVEABLE_TILE,
                    ctrl->player.rect.x + (PLAYER_SIZE / 2), tile_y, NO_TILE))
                ++ctrl->blocks;
        }

        ctrl->input.enter = 0;
    }

    if (ctrl->input.space && !ctrl->player.grounded && ctrl->blocks > 0) {
        tile_y = ctrl->player.rect.y + ctrl->player.rect.h + (TILE_SIZE * 2);

        if (tile_y < ctrl->screen_height) {
            if (set_tile_by_screen(ctrl, NO_TILE,
                    ctrl->player.rect.x + (PLAYER_SIZE / 2), tile_y,
                    MOVEABLE_TILE))
                --ctrl->blocks;
        }

        ctrl->input.space = 0;
    }
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
    do_tile_changes(ctrl);
    calculate_ymovement(ctrl);
    ctrl->player.rect.y += ctrl->player.yvelocity;
}
