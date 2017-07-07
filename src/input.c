#include "defs.h"
#include "input.h"
#include "frame.h"
#include "scenes.h"

static uint8_t paused;

static void set_key_state (ctrl_t *ctrl, game_t *game,
        int key, unsigned int val)
{
    switch (key) {
        case SDLK_ESCAPE:
            exit(0);
        break;
        case SDLK_LEFT:
            ctrl->input.left = val;
        break;
        case SDLK_RIGHT:
            ctrl->input.right = val;
        break;
        case SDLK_UP:
            ctrl->input.up = val;
        break;
        case SDLK_DOWN:
            ctrl->input.down = val;
        break;
        case SDLK_RETURN:
            ctrl->input.enter = val;
        break;
        case SDLK_SPACE:
            ctrl->input.space = val;
        break;
        case SDLK_RSHIFT:
            ctrl->input.shift = val;
        break;
        case SDLK_p:
            if (val) {
                game->current_scene = (paused) ? draw_scene_game :
                    draw_scene_game_paused;
                paused = !paused;
            }

        break;
        case SDLK_t:
            ctrl->input.t = val;
        break;
    }
}

int process_event (SDL_Event *event, ctrl_t *ctrl, game_t *game)
{
    switch (event->type) {
        case SDL_USEREVENT:
            /* Frame timer expired; draw the next frame */
            return do_frame(ctrl, game);
        break;
        case SDL_KEYDOWN:
            /* Key down event; store it in the input struct */
            set_key_state(ctrl, game, event->key.keysym.sym, 1);
        break;
        case SDL_KEYUP:
            /* Key up event; store it in the input struct */
            set_key_state(ctrl, game, event->key.keysym.sym, 0);
        break;
        case SDL_QUIT:
            exit(0);
        break;
    }

    return 0;
}
