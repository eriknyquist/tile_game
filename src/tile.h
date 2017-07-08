#ifndef TILE_H_
#define TILE_H_

#define NO_TILE       '\0'
#define FIXED_TILE    '*'
#define MOVEABLE_TILE 'x'
#define PLAYER_START  'S'
#define PLAYER_FINISH 'F'

SDL_Rect draw_map_tile (ctrl_t *ctrl, int pos_x, int pos_y);
SDL_Rect draw_moveable_tile (ctrl_t *ctrl, int pos_x, int pos_y);
SDL_Rect draw_tile (ctrl_t *ctrl, uint8_t sym, int pos_x, int pos_y);
void draw_bg_tile (ctrl_t *ctrl, int pos_x, int pos_y);

#endif
