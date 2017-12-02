#include "defs.h"
#include "map.h"
#include "tile.h"
#include "math.h"
#include "tile_physics.h"
#include "colours.h"
#include "utils.h"
#include "text.h"
#include "scenes.h"

#define LINE_ENDING(c) (c == '\n' || c == '\r')

#define X_ACCEL(ctrl) ((ctrl->map.x_accel >= MAP_PIXELS) ? MAP_PIXELS : \
    ctrl->map.x_accel + 0.5)

#define BG_X_ACCEL(ctrl) \
    (((ctrl->map.x_accel / 2.0) >= BG_PIXELS) ? BG_PIXELS : \
    (ctrl->map.x_accel / 2.0))

#define SKY_X_ACCEL(ctrl) \
    (((ctrl->map.x_accel / 6.0) >= SKY_PIXELS) ? SKY_PIXELS : \
    (ctrl->map.x_accel / 6.0))

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

/* draw_bg_bmp: draws repeatable sky/cloud image */
void draw_bg_bmp (ctrl_t *ctrl)
{
    int i;

    SDL_RenderClear(ctrl->rend);
    ctrl->bg_rect.y = 0;

    for (i = -ctrl->bg_rect.w; i <= ctrl->screen_width; i += ctrl->bg_rect.w) {
        ctrl->bg_rect.x = i + ctrl->skyoffset;
        SDL_RenderCopy(ctrl->rend, ctrl->images.bg_fixed, NULL,
            &ctrl->bg_rect);
    }
}

/* do_bg: sets up the offsets required for drawing background scenery tiles,
 * based on the current input readings */
static void do_bg (ctrl_t *ctrl, double map_pixels)
{
    double pixels;

    if ((pixels = BG_X_ACCEL(ctrl)) > map_pixels)
        pixels = map_pixels;

    if (ctrl->input.right && ctrl->pos < ctrl->map.max_p) {
        /* Handle positioning of BG scenery between tile boundaries */
        if ((ctrl->bgoffset - pixels) < 0) {

            ctrl->bgoffset = BG_TILE_SIZE + (ctrl->bgoffset - pixels);
            ctrl->bgpos = fmod(ctrl->bgpos + 1.0, ctrl->map.bg_max_x);
        } else {
            ctrl->bgoffset -= pixels;
        }

        ctrl->map.bg_x_accel = pixels;
    }

    if (ctrl->input.left && (ctrl->pos > 0 || ctrl->offset < 0)) {
        /* Handle positioning of BG scenery between tile boundaries */
        if (((ctrl->bgoffset + pixels) > BG_TILE_SIZE)) {
            ctrl->bgoffset = fmod(ctrl->bgoffset + pixels,
                BG_TILE_SIZE);

            if (ctrl->bgpos >= 1)
                ctrl->bgpos -= 1;
            else
                ctrl->bgpos = ctrl->map.bg_max_x - 1;

        } else {
            ctrl->bgoffset += pixels;
        }

        ctrl->map.bg_x_accel = pixels;
    }
}

/* do_sky: sets up the offsets required to draw the sky bitmap, based on
 * current input readings */
static void do_sky (ctrl_t *ctrl, double map_pixels)
{
    double pixels;

    if ((pixels = SKY_X_ACCEL(ctrl)) > map_pixels)
        pixels = map_pixels;

    if (ctrl->input.right && ctrl->pos < ctrl->map.max_p) {
        /* Handle positioning of BG scenery between tile boundaries */
        if ((ctrl->skyoffset - pixels) < 0) {

            ctrl->skyoffset = ctrl->bg_rect.w + (ctrl->skyoffset - pixels);
        } else {
            ctrl->skyoffset -= pixels;
        }
    }

    if (ctrl->input.left && (ctrl->pos > 0 || ctrl->offset < 0)) {
        /* Handle positioning of BG scenery between tile boundaries */
        if (((ctrl->skyoffset + pixels) > ctrl->bg_rect.w)) {
            ctrl->skyoffset = fmod(ctrl->skyoffset + pixels, ctrl->bg_rect.w);
        } else {
            ctrl->skyoffset += pixels;
        }
    }
}

