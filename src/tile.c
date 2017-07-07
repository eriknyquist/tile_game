#include "defs.h"
#include "colours.h"
#include "tile.h"
#include "utils.h"

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

/* draw_moveable_tile: draw one moveable tile */
SDL_Rect draw_moveable_tile (ctrl_t *ctrl, int pos_x, int pos_y)
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
    SDL_SetRenderDrawColor(ctrl->rend, moveable_fill.r, moveable_fill.g,
        moveable_fill.b, 255);
    SDL_RenderFillRect(ctrl->rend, &r);

    return r;
}

/* draw_tile: draws the correct tile type based on the symbol from the
 * map array, at the index corresponding to screen point (pos_x, pos_y) */
SDL_Rect draw_tile (ctrl_t *ctrl, uint8_t sym, int pos_x, int pos_y)
{
    SDL_Rect ret;
    SDL_Rect rect;

    switch (sym) {
        case FIXED_TILE:
            ret = draw_map_tile(ctrl, pos_x, pos_y);
        break;
        case MOVEABLE_TILE:
            ret = draw_moveable_tile(ctrl, pos_x, pos_y);
        break;
        case PLAYER_FINISH:
            rect.x = pos_x;
            rect.y = pos_y;
            rect.w = rect.h = TILE_SIZE;
            draw_coloured_rect(ctrl, &rect, green);
        break;
    }

    return ret;
}
