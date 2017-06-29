#include "defs.h"
#include "player.h"
#include "map.h"

/* draw_coloured_rect: draw coloured rect outline */
void draw_coloured_rect (ctrl_t *ctrl, SDL_Rect *rect, SDL_Colour c)
{
    SDL_SetRenderDrawColor(ctrl->rend, c.r, c.g, c.b, 255);
    SDL_RenderDrawRect(ctrl->rend, rect);
}

int min_positive (int a, int b)
{
    int r;

    r = INT_MAX;

    if (a > 0) r = MIN(r, a);
    if (b > 0) r = MIN(r, b);

    if (r == INT_MAX) r = MIN(a, b);
    return r;
}

/* set_bg_color: draws the background colour on the entire window */
void draw_bg_colour (ctrl_t *ctrl, const SDL_Color colour){
    SDL_SetRenderDrawColor(ctrl->rend, colour.r, colour.g, colour.b, 255);
    SDL_RenderClear(ctrl->rend);
}

int trajectory_range (ctrl_t *ctrl, int xvelocity, int yvelocity)
{
    float time;

    time = (yvelocity / (float)GRAVITY_PIXELS) * 2.0;
    return (int)(time * xvelocity);
}
