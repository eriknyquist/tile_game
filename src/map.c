#include "defs.h"
#include "map.h"
#include "tile.h"
#include "math.h"
#include "tile_physics.h"
#include "colours.h"
#include "utils.h"
#include "text.h"

#define LINE_ENDING(c) (c == '\n' || c == '\r')

static void draw_block_counter (ctrl_t *ctrl)
{
    char buf[10];

    snprintf(buf, sizeof(buf), "%u/%d", ctrl->blocks, MAX_BLOCKS);
    text_draw(ctrl, buf, 100, 100);
}

static void draw_bg_tiles (ctrl_t *ctrl)
{
    unsigned int map_index;
    int x, y;

    /* Draw background scenery tiles */
    for (y = 0; y < BG_YTILES_HEIGHT; ++y) {
        for (x = 0; x < BG_XTILES_WIDTH + 2; ++x) {
            map_index = (ctrl->bgpos + x) % ctrl->map.bg_max_x;

            if (ctrl->map.bg[y][map_index] > 0) {
                draw_bg_tile(ctrl,
                        ((x * BG_TILE_SIZE) + ctrl->bgoffset) - BG_TILE_SIZE,
                        y * BG_TILE_SIZE);
            }
        }
    }
}

/* draw_bg_scenery: draws the on-screen tiles from the currently loaded
 * background, based on the current keyboard inputs, and the limiting 'pixels'
 * value provided. The BG scenery will never be moved by more than 'pixels',
 * despite keyboard inputs */
static void do_bg (ctrl_t *ctrl, float map_pixels)
{
    float pixels;

    pixels = (map_pixels < BG_PIXELS) ? map_pixels : BG_PIXELS;

    if (ctrl->input.right && ctrl->pos < ctrl->map.max_p) {
        /* Handle positioning of BG scenery between tile boundaries */
        if ((ctrl->bgoffset - pixels) < 0) {
            ctrl->bgoffset = BG_TILE_SIZE + (ctrl->bgoffset - pixels);
            ctrl->bgpos = fmod(ctrl->bgpos + 1.0, ctrl->map.bg_max_x);
        } else {
            ctrl->bgoffset -= pixels;
        }
    }

    if (ctrl->input.left && (ctrl->pos > 0 || ctrl->offset < 0)) {
        /* Handle positioning of BG scenery between tile boundaries */
        if (((ctrl->bgoffset + pixels) > BG_TILE_SIZE)) {
            ctrl->bgoffset = fmod(ctrl->bgoffset + pixels, BG_TILE_SIZE);

            if (ctrl->bgpos >= 1)
                ctrl->bgpos -= 1;
            else
                ctrl->bgpos = ctrl->map.bg_max_x - 1;

        } else {
            ctrl->bgoffset += pixels;
        }
    }
}

static void draw_map_tiles (ctrl_t *ctrl)
{
    uint8_t sym;
    int x, y;

    /* Draw visible tiles from the map on the screen */
    for (y = 0; y < YTILES_HEIGHT; ++y) {
        for (x = (ctrl->pos) ? -1 : 0; x < XTILES_WIDTH + 1; ++x) {
            /* Get this tile's symbol */
            sym = ctrl->map.data[y][ctrl->pos + x];

            if (sym > 0) {
                /* Draw a new tile here at (x,y) */
                 ctrl->colliders[y][x] = draw_tile(ctrl, sym,
                        (x * TILE_SIZE) + ctrl->offset,
                         y * TILE_SIZE);
            }
        }
    }
}

/* do_map: draws the on-screen tiles from the currently loaded map, based on the
 * the current keyboard inputs, and the result of tile collision calculations
 * on the X-plane (tiles to the left or to the right of the player). Map
 * movement is restricted if there is a map tile blocking the player on this
 * plane, and that information gets passed on to draw_bg_scenery so that the
 * BG scenery's movement can be restricted too */
