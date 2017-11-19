#include "defs.h"
#include "player.h"
#include "map.h"
#include "text.h"
#include "utils.h"
#include "colours.h"
#include "hud.h"

static void do_game_loop (ctrl_t *ctrl, game_t *game, double blend)
{
    do_map(ctrl, blend);
    draw_map(ctrl);
    do_player(ctrl, game, blend);
}

/* Draws one full frame of the game scene, advancing the physics
 * engine as many steps as needed to catch up with the time elapsed since
 * the last frame */
int draw_scene_game (ctrl_t *ctrl, game_t *game)
{
    double blend;
    static unsigned int accumulator;

    if (ctrl->input.t) {
        game->slomo = !game->slomo;
        game->dt_factor = (game->slomo) ? 0.5 : 1.0;
        ctrl->input.t = 0;
    }

    /* Accumulator tracks how many milliseconds of physics need to be
     * simulated before we can render the next frame */
    accumulator += (unsigned int)(game->dt * game->dt_factor);

    /* Advance physics by as many whole steps as possible */
    while (accumulator >= PHYSICS_DT) {
        do_game_loop(ctrl, game, 1.0);
        accumulator -= PHYSICS_DT;
    }

    /* Calculate remainder and advance physics by partial step */
    if (accumulator > 0) {
        blend = (1.0 / PHYSICS_DT) * (double)accumulator;
        do_game_loop(ctrl, game, blend);
        accumulator -= PHYSICS_DT * blend;
    }

    draw_player(ctrl);
    draw_hud(ctrl);
    return 0;
}

int draw_scene_game_reset (ctrl_t *ctrl, game_t *game)
{
    reset_map(ctrl);
    game->current_scene = draw_scene_game;
    return 0;
}

/* Draws the current state of the game without advancing the physics engine */
int draw_scene_game_paused (ctrl_t *ctrl, game_t *game)
{
    draw_map(ctrl);
    draw_player(ctrl);
    draw_hud(ctrl);

    return 0;
}

int draw_scene_text (ctrl_t *ctrl, game_t *game)
{
    unsigned int height, width, x, y;

    height = text_texture_height(game->scene_text);
    width = text_texture_width(game->scene_text);

    x = (ctrl->screen_width / 2) - (width / 2);
    y = (ctrl->screen_height / 2) - (height / 2);

    draw_bg_colour(ctrl, black);
    text_draw(ctrl, game->scene_text, x, y);

    return 0;
}
