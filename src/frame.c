
#include "defs.h"
#include "map.h"
#include "player.h"

static unsigned int accumulator;

void do_frame(ctrl_t *ctrl)
{
    unsigned int now;

    /* Calculate time between last frame and now */
    now = SDL_GetTicks();
    ctrl->dt = now - ctrl->lastframe;
    ctrl->lastframe = now;

    /* Accumulator tracks how many milliseconds of physics need to be
     * simulated before we can render the next frame */
    accumulator += ctrl->dt;

    /* Advance physics by as many steps as needed
     * to catch up */
    while (accumulator >= PHYSICS_DT) {
        draw_bg_colour(ctrl);
        do_map(ctrl);
        do_player(ctrl);
        accumulator -= PHYSICS_DT;
    }

    SDL_RenderPresent(ctrl->rend);
}
