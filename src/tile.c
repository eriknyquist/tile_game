#include "defs.h"
#include "colours.h"

#define BORDER_SIZE 5

/* draw_bg_tile: draw one background tile */
void draw_bg_tile (ctrl_t *ctrl, int pos_x, int pos_y)
{
    SDL_Rect r;

    r.x = pos_x;
    r.y = pos_y;
    r.w = BG_TILE_SIZE;
    r.h = BG_TILE_SIZE;

    SDL_SetRenderDrawColor(ctrl->rend, bg_fill.r, bg_fill.g, bg_fill.b, 255);
    SDL_RenderFillRect(ctrl->rend, &r);
}

/* draw_map_tile: draw one map tile */
SDL_Rect draw_map_tile (ctrl_t *ctrl, int pos_x, int pos_y)
{
    SDL_Rect r;

    r.x = pos_x;
    r.y = pos_y;
    r.w = TILE_SIZE;
    r.h = TILE_SIZE;

    /* Draw outer dark square */
    SDL_SetRenderDrawColor(ctrl->rend, map_border.r, map_border.g,
        map_border.b, 255);
    SDL_RenderFillRect(ctrl->rend, &r);

    r.w -= BORDER_SIZE;
    r.h -= BORDER_SIZE;

    /* Draw smaller, light coloured square inside */
    SDL_SetRenderDrawColor(ctrl->rend, map_fill.r, map_fill.g,
        map_fill.b, 255);
    SDL_RenderFillRect(ctrl->rend, &r);

    return r;
}
