#ifndef MAP_H_
#define MAP_H_

#define map_zero(map) \
    memset(map->data, 0,\
           sizeof(map->data[0][0]) * (MAX_X + XTILES_WIDTH + 1) * MAX_Y)

void do_map (ctrl_t *ctrl);
void reset_map (ctrl_t *ctrl);
int map_from_file (map_t *map, char *filename);

#endif
