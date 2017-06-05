#include "defs.h"

SDL_Window *window;
SDL_Renderer *renderer;

/* Sky blue */
const uint8_t bgcolor[3] = {102, 204, 255};

void set_bg_color(SDL_Window *win, SDL_Renderer *render)
{
    SDL_SetRenderDrawColor(render, bgcolor[0], bgcolor[1], bgcolor[2], 255);
    SDL_RenderClear(render);
    SDL_RenderPresent(render);
}

void init (char *title)
{
	/* Initialise SDL Video */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Could not initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, SWIDTH, SHEIGHT, 0);

    if (window == NULL) {
        printf("Couldn't create %dx%d window: %s\n", SWIDTH, SHEIGHT, 
            SDL_GetError());
		exit(1);
	}

    renderer = SDL_CreateRenderer(window, -1, 0);
    set_bg_color(window, renderer);
}

void cleanup (void)
{
	/* Shut down SDL */
	SDL_Quit();
}
