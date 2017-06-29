#include "defs.h"
#include "player.h"
#include "map.h"

/* Draws one full frame of the game scene, advancing the physics
 * engine as many steps as needed to catch up with the time elapsed since
 * the last frame */
int draw_scene_game (ctrl_t *ctrl, game_t *game)
{
    static unsigned int accumulator;

    /* Accumulator tracks how many milliseconds of physics need to be
     * simulated before we can render the next frame */
    accumulator += game->dt;

    /* Advance physics by as many steps as needed
     * to catch up */
    while (accumulator >= PHYSICS_DT) {
        do_map(ctrl);
        draw_map(ctrl);

        do_player(ctrl);
        draw_player(ctrl);
        accumulator -= PHYSICS_DT;
    }

    return 0;
}

/* Draws the current state of the game without advancing the physics engine */
int draw_scene_game_paused (ctrl_t *ctrl, game_t *game)
{
    draw_map(ctrl);
    draw_player(ctrl);

    return 0;
}
