#include "defs.h"
#include "map.h"
#include "tile_collisions.h"
#include "utils.h"


/* Adds gravity to moveable object 'obj' by modifying
 * the object's yvelocity */
void add_gravity(moveable_t *obj)
{
    if (obj->yvelocity < MAX_GRAVITY_PIXELS) {
        obj->yvelocity += GRAVITY_PIXELS;
    }
}

/* Returns the distance (in pixels) to the nearest map tile to the
 * left of the object's position on the screen */
int tile_distance_left (ctrl_t *ctrl, moveable_t *obj)
{
    return min_positive(
        tile_obstacle_left(ctrl, obj->rect.x, obj->rect.y),
        tile_obstacle_left(ctrl, obj->rect.x, obj->rect.y + obj->rect.h)
    );
}

/* Returns the distance (in pixels) to the nearest map tile to the
 * right of the object's position on the screen */
int tile_distance_right (ctrl_t *ctrl, moveable_t *obj)
{
    return min_positive(
        tile_obstacle_right(ctrl, obj->rect.x + obj->rect.w, obj->rect.y),
        tile_obstacle_right(ctrl, obj->rect.x + obj->rect.w,
            obj->rect.y + obj->rect.h)
    );
}

/* Returns the distance (in pixels) to the nearest map tile above
 * the object's position on the screen */
int tile_distance_up (ctrl_t *ctrl, moveable_t *obj)
{
    return min_positive(
        tile_obstacle_up(ctrl, obj->rect.x, obj->rect.y),
        tile_obstacle_up(ctrl, obj->rect.x + obj->rect.w, obj->rect.y)
    );
}

/* Returns the distance (in pixels) of the nearest map tile below
 * the object's position on the screen */
int tile_distance_down (ctrl_t *ctrl, moveable_t *obj)
{
    return min_positive(
        tile_obstacle_down(ctrl, obj->rect.x, obj->rect.y + obj->rect.w),
        tile_obstacle_down(ctrl, obj->rect.x + obj->rect.w,
            obj->rect.y + obj->rect.h)
    );
}

/* Performs collision detection and correction for tiles above a
 * moveable object */
void tile_collisions_top (ctrl_t *ctrl, moveable_t *obj)
{
    int udist;

    udist = tile_distance_up(ctrl, obj);
    if (udist >= 0 && -(obj->yvelocity) > udist) {
        obj->yvelocity = -(udist - 1);
    }
}

/* Performs collision detection and correction for tiles below a
 * moveable object */
void tile_collisions_bottom (ctrl_t *ctrl, moveable_t *obj)
{
    int ddist;

    ddist = tile_distance_down(ctrl, obj);
    if (ddist > 1) {
        add_gravity(obj);

        if (obj->yvelocity > (ddist - 1)) {
            /* Ensure we don't move object inside, or past the tile */
            obj->yvelocity = ddist - 1;
            obj->grounded = 1;
        } else {
            obj->grounded = 0;
        }
    } else if (ddist == 1 && obj->yvelocity > 0) {
        obj->yvelocity = 0;
        obj->grounded = 1;
    } else if (ddist == 0) {
        obj->rect.y -= 1;
        obj->yvelocity = 0;
        obj->grounded = 1;
    } else if (ddist == BELOW_MAP) {
        reset_map(ctrl);
    }
}
