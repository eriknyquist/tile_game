#include "defs.h"
#include "game_window_init.h"
#include "scenes.h"
#include "input.h"
#include "frame.h"

ctrl_t control;
game_t game;

#if !VSYNC
uint32_t frame_timer(uint32_t interval, void *param)
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
#endif /* !VSYNC */

int main(int argc, char *argv[])
{
    SDL_Event event;
#if !VSYNC
    SDL_TimerID timer;

    /* Start the frame timer */
    timer = SDL_AddTimer(MS_PER_FRAME, frame_timer, NULL);
#endif /* !VSYNC */

    /* Call the cleanup function when the program exits */
    atexit(game_window_cleanup);

    /* Initialize main window */
    game_window_init(&control, "Tile map engine");
    game_init(&control);

    /* Go straight to game physics engine (no menus yet) */
    game.current_scene = draw_scene_game;

#if VSYNC
    while (1) {
        /* Empty the event queue, process events */
        while (SDL_PollEvent(&event))
            process_event(&event, &control, &game);

        /* Draw the current scene (blocks until next Vsync) */
        do_frame(&control, &game);
    }
#else
    /* Wait for the next event; in this case (no Vsync), a USEREVENT will
     * be generated when the frame timer expires, and process_event() will
     * be responsible for calling do_frame() */
    while (SDL_WaitEvent(&event))
        process_event(&event, &control);

    SDL_RemoveTimer(timer);
#endif /* VSYNC */

    exit(0);
}
