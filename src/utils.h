#ifndef UTILS_H_
#define UTILS_H_

int min_positive (int a, int b);
int trajectory_range (ctrl_t *ctrl, int xvelocity, int yvelocity);

void death (game_t *game);
int set_tile_by_screen (ctrl_t *ctrl, uint8_t sym, int x, int y, uint8_t value);
void winning_scene (ctrl_t *ctrl, game_t *game);
void cut_to_text (game_t *game, char *text, unsigned int len,
    unsigned int secs);

void draw_coloured_rect (ctrl_t *ctrl, SDL_Rect *rect, SDL_Colour c);
void draw_bg_colour (ctrl_t *ctrl, const SDL_Color colour);
void draw_bg_bmp (ctrl_t *ctrl);

#endif
