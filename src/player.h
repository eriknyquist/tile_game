#ifndef PLAYER_H_
#define PLAYER_H_

void init_player (ctrl_t *ctrl);
int collisions_right (ctrl_t *ctrl);
void do_player (ctrl_t *ctrl);
void draw_player (ctrl_t *ctrl);

int player_distance_up (ctrl_t *ctrl);
int player_distance_down (ctrl_t *ctrl);
int player_distance_left (ctrl_t *ctrl);
int player_distance_right (ctrl_t *ctrl);

#endif
