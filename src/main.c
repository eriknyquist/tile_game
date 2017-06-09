#include "defs.h"
#include "init.h"
#include "input.h"

#define FPS 60
#define MS_PER_FRAME (1000 / FPS)

ctrl_t control;

uint32_t frame_timer(uint32_t interval, void *param)
{
    SDL_Event event;
    SDL_UserEvent userevent;

    /* In this example, our callback pushes an SDL_USEREVENT event
     * into the queue, and causes our callback to be called again at the
     * same interval: */

    userevent.type = SDL_USEREVENT;
    userevent.code = 0;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return interval;
}

int main(int argc, char *argv[])
{
    SDL_TimerID timer;
    SDL_Event event;

    /* Start up SDL */
    init(&control, "Tile map engine");

    /* Call the cleanup function when the program exits */
    atexit(cleanup);

    /* Start the frame timer */
    timer = SDL_AddTimer(MS_PER_FRAME, frame_timer, NULL);

    while (SDL_WaitEvent(&event))
        process_event(&event, &control);

    SDL_RemoveTimer(timer);
    exit(0);
}
