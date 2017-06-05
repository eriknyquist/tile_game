#include "defs.h"

ctrl_t ctrl;
SDL_Window *window;
SDL_Renderer *renderer;

/* Sky blue */
const uint8_t bgcolor[3] = {102, 204, 255};

void set_bg_color(ctrl_t *ctrl)
{
    SDL_SetRenderDrawColor(ctrl->rend, bgcolor[0], bgcolor[1], bgcolor[2], 255);
    SDL_RenderClear(ctrl->rend);
    SDL_RenderPresent(ctrl->rend);
}

void init (char *title)
{
	/* Initialise SDL Video */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Could not initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

    ctrl.win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, SWIDTH, SHEIGHT, 0);

    if (ctrl.win == NULL) {
        printf("Couldn't create %dx%d window: %s\n", SWIDTH, SHEIGHT, 
            SDL_GetError());
		exit(1);
	}

    ctrl.rend = SDL_CreateRenderer(ctrl.win, -1, 0);
    set_bg_color(&ctrl);
}

void cleanup (void)
{
	/* Shut down SDL */
	SDL_Quit();
}
