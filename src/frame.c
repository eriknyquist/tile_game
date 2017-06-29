
#include "defs.h"

/* Keeps track of the frame timer, and draws the current scene */
int do_frame(ctrl_t *ctrl, game_t *game)
{
    unsigned int now;

    /* Calculate time between last frame and now */
    now = SDL_GetTicks();
    game->dt = now - ctrl->lastframe;
    ctrl->lastframe = now;

    /* Draw and render the current scene */
    if (game->current_scene) {
        if(game->current_scene(ctrl, game))
            return 1;

        SDL_RenderPresent(ctrl->rend);
    }

    return 0;
}
