#include "defs.h"
#include "tile.h"

ctrl_t control;

/* Sky blue */
const uint8_t bgcolor[3] = {102, 204, 255};

void set_bg_color(ctrl_t *ctrl)
{
    SDL_SetRenderDrawColor(ctrl->rend, bgcolor[0], bgcolor[1], bgcolor[2], 255);
    SDL_RenderClear(ctrl->rend);
}

void init (char *title)
{
	/* Initialise SDL Video */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Could not initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

    control.win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, SWIDTH, SHEIGHT, 0);

    if (control.win == NULL) {
        printf("Couldn't create %dx%d window: %s\n", SWIDTH, SHEIGHT, 
            SDL_GetError());
		exit(1);
	}

    control.rend = SDL_CreateRenderer(control.win, -1, 0);
    set_bg_color(&control);
    draw_tile(&control, 30, 30);
    SDL_RenderPresent(control.rend);
}

void cleanup (void)
{
	/* Shut down SDL */
	SDL_Quit();
}
