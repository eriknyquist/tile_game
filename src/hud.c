#include "defs.h"
#include "text.h"
#include "tile.h"

static void draw_block_counter (ctrl_t *ctrl, int x, int y)
{
    char buf[10];

    snprintf(buf, sizeof(buf), "%u/%d", ctrl->blocks, ctrl->max_blocks);
    draw_moveable_tile(ctrl, x, y);
    text_draw(ctrl, buf, x + (TILE_SIZE * 2), y);
}

void draw_hud (ctrl_t *ctrl)
{
    draw_block_counter(ctrl, TILE_SIZE, TILE_SIZE);
}
