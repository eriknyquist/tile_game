#ifndef MAP_H_
#define MAP_H_

#define bg_zero(map) \
    memset(map->bg, 0, \
            sizeof(map->bg[0][0]) * BG_MAX_Y * BG_MAX_X)

#define map_zero(map) \
    memset(map->data, 0,\
           sizeof(map->data[0][0]) * (MAX_X + XTILES_WIDTH + 1) * MAX_Y)

void do_map (ctrl_t *ctrl);
void draw_map (ctrl_t *ctrl);
void reset_map (ctrl_t *ctrl);
int load_map (map_t *map, unsigned int level_num);

#endif
