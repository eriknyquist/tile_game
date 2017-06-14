#include "defs.h"
#include "map.h"
#include "player.h"
#include "input.h"

static unsigned int accumulator;

static void set_key_state (ctrl_t *ctrl, int key, unsigned int val)
{
    switch (key) {
        case SDLK_ESCAPE:
            exit(0);
        break;
        case SDLK_LEFT:
            ctrl->input.left = val;
        break;
        case SDLK_RIGHT:
            ctrl->input.right = val;
        break;
        case SDLK_UP:
            ctrl->input.up = val;
        break;
    }
}

void do_frame(ctrl_t *ctrl)
{
    unsigned int now;

    now = SDL_GetTicks();
    ctrl->dt = now - ctrl->lastframe;
    ctrl->lastframe = now;
    accumulator += ctrl->dt;

    while (accumulator >= PHYSICS_DT) {
        do_map(ctrl);
        do_player(ctrl);
        accumulator -= PHYSICS_DT;
    }

    SDL_RenderPresent(ctrl->rend);
}

void process_event (SDL_Event *event, ctrl_t *ctrl)
{
    switch (event->type) {
        case SDL_USEREVENT:
            /* Frame timer expired; draw the next frame */
            do_frame(ctrl);
        break;
        case SDL_KEYDOWN:
            /* Key down event; store it in the input struct */
            set_key_state(ctrl, event->key.keysym.sym, 1);
        break;
        case SDL_KEYUP:
            /* Key up event; store it in the input struct */
            set_key_state(ctrl, event->key.keysym.sym, 0);
        break;
        case SDL_QUIT:
            exit(0);
        break;
    }
}
