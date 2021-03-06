#include "defs.h"
#include "player.h"
#include "map.h"
#include "scenes.h"

static uint8_t dying;
static void *params[2];

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

int set_tile_by_screen (ctrl_t *ctrl, uint8_t sym, int x, int y, uint8_t value)
{
    uint8_t old;
    int xM, yM;

    xM = SCREEN_TO_XTILE(ctrl, x);
    yM = SCREEN_TO_YTILE(y);

    old = ctrl->map.data[yM][xM + ctrl->pos];

    if (old == sym) {
        ctrl->map.data[yM][xM + ctrl->pos] = value;
        return 1;
    }

    return 0;
}

int trajectory_range (ctrl_t *ctrl, int xvelocity, int yvelocity)
{
    double time;

    time = (yvelocity / (double)GRAVITY_PIXELS) * 2.0;
    return (int)(time * xvelocity);
}

static uint32_t game_text_timer (uint32_t interval, void *param)
{
    game_t *game;

    game = (game_t *)param;
    game->current_scene = game->return_scene;
    return 0;
}

static uint32_t win_scene_timer (uint32_t interval, void *param)
{
    void **data;
    ctrl_t *ctrl;
    game_t *game;

    data = (void **)param;
    ctrl = (ctrl_t *)*data;
    game = (game_t *)*(data + 1);

    ctrl->level = 0;
    next_level(ctrl, game);
    return 0;
}

void cut_to_text (game_t *game, char *text, unsigned int len, unsigned int secs)
{
    memcpy(game->scene_text, text, len);
    game->scene_text_len = len;
    game->current_scene = draw_scene_text;
    SDL_AddTimer(secs * 1000, game_text_timer, game);
}

static uint32_t death_timer (uint32_t interval, void *param)
{
    void **ptr;
    ctrl_t *ctrl;
    game_t *game;

    ptr = (void **)param;
    ctrl = (ctrl_t *)ptr[0];
    game = (game_t *)ptr[1];

    game->return_scene = draw_scene_game_reset;
    if (ctrl->lives == 1) {
        memcpy(ctrl->map.data, ctrl->map.reset_copy, sizeof(ctrl->map.data));
        ctrl->lives = MAX_LIVES;
        ctrl->blocks = 0;
        cut_to_text(game, "game over", 10, 5);
    } else {
        --ctrl->lives;
        cut_to_text(game, "dead", 5, 1);
    }

    dying = 0;
    return 0;
}

void death (ctrl_t *ctrl, game_t *game)
{
    params[0] = ctrl;
    params[1] = game;

    if (dying) return;

    dying = 1;
    game->current_scene = draw_scene_game_paused;
    SDL_AddTimer(1000, death_timer, params);
}

void winning_scene (ctrl_t *ctrl, game_t *game)
{
    params[0] = ctrl;
    params[1] = game;

    memcpy(game->scene_text, "win", 4);
    game->scene_text_len = 3;
    game->current_scene = draw_scene_text;
    SDL_AddTimer(2000, win_scene_timer, params);
}
