#ifndef TILE_PHYSICS_H_
#define TILE_PHYSICS_H_

void add_gravity(ctrl_t *ctrl, moveable_t *obj);

int tile_distance_left (ctrl_t *ctrl, moveable_t *obj);
int tile_distance_right (ctrl_t *ctrl, moveable_t *obj);
int tile_distance_up (ctrl_t *ctrl, moveable_t *obj);
int tile_distance_down (ctrl_t *ctrl, moveable_t *obj);

double clip_movement (double movement, int distance, double backup);
void tile_collisions_top (ctrl_t *ctrl, game_t *game, moveable_t *obj);
void tile_collisions_bottom (ctrl_t *ctrl, game_t *game, moveable_t *obj);

#endif
