#include "defs.h"
#include "text.h"
#include "map.h"
#include "bitmap_manager.h"

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

    if (ctrl->fullscreen) {
        if (SDL_SetWindowFullscreen(ctrl->win, SDL_WINDOW_FULLSCREEN) < 0) {
            fprintf(stderr, "Couldn't set window to fullscreen: %s\n",
                SDL_GetError());
            exit(1);
        }
    }

    /* Set font size based on game windows size */
    ctrl->font_size = ctrl->screen_height / SCREEN_FONT_FACTOR;

    if (ctrl->vsync)
        game->rflags |= SDL_RENDERER_PRESENTVSYNC;

    ctrl->rend = SDL_CreateRenderer(ctrl->win, -1, game->rflags);
    if (ctrl->rend == NULL) {
        fprintf(stderr, "Couldn't create renderer: %s\n", SDL_GetError());
        exit(1);
    }

    ctrl->images.bg_fixed =
        load_bitmap(ctrl, "bitmaps/sky.bmp", &ctrl->bg_rect);
    ctrl->images.hud_life =
        load_bitmap(ctrl, "bitmaps/hud_life_icon.png", NULL);

    text_init(ctrl, ctrl->font_size);
}

/* Loads data for map 1, initialises timer count value and player screen
 * position */
void game_init (ctrl_t *ctrl, game_t *game)
{
    /* Initialise player & screen position */
    ctrl->lastframe = SDL_GetTicks();
    ctrl->player.rect.w = PLAYER_SIZE;
    ctrl->player.rect.h = PLAYER_SIZE;
    ctrl->max_blocks = 1;
    ctrl->lives = 5;

    if (next_level(ctrl, game) != 0) {
        fprintf(stderr, "Error: unable to load map files for level 1\n");
    }
}
