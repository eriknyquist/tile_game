#include "defs.h"

SDL_Window *window;

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
}

void cleanup (void)
{
	/* Shut down SDL */
	SDL_Quit();
}
