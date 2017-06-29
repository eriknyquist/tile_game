#include "defs.h"
#include "map.h"

/* Initialises SDL and opens the main game window */
void game_window_init (ctrl_t *ctrl, game_t *game, char *title)
{
    ctrl->map.start_x = XTILES_WIDTH / 2;
    ctrl->map.start_y = 1;

    /* Create main window & renderer */
    ctrl->win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, ctrl->screen_width, ctrl->screen_height, 0);

    if (ctrl->win == NULL) {
        fprintf(stderr, "Couldn't create %dx%d window: %s\n",
            ctrl->screen_width, ctrl->screen_height, SDL_GetError());
        exit(1);
    }

    if (ctrl->vsync)
        game->rflags |= SDL_RENDERER_PRESENTVSYNC;

    ctrl->rend = SDL_CreateRenderer(ctrl->win, -1, game->rflags);
}

/* Loads data for map 1, initialises timer count value and player screen
 * position */
void game_init (ctrl_t *ctrl)
{
    /* Load map & background data from files */
    if (load_map(ctrl, 1) != 0) {
        fprintf(stderr, "Error reading map data\n");
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
