#include "defs.h"
#include "input.h"
#include "frame.h"

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

void process_event (SDL_Event *event, ctrl_t *ctrl, game_t *game)
{
    switch (event->type) {
        case SDL_USEREVENT:
            /* Frame timer expired; draw the next frame */
            do_frame(ctrl, game);
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
