#include "defs.h"
#include "init.h"
#include "input.h"
#include "map.h"

#define FPS 60
#define TICKS_PER_FRAME (1000 / FPS)

int main(int argc, char *argv[])
{
    /* Start up SDL */
    init("Tile map engine");

    /* Call the cleanup function when the program exits */
    atexit(cleanup);
    
    while (1) {
        if ((SDL_GetTicks() - control.lastframe) >= TICKS_PER_FRAME) {
            get_input(&control.input);
            do_map(&control);

            SDL_RenderPresent(control.rend);
            control.lastframe = SDL_GetTicks();
        }
    }
    
    exit(0);
}
