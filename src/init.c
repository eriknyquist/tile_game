#include "defs.h"
#include "map.h"

#define DEFAULT_MAP "maps/1.dat"

/* #define RENDER_FLAGS SDL_RENDERER_PRESENTVSYNC */
#define RENDER_FLAGS 0

ctrl_t control;

void init (char *title)
{
    memset(&control, 0, sizeof(ctrl_t));

    /* Initialise SDL Video */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    if (map_from_file(&control.map, DEFAULT_MAP) != 0) {
        printf("Error reading "DEFAULT_MAP"\n");
        exit(1);
    }

    control.win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, SWIDTH, SHEIGHT, 0);

    if (control.win == NULL) {
        printf("Couldn't create %dx%d window: %s\n", SWIDTH, SHEIGHT, 
            SDL_GetError());
        exit(1);
    }

    control.lastframe = SDL_GetTicks();
    control.pos = control.offset = 0;
    control.rend = SDL_CreateRenderer(control.win, -1, RENDER_FLAGS);
}

void cleanup (void)
{
    /* Shut down SDL */
    SDL_Quit();
}
