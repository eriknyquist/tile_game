#include "defs.h"
#include "init.h"
#include "input.h"
#include "map.h"

int main(int argc, char *argv[])
{
    /* Start up SDL */
    init("Tile map engine");

    /* Call the cleanup function when the program exits */
    atexit(cleanup);
    
    while (1) {
        get_input(&control.input);
        do_map(&control);

        SDL_RenderPresent(control.rend);
        SDL_Delay(16);
    }
    
    exit(0);
}
