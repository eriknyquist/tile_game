#include "defs.h"
#include "map.h"
#include "tile.h"
#include "tile_collisions.h"
#include "player.h"

/* Sky blue */
const uint8_t bgcolor[3] = {102, 204, 255};

/* set_bg_color: draws the background colour on the entire window */
void set_bg_color(ctrl_t *ctrl)
{
    SDL_SetRenderDrawColor(ctrl->rend, bgcolor[0], bgcolor[1], bgcolor[2], 255);
    SDL_RenderClear(ctrl->rend);
}

/* do_map: draws the on-screen tiles from the currently loaded map, based on the
 * the current keyboard inputs */
void do_map (ctrl_t *ctrl)
{
    int pixels;
    int dist;
    int x, y, maxp;

    /* Number of pixels to move the map */
    pixels = MAP_PIXELS;

    /* Maximum starting position in the map array, taking
     * screen width into account */
    maxp = ctrl->map.max_x - (XTILES_WIDTH / 2) - 1;

    /* Left keypress: scroll map to the right */
    if (ctrl->input.left && (ctrl->pos > 0 || ctrl->offset < 0)) {
        dist = player_distance_left(ctrl);
        if (dist >= 0 && pixels >= dist) {
            pixels = dist - 1;
        }

        /* Handle positioning of map between tile boundaries */
        if (((ctrl->offset + pixels) > TILE_SIZE)) {
            ctrl->offset = (ctrl->offset + pixels) % TILE_SIZE;

            if (ctrl->pos >= 1)
                ctrl->pos -= 1;
        } else {
            ctrl->offset += pixels;
        }
    }

    /* Right keypress: scroll map to the left */
    if (ctrl->input.right && ctrl->pos < maxp) {
        dist = player_distance_right(ctrl);
        if (dist >= 0 && pixels >= dist) {
            pixels = dist - 1;
        }

        /* Handle positioning of map between tile boundaries */
        if ((ctrl->offset - pixels) < 0) {
            ctrl->offset = TILE_SIZE + (ctrl->offset - pixels);
            ctrl->pos += 1;
        } else {
            ctrl->offset -= pixels;
        }
    }

    /* Reset on-screen collider array */
    memset(ctrl->colliders, 0,
        sizeof(ctrl->colliders[0][0]) * YTILES_HEIGHT * (XTILES_WIDTH + 1));

    /* Draw background colour */
    set_bg_color(ctrl);

    /* Draw visible tiles from the map on the screen */
    for (y = 0; y < YTILES_HEIGHT; ++y) {
        for (x = (ctrl->pos) ? -1 : 0; x < XTILES_WIDTH + 1; ++x) {
            if (ctrl->map.data[y][ctrl->pos + x] > 0) {
                /* Draw a new tile here at (x,y) */
                 ctrl->colliders[y][x] =
                     draw_tile(ctrl,
                               (x * TILE_SIZE) + ctrl->offset,
                               y * TILE_SIZE);
            }
        }
    }
}

/* map_reset: resets the player and map to starting positions */
void reset_map (ctrl_t *ctrl)
{
    ctrl->player.rect.x = (XTILES_WIDTH * TILE_SIZE)/ 2;
    ctrl->player.rect.y = TILE_SIZE * ctrl->map.start_y + 1;
    ctrl->pos = ctrl->map.start_x - (XTILES_WIDTH / 2);
    ctrl->player.yvelocity = 0;
    ctrl->offset = 0;
}

/* map_from_file: opens file 'filename', and reads map data
 * into 'map' structure. Returns 0 if successful, otherwise -1 */
int map_from_file (map_t *map, char *filename)
{
    FILE *fp;
    unsigned int x;
    unsigned int y;
    char c;

    x = XTILES_WIDTH / 2;
    y = map->max_x = 0;

    if ((fp = fopen(filename, "rb")) == NULL) {
        return -1;
    }

    map_zero(map);

    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n' || x >= (MAX_X + (XTILES_WIDTH / 2))) {
            if ((x + 1) > map->max_x)
                map->max_x = x + 1;

            x = XTILES_WIDTH / 2;
            ++y;

            if (y >= MAX_Y)
                return 0;

        } else {
            switch (c) {
                case '*':
                    map->data[y][x] = 1;
                break;
                case '@':
                    map->start_x = x;
                    map->start_y = y;
                break;
            }

            ++x;
        }
    }

    fclose(fp);
    return 0;
}
