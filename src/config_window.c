#include "defs.h"
#include "utils.h"
#include "text.h"
#include "colours.h"
#include "config_window.h"
#include "game_window_init.h"
#include "menu.h"
#include "scenes.h"

#define NUM_FPS_OPTIONS 7
#define NUM_RES_OPTIONS 6

static menu_t menu;

/* Menu data for FPS options */
static char *fps_option_labels[NUM_FPS_OPTIONS] = {
    "10", "15", "20", "24", "25", "30", "60"
};

static int fps_option_values[NUM_FPS_OPTIONS] = {10, 15, 20, 24, 25, 30, 60};


/* Menu data for resolution options */
static char *res_option_labels[NUM_RES_OPTIONS] = {
    "1152x648", "1280x720", "1366x768", "1600x900", "1920x1080", "2569x1440"
};

static int res_option_values[NUM_RES_OPTIONS][3] = {
/*  width    height   font size */
    {1152,   648,     35},
    {1280,   720,     38},
    {1366,   768,     40},
    {1600,   900,     48},
    {1920,   1080,    58},
    {2560,   1440,    77}
};

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

static void fps_setval (int id, ctrl_t *ctrl, game_t *game)
{
    game->fps = fps_option_values[id];
}

static void resolution_setval (int id, ctrl_t *ctrl, game_t *game)
{
    ctrl->screen_width = res_option_values[id][0];
    ctrl->screen_height = res_option_values[id][1];
    ctrl->font_size = res_option_values[id][2];
}

static void vsync_setval (int id, ctrl_t *ctrl, game_t *game)
{
    ctrl->vsync = id;
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

void add_menu_option_values(menu_t *menu, char *labels[], int selected, int num)
{
    int i;

    for (i = 0; i < num; ++i) {
        menu_add_value(menu, labels[i], i, (selected == i) ? 1 : 0);
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
    add_menu_option_values(&menu, fps_option_labels, 5, NUM_FPS_OPTIONS);


    menu_add_option(&menu, "Resolution", resolution_setval, 0);
    add_menu_option_values(&menu, res_option_labels, 3, NUM_RES_OPTIONS);

    menu_add_option(&menu, "VSync", vsync_setval, 0);
    menu_add_value(&menu, "no", 0, 0);
    menu_add_value(&menu, "yes", 1, 1);

    menu_add_option(&menu, "Play!", NULL, 1);
    menu_draw(ctrl, &menu, CONFIG_WINDOW_WIDTH, CONFIG_WINDOW_HEIGHT);
}

int draw_config_window (ctrl_t *ctrl, game_t *game)
{
    int ret;

    ret = 0;

    if (ctrl->input.down) {
        menu_cursor_down(&menu);
        ctrl->input.down = 0;
    } else if (ctrl->input.up) {
        menu_cursor_up(&menu);
        ctrl->input.up = 0;
    } else if (ctrl->input.left) {
        menu_current_down(&menu);
        ctrl->input.left = 0;
    } else if (ctrl->input.enter || ctrl->input.right) {
        if (menu.li == menu.max) {
            /* Last menu item-- "Play!". Start the game */
            menu_set_values(&menu, ctrl, game);
            start_game(ctrl, game);
            ret = 1;
        } else if (menu.options[menu.li].max > 0) {
            menu_current_up(&menu);
        }

        ctrl->input.enter = 0;
        ctrl->input.right = 0;
    } else {
        return ret;
    }

    menu_draw(ctrl, &menu, CONFIG_WINDOW_WIDTH, CONFIG_WINDOW_HEIGHT);
    return ret;
}
