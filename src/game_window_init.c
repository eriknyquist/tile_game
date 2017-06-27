#include "defs.h"
#include "map.h"

#if VSYNC
#define RENDER_FLAGS (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
#else
#define RENDER_FLAGS 0
#endif /* VSYNC */

/* Initialises SDL and opens the main game window */
void game_window_init (ctrl_t *ctrl, char *title)
{
    memset(ctrl, 0, sizeof(ctrl_t));

    ctrl->map.start_x = XTILES_WIDTH / 2;
    ctrl->map.start_y = 1;

    /* Initialise SDL Video */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    /* Create main window & renderer */
    ctrl->win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, SWIDTH, SHEIGHT, 0);

    if (ctrl->win == NULL) {
        printf("Couldn't create %dx%d window: %s\n", SWIDTH, SHEIGHT,
            SDL_GetError());
        exit(1);
    }

    ctrl->rend = SDL_CreateRenderer(ctrl->win, -1, RENDER_FLAGS);
}

/* Loads data for map 1, initialises timer count value and player screen
 * position */
void game_init (ctrl_t *ctrl)
{
    /* Load map & background data from files */
    if (load_map(&ctrl->map, 1) != 0) {
        printf("Error reading map data\n");
        exit(1);
    }

    /* Initialise player & screen position */
    ctrl->lastframe = SDL_GetTicks();
    ctrl->player.rect.w = PLAYER_SIZE;
    ctrl->player.rect.h = PLAYER_SIZE;
    reset_map(ctrl);
}

/* Closes game window and shuts down all initialised SDL subsystems */
void game_window_cleanup (void)
{
    /* Shut down SDL */
    SDL_Quit();
}
