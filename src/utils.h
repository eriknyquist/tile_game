#ifndef UTILS_H_
#define UTILS_H_

void draw_coloured_rect (ctrl_t *ctrl, SDL_Rect *rect, SDL_Colour c);
int min_positive (int a, int b);
void draw_bg_colour (ctrl_t *ctrl, const SDL_Color colour);
int trajectory_range (ctrl_t *ctrl, int xvelocity, int yvelocity);

#endif
