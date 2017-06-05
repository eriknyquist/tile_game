#include "defs.h"

void get_input (input_t *input)
{
	SDL_Event event;

    /* Clear the input store from the last frame */
    memset(input, 0, sizeof(input_t));

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				exit(0);
			break;
			
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						exit(0);
					break;
			        case SDLK_LEFT:
                        input->left = 1;
                    break;
			        case SDLK_RIGHT:
                        input->right = 1;
                    break;
				}
			break;
		}
	}
}
