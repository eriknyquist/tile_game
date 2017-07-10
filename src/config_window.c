#include "defs.h"
#include "utils.h"
#include "text.h"
#include "colours.h"
#include "config_window.h"
#include "game_window_init.h"
#include "menu.h"
#include "scenes.h"

static menu_t menu;

static uint32_t frame_timer (uint32_t interval, void *param)
{
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = 0;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return interval;
}

static void fps_setval (option_t *opt, ctrl_t *ctrl, game_t *game)
{
    switch(opt->vi) {
        case 0:
            game->fps = 10;
        break;
        case 1:
            game->fps = 15;
        break;
        case 2:
            game->fps = 20;
        break;
        case 3:
            game->fps = 24;
        break;
        case 4:
            game->fps = 25;
        break;
        case 5:
            game->fps = 30;
        break;
        case 6:
            game->fps = 60;
        break;
    }
}

static void resolution_setval (option_t *opt, ctrl_t *ctrl, game_t *game)
{
    switch(opt->vi) {
        case 0:
            ctrl->screen_width = 1152;
            ctrl->screen_height = 648;
            ctrl->font_size = 35;
        break;
        case 1:
            ctrl->screen_width = 1280;
            ctrl->screen_height = 720;
            ctrl->font_size = 38;
        break;
        case 2:
            ctrl->screen_width = 1366;
            ctrl->screen_height = 768;
            ctrl->font_size = 40;
        break;
        case 3:
            ctrl->screen_width = 1600;
            ctrl->screen_height = 900;
            ctrl->font_size = 48;
        break;
        case 4:
            ctrl->screen_width = 1920;
            ctrl->screen_height = 1080;
            ctrl->font_size = 58;
        break;
        case 5:
            ctrl->screen_width = 2560;
            ctrl->screen_height = 1440;
            ctrl->font_size = 77;
        break;
    }
}

static void vsync_setval (option_t *opt, ctrl_t *ctrl, game_t *game)
{
    ctrl->vsync = opt->vi;
}

static void start_game (ctrl_t *ctrl, game_t *game)
{
    text_quit(ctrl);
    SDL_DestroyRenderer(ctrl->rend);
    SDL_DestroyWindow(ctrl->win);
    game_window_init(ctrl, game, "Tile game");
    game_init(ctrl, game);

    if (!ctrl->vsync) {
        /* Start the frame timer */
        game->timer = SDL_AddTimer(1000 / game->fps, frame_timer, NULL);
    }
}

/* Initialises SDL and opens the main game window */
void config_window_init (ctrl_t *ctrl, game_t *game)
{
    memset(ctrl, 0, sizeof(ctrl_t));

    /* Initialise SDL Video */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    /* Create main window & renderer */
    ctrl->win = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, CONFIG_WINDOW_WIDTH, CONFIG_WINDOW_HEIGHT, 0);

    if (ctrl->win == NULL) {
        fprintf(stderr, "Couldn't create %dx%d window: %s\n",
            CONFIG_WINDOW_WIDTH, CONFIG_WINDOW_HEIGHT, SDL_GetError());
        exit(1);
    }

    ctrl->rend = SDL_CreateRenderer(ctrl->win, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    /* Build game config. menu */
    text_init(ctrl, 48);
    menu_init(&menu);
    menu_add_option(&menu, "FPS", fps_setval, 0);
    menu_add_value(&menu, "10", 0);
    menu_add_value(&menu, "15", 0);
    menu_add_value(&menu, "20", 0);
    menu_add_value(&menu, "24", 0);
    menu_add_value(&menu, "25", 0);
    menu_add_value(&menu, "30", 1);
    menu_add_value(&menu, "60", 0);

    menu_add_option(&menu, "VSync", vsync_setval, 0);
    menu_add_value(&menu, "no", 0);
    menu_add_value(&menu, "yes", 1);

    menu_add_option(&menu, "Resolution", resolution_setval, 0);
    menu_add_value(&menu, "1152x648", 0);
    menu_add_value(&menu, "1280x720", 0);
    menu_add_value(&menu, "1366x768", 0);
    menu_add_value(&menu, "1600x900", 1);
    menu_add_value(&menu, "1920x1080", 0);
    menu_add_value(&menu, "2569x1440", 0);

    menu_add_option(&menu, "Play!", NULL, 1);
    menu_draw(ctrl, &menu, CONFIG_WINDOW_WIDTH, CONFIG_WINDOW_HEIGHT);
}

int draw_config_window (ctrl_t *ctrl, game_t *game)
{
    int i, ret;

    ret = 0;

    if (ctrl->input.down) {
        menu.li = (menu.li + 1) % (menu.max + 1);
        ctrl->input.down = 0;
    } else if (ctrl->input.up) {
        if (menu.li == 0)
            menu.li = menu.max;
        else
            --menu.li;

        ctrl->input.up = 0;
    } else if (ctrl->input.enter) {
        if (menu.li == menu.max) {
            /* Last menu item-- "Play!". Start the game */
            menu_set_values(&menu, ctrl, game);
            start_game(ctrl, game);
            ret = 1;
        } else if (menu.options[menu.li].max > 0) {
            i = menu.options[menu.li].vi;
            menu.options[menu.li].vi = (i + 1) % menu.options[menu.li].max;
        }

        ctrl->input.enter = 0;
    } else {
        return ret;
    }

    menu_draw(ctrl, &menu, CONFIG_WINDOW_WIDTH, CONFIG_WINDOW_HEIGHT);
    return ret;
}
