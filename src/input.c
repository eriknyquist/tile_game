#include "defs.h"

void get_input (input_t *input)
{
    SDL_Event event;
    unsigned int val;


    /* Get input type */
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
            break;
            case SDL_KEYDOWN:
                val = 1;
            break;
            case SDL_KEYUP:
                val = 0;
            break;
            default:
                continue;
        }

        /* Get key */
        switch (event.key.keysym.sym) {
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
}
