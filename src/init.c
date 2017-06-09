#include "defs.h"
#include "map.h"

#define DEFAULT_MAP "maps/1.dat"

/* #define RENDER_FLAGS SDL_RENDERER_PRESENTVSYNC */
#define RENDER_FLAGS 0

void init (ctrl_t *ctrl, char *title)
{
    memset(ctrl, 0, sizeof(ctrl_t));

    /* Initialise SDL Video */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    if (map_from_file(&ctrl->map, DEFAULT_MAP) != 0) {
        printf("Error reading "DEFAULT_MAP"\n");
        exit(1);
    }

    ctrl->win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, SWIDTH, SHEIGHT, 0);

    if (ctrl->win == NULL) {
        printf("Couldn't create %dx%d window: %s\n", SWIDTH, SHEIGHT, 
            SDL_GetError());
        exit(1);
    }

    ctrl->player.rect.x = SWIDTH / 2;
    ctrl->player.rect.y = 50;
    ctrl->player.rect.w = PLAYER_SIZE;
    ctrl->player.rect.h = PLAYER_SIZE;
    ctrl->player.yvelocity = 0;

    ctrl->lastframe = SDL_GetTicks();
    ctrl->pos = ctrl->offset = 0;
    ctrl->rend = SDL_CreateRenderer(ctrl->win, -1, RENDER_FLAGS);
}

void cleanup (void)
{
    /* Shut down SDL */
    SDL_Quit();
}
