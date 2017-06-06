#include "defs.h"

#define BORDER_SIZE 5

const uint8_t tfill[3] = {204, 102, 0};
const uint8_t tline[3] = {0, 0, 0};

SDL_Rect draw_tile (ctrl_t *ctrl, int pos_x, int pos_y)
{
    SDL_Rect r;

    r.x = pos_x;
    r.y = pos_y;
    r.w = TILE_SIZE;
    r.h = TILE_SIZE;

    /* Draw outer dark square */
    SDL_SetRenderDrawColor(ctrl->rend, tline[0], tline[1], tline[2], 255);
    SDL_RenderFillRect(ctrl->rend, &r);

    r.w -= BORDER_SIZE;
    r.h -= BORDER_SIZE;

    /* Draw smaller, light coloured square inside */
    SDL_SetRenderDrawColor(ctrl->rend, tfill[0], tfill[1], tfill[2], 255);
    SDL_RenderFillRect(ctrl->rend, &r);
    return r;
}
