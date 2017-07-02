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
void draw_bg_colour (ctrl_t *ctrl, const SDL_Color colour)
{
    SDL_SetRenderDrawColor(ctrl->rend, colour.r, colour.g, colour.b, 255);
    SDL_RenderClear(ctrl->rend);
}

/* set_bg_color: draws the background colour on the entire window */
void draw_bg_bmp (ctrl_t *ctrl)
{
    int i, j;

    SDL_RenderClear(ctrl->rend);

    for (i = 0; i <= (ctrl->screen_height / ctrl->bg_rect.h); ++i) {
        for (j = 0; j <= (ctrl->screen_width / ctrl->bg_rect.w); ++j) {
            ctrl->bg_rect.y = i * ctrl->bg_rect.h;
            ctrl->bg_rect.x = j * ctrl->bg_rect.w;
            SDL_RenderCopy(ctrl->rend, ctrl->bg_texture, NULL, &ctrl->bg_rect);
        }
    }
}

int set_tile_by_screen (ctrl_t *ctrl, int x, int y, uint8_t value)
{
    uint8_t old;
    int xM, yM;

    xM = SCREEN_TO_XTILE(ctrl, x);
    yM = SCREEN_TO_YTILE(y);

    old = ctrl->map.data[yM][xM + ctrl->pos];
    ctrl->map.data[yM][xM + ctrl->pos] = value;

    return old;
}

int trajectory_range (ctrl_t *ctrl, int xvelocity, int yvelocity)
{
    float time;

    time = (yvelocity / (float)GRAVITY_PIXELS) * 2.0;
    return (int)(time * xvelocity);
}
