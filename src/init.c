#include "defs.h"
#include "map.h"

ctrl_t control;

void init (char *title)
{
    /* Initialise SDL Video */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    if (map_from_file(&control.map, "map.dat") != 0) {
        printf("Error reading map.dat\n");
        exit(1);
    }

    control.win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, SWIDTH, SHEIGHT, 0);

    if (control.win == NULL) {
        printf("Couldn't create %dx%d window: %s\n", SWIDTH, SHEIGHT, 
            SDL_GetError());
        exit(1);
    }

    memset(&control.input, 0, sizeof(input_t));
    control.lastframe = SDL_GetTicks();
    control.pos = control.offset = 0;
    control.rend = SDL_CreateRenderer(control.win, -1, 0);
}

void cleanup (void)
{
    /* Shut down SDL */
    SDL_Quit();
}
