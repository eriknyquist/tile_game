#include "defs.h"
#include "config_window.h"
#include "scenes.h"
#include "input.h"
#include "game_window_init.h"
#include "bitmap_manager.h"
#include "frame.h"
#include "text.h"

static ctrl_t control;
static game_t game;

/* Closes game window and shuts down all initialised SDL subsystems */
static void cleanup (void)
{
    if (control.rend) SDL_DestroyRenderer(control.rend);
    if (control.win) SDL_DestroyWindow(control.win);
    text_quit(&control);
    destroy_bitmaps();
    SDL_Quit();
}

static int do_game_loop (void)
{
    SDL_Event event;

    if (control.vsync) {
        while (1) {
            /* Empty the event queue, process events */
            while (SDL_PollEvent(&event))
                process_event(&event, &control, &game);

            /* Draw the current scene (blocks until next Vsync) */
            if (do_frame(&control, &game))
                return 1;
        }
    } else {
        /* Wait for the next event; in this case (no Vsync), a USEREVENT will
         * be generated when the frame timer expires, and process_event() will
         * be responsible for calling do_frame() */
        while (SDL_WaitEvent(&event))
            if (process_event(&event, &control, &game))
                return 1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    /* Call the cleanup function when the program exits */
    atexit(cleanup);

    memset(&game, 0, sizeof(game_t));
    game.dt_factor = 1.0;

    config_window_init(&control, &game);
    game.current_scene = draw_config_window;

    control.vsync = 1;
    while (1)
        do_game_loop();

    SDL_RemoveTimer(game.timer);
    exit(0);
}
