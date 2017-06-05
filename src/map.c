#include "defs.h"
#include "map.h"
#include "tile.h"

#define XTILES_WIDTH (SWIDTH / TILE_SIZE)

const int speed = 10;

/* Sky blue */
const uint8_t bgcolor[3] = {102, 204, 255};

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

void set_bg_color(ctrl_t *ctrl)
{
    SDL_SetRenderDrawColor(ctrl->rend, bgcolor[0], bgcolor[1], bgcolor[2], 255);
    SDL_RenderClear(ctrl->rend);
}

void do_map (ctrl_t *ctrl)
{
    int x, y, maxp;

    maxp = ctrl->map.max_x - XTILES_WIDTH;

    if (ctrl->input.left && (ctrl->pos > 0 || ctrl->offset < 0)) {
        if (((ctrl->offset + speed) > TILE_SIZE)) {
            ctrl->offset = (ctrl->offset + speed) % TILE_SIZE;

            if (ctrl->pos >= 1)
                ctrl->pos -= 1;
        } else {
            ctrl->offset += speed;
        }
    }

    if (ctrl->input.right && ((ctrl->pos <  maxp) || (ctrl->offset > 0))) {
        if ((ctrl->offset - speed) < 0) {
            ctrl->offset = TILE_SIZE - 1;
            if (ctrl->pos <  (maxp - 1))
                ctrl->pos += 1;
        } else {
            ctrl->offset -= speed;
        }
    }

    set_bg_color(ctrl);

    for (y = 0; y < MAX_Y; ++y) {
        for (x = (ctrl->pos) ? -1 : 0; x < XTILES_WIDTH + 2; ++x) {
            if (ctrl->map.data[y][ctrl->pos + x] > 0) {
                draw_tile(&control, (x * TILE_SIZE) + ctrl->offset,
                    y * TILE_SIZE);
            }
        }
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
