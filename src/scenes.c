#include "defs.h"
#include "map.h"
#include "player.h"

/* draw_scene_game: draw one full frame of the game scene, advancing the physics
 * engine as many steps as needed to catch up with the time elapsed since
 * the last frame */
void draw_scene_game (ctrl_t *ctrl, game_t *game)
{
    static unsigned int accumulator;

    /* Accumulator tracks how many milliseconds of physics need to be
     * simulated before we can render the next frame */
    accumulator += game->dt;

    /* Advance physics by as many steps as needed
     * to catch up */
    while (accumulator >= PHYSICS_DT) {
        draw_bg_colour(ctrl);
        do_map(ctrl);
        do_player(ctrl);
        accumulator -= PHYSICS_DT;
    }
}