static void draw_map_tiles (ctrl_t *ctrl)
{
    SDL_Rect tile;
    uint8_t sym;
    int x, y;

    /* Draw visible tiles from the map on the screen */
    for (y = 0; y < YTILES_HEIGHT; ++y) {
        for (x = (ctrl->pos) ? -1 : 0; x < (XTILES_WIDTH + 1); ++x) {
            /* Get this tile's symbol */
            sym = ctrl->map.data[y][ctrl->pos + x];

            if (sym > 0) {
                /* Draw a new tile here at (x,y) */
                tile = draw_tile(ctrl, sym, (x * TILE_SIZE) + ctrl->offset,
                    y * TILE_SIZE);

                /* Save in colliders array, *only* if tile is fully
                 * on-screen */
                if (x >= 0) {
                    ctrl->colliders[y][x] = tile;
                }
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
void do_map (ctrl_t *ctrl, double blend)
{
    int dist;
    double blended;

    blended = 0.0;

    /* Left keypress: scroll map to the right */
    if (ctrl->input.left &&
            !ctrl->input.right && (ctrl->pos > 0 || ctrl->offset < 0)) {
        dist = tile_distance_left(ctrl, &ctrl->player);
        ctrl->map.x_accel = X_ACCEL(ctrl);
        blended = clip_movement(ctrl->map.x_accel * blend, dist, 1.0);

        /* Handle positioning of map between tile boundaries */
        if (((ctrl->offset + blended) > TILE_SIZE)) {
            ctrl->offset = fmod(ctrl->offset + blended, TILE_SIZE);

            if (ctrl->pos >= 1)
                ctrl->pos -= 1;
        } else {
            ctrl->offset += blended;
        }

    /* Right keypress: scroll map to the left */
    } else if (ctrl->input.right &&
            !ctrl->input.left && (ctrl->pos < ctrl->map.max_p)) {
        dist = tile_distance_right(ctrl, &ctrl->player);
        ctrl->map.x_accel = X_ACCEL(ctrl);
        blended = clip_movement(ctrl->map.x_accel * blend, dist, -1.0);

        /* Handle positioning of map between tile boundaries */
        if ((ctrl->offset - blended) < 0) {
            ctrl->offset = TILE_SIZE + (ctrl->offset - blended);
            ctrl->pos += 1;
        } else {
            ctrl->offset -= blended;
        }

    } else {
        ctrl->map.x_accel = 0.0;
    }

    /* Reset on-screen collider array */
    memset(ctrl->colliders, 0,
        sizeof(ctrl->colliders[0][0]) * YTILES_HEIGHT * (XTILES_WIDTH + 1));

    do_sky(ctrl, blended);
    do_bg(ctrl, blended);
}

void draw_map (ctrl_t *ctrl)
{
    draw_bg_colour(ctrl, bg_base);
    draw_bg_bmp(ctrl);
    draw_bg_tiles(ctrl);
    draw_map_tiles(ctrl);
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
                case PLAYER_FINISH:
                    ctrl->map.data[y][x] = c;
                    ctrl->map.finish_x = x;
                    ctrl->map.finish_y = y;
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
                case FIXED_TILE:
                    ctrl->map.bg[y][x] = 1;
                break;
            }

            ++x;
        }
    }

    fclose(fp);
    return 0;
}

int next_level (ctrl_t *ctrl, game_t *game)
{
    char buf[20];

    ++ctrl->level;

    /* Set up level intro "cut scene" text and return scene */
    snprintf(buf, sizeof(buf), "Level %u", ctrl->level);
    game->return_scene = draw_scene_game_reset;
    cut_to_text(game, buf, sizeof(buf), LEVEL_BANNER_SECS);

    /* Load map & background data from files */
    if (load_map(ctrl) != 0) {
        return -1;
    }

    return 0;
}

int load_map (ctrl_t *ctrl)
{
    char filename[256];

    snprintf(filename, sizeof(filename), "maps/%d/%s", ctrl->level, MAP_FILE_NAME);
    if (map_from_file(ctrl, filename) != 0) {
        return -1;
    }

    /* Make a copy of initial map state, to use for a full reset; simple,
     * but uses more memory... */
    memcpy(ctrl->map.reset_copy, ctrl->map.data, sizeof(ctrl->map.data));

    snprintf(filename, sizeof(filename), "maps/%d/%s", ctrl->level, BG_FILE_NAME);
    if (bg_from_file(ctrl, filename) != 0) {
        return -1;
    }

    /* Maximum starting position in the map array, taking
     * screen width into account */
    ctrl->map.max_p = ctrl->map.max_x - (XTILES_WIDTH / 2) - 1;
    return 0;
}
