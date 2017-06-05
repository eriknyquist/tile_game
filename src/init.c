#include "defs.h"

SDL_Surface *screen;

void init (char *title)
{
	/* Initialise SDL Video */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Could not initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	
	/* Open a 640 x 480 screen */
	screen = SDL_SetVideoMode(SWIDTH, SHEIGHT, 0, SDL_HWPALETTE);
	
	if (screen == NULL) {
		printf("Couldn't set screen mode to %dx%d: %s\n", SWIDTH, SHEIGHT,
                SDL_GetError());
		exit(1);
	}
	
	/* Set the screen title */
	SDL_WM_SetCaption(title, NULL);
}

void cleanup (void)
{
	/* Shut down SDL */
	SDL_Quit();
}
