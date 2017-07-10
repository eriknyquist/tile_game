#include "defs.h"
#include "text.h"
#include "tile.h"

static int draw_block_counter (ctrl_t *ctrl, int x, int y)
{
    char buf[10];

    snprintf(buf, sizeof(buf), "%u/%d", ctrl->blocks, ctrl->max_blocks);
    draw_moveable_tile(ctrl, x, y);
    text_draw(ctrl, buf, x + (TILE_SIZE * 2), y);

    return (TILE_SIZE * 2) + (text_texture_width(buf));
}

static int draw_life_counter (ctrl_t *ctrl, int x, int y)
{
    SDL_Rect rect;
    unsigned int i;
    int icon_size;

    icon_size = TILE_SIZE;
    rect.x = x;
    rect.y = y;
    rect.w = icon_size;
    rect.h = icon_size;

    for (i = 0; i < ctrl->lives; ++i) {
        SDL_RenderCopy(ctrl->rend, ctrl->images.hud_life, NULL, &rect);
        rect.x += icon_size;
    }

    return icon_size * ctrl->lives;
}

void draw_hud (ctrl_t *ctrl)
{
    int x, y;

    x = y = TILE_SIZE;

    x += draw_block_counter(ctrl, x, y);
    x += (TILE_SIZE * 2);
    x += draw_life_counter(ctrl, x, y);
}