void do_map (ctrl_t *ctrl)
{
    float pixels;
    int dist;

    /* Number of pixels to move the map */
    pixels = MAP_PIXELS;

    /* Left keypress: scroll map to the right */
    if (ctrl->input.left && (ctrl->pos > 0 || ctrl->offset < 0)) {
        dist = tile_distance_left(ctrl, &ctrl->player);
        pixels = clip_movement(pixels, dist, -1.0);

        /* Handle positioning of map between tile boundaries */
        if (((ctrl->offset + pixels) > TILE_SIZE)) {
            ctrl->offset = fmod(ctrl->offset + pixels, TILE_SIZE);

            if (ctrl->pos >= 1)
                ctrl->pos -= 1;
        } else {
            ctrl->offset += pixels;
        }
    }

    /* Right keypress: scroll map to the left */
    if (ctrl->input.right && ctrl->pos < ctrl->map.max_p) {
        dist = tile_distance_right(ctrl, &ctrl->player);
        pixels = clip_movement(pixels, dist, 1.0);

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

    do_bg(ctrl, pixels);
}

void draw_map (ctrl_t *ctrl)
{
    draw_bg_bmp(ctrl);
    draw_bg_tiles(ctrl);
    draw_map_tiles(ctrl);
    draw_block_counter(ctrl);
}

/* map_reset: resets the player and map to starting positions */
void reset_map (ctrl_t *ctrl)
{
    ctrl->player.rect.x = (XTILES_WIDTH * TILE_SIZE)/ 2;
    ctrl->player.rect.y = TILE_SIZE * ctrl->map.start_y + 1;
    ctrl->pos = ctrl->map.start_x - (XTILES_WIDTH / 2);
    ctrl->player.yvelocity = 0;
    ctrl->offset = 0;
    ctrl->bgpos = 0;
    ctrl->bgoffset = 0;
}

/* Detect a sequence of '\r\n' or '\n\r' in the file stream, so we
 * can treat it as a single line ending */
static int eat_line_endings(char c, FILE *fp)
{
    char last;

    if (!LINE_ENDING(c))
        return 0;

    last = c;
    while(((c = fgetc(fp)) != last) && LINE_ENDING(c))
        last = c;

    ungetc(c, fp);
    return 1;
}

/* map_from_file: opens file 'filename', and reads map data
 * into 'map' structure. Returns 0 if successful, otherwise -1 */
static int map_from_file (ctrl_t *ctrl, char *filename)
{
    FILE *fp;
    unsigned int x;
    unsigned int y;
    char c;

    x = XTILES_WIDTH / 2;
    y = ctrl->map.max_x = 0;

    if ((fp = fopen(filename, "rb")) == NULL) {
        return -1;
    }

    map_zero(ctrl);

    while ((c = fgetc(fp)) != EOF) {
        if (eat_line_endings(c, fp) || x >= (MAX_X + (XTILES_WIDTH / 2))) {
            if ((x + 1) > ctrl->map.max_x)
                ctrl->map.max_x = x + 1;

            x = XTILES_WIDTH / 2;
            ++y;

            if (y >= MAX_Y)
                return 0;

        } else {
            switch (c) {
                case FIXED_TILE:
                case MOVEABLE_TILE:
                    ctrl->map.data[y][x] = c;
                break;
                case PLAYER_START:
                    ctrl->map.start_x = x;
                    ctrl->map.start_y = y;
                break;
            }

            ++x;
        }
    }

    fclose(fp);
    return 0;
}

static int bg_from_file(ctrl_t *ctrl, char *filename)
{
    FILE *fp;
    unsigned int x;
    unsigned int y;
    char c;

    x = 0;
    y = ctrl->map.bg_max_x = 0;

    if ((fp = fopen(filename, "rb")) == NULL) {
        return -1;
    }

    bg_zero(ctrl);

    while ((c = fgetc(fp)) != EOF) {
        if (eat_line_endings(c, fp) || x >= BG_MAX_X) {
            if (x > ctrl->map.bg_max_x)
                ctrl->map.bg_max_x = x;

            x = 0;
            ++y;

            if (y >= BG_MAX_Y)
                return 0;

        } else {
            switch (c) {
                case '*':
                    ctrl->map.bg[y][x] = 1;
                break;
            }

            ++x;
        }
    }

    fclose(fp);
    return 0;
}

int load_map (ctrl_t *ctrl, unsigned int num)
{
    char filename[256];

    snprintf(filename, sizeof(filename), "maps/%d/%s", num, MAP_FILE_NAME);
    if (map_from_file(ctrl, filename) != 0) {
        return -1;
    }

    snprintf(filename, sizeof(filename), "maps/%d/%s", num, BG_FILE_NAME);
    if (bg_from_file(ctrl, filename) != 0) {
        return -1;
    }

    /* Maximum starting position in the map array, taking
     * screen width into account */
    ctrl->map.max_p = ctrl->map.max_x - (XTILES_WIDTH / 2) - 1;

    return 0;
}
