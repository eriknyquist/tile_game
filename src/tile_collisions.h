#ifndef TILE_COLLISIONS_H_
#define TILE_COLLISIONS_H_

#define BELOW_MAP -1
#define NO_TILES  -2

int tile_obstacle_right (ctrl_t *ctrl, int x, int y);
int tile_obstacle_left (ctrl_t *ctrl, int x, int y);
int tile_obstacle_up (ctrl_t *ctrl, int x, int y);
int tile_obstacle_down (ctrl_t *ctrl, int x, int y);

#endif
