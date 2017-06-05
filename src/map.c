#include "defs.h"
#include "map.h"

void print_map(map_t *map)
{
    unsigned int x, y;

    for (y = 0; y < MAX_Y; ++y) {
        for (x = 0; x < map->max_x; ++x) {
            printf("%s", map->data[y][x] ? "*" : " ");
        }
        printf("\n");
    }
}

int map_from_file (map_t *map, char *filename)
{
    FILE *fp;
    unsigned int x;
    unsigned int y;
    char c;

    x = y = map->max_x = 0;

    if ((fp = fopen(filename, "rb")) == NULL) {
        return -1;
    }

    map_zero(map);

    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n' || x >= MAX_X) {
            if ((x + 1) > map->max_x)
                map->max_x = x + 1;

            x = 0;
            ++y;

            if (y >= MAX_Y)
                return 0;

        } else {
            if (c != ' ') {
                map->data[y][x] = 1;
            }

            ++x;
        }
    }

    fclose(fp);
    return 0;
}
