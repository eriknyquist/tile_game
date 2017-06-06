#include "defs.h"
#include "map.h"

static void set_key_state (input_t *input, int key, unsigned int val)
{
    switch (key) {
        case SDLK_ESCAPE:
            exit(0);
        break;
        case SDLK_LEFT:
            input->left = val;
        break;
        case SDLK_RIGHT:
            input->right = val;
        break;
    }
}

void process_event (SDL_Event *event)
{
    switch (event->type) {
        case SDL_USEREVENT:
            /* Frame timer expired; draw the next frame */
            do_map(&control);
            SDL_RenderPresent(control.rend);
            control.lastframe = SDL_GetTicks();
        break;
        case SDL_KEYDOWN:
            /* Key down event; store it in the input struct */
            set_key_state(&control.input, event->key.keysym.sym, 1);
        break;
        case SDL_KEYUP:
            /* Key up event; store it in the input struct */
            set_key_state(&control.input, event->key.keysym.sym, 0);
        break;
        case SDL_QUIT:
            exit(0);
        break;
    }
}
