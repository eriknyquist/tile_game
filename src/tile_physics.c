#include "defs.h"
#include "map.h"
#include "tile_collisions.h"
#include "utils.h"


/* Adds gravity to moveable object 'obj' by modifying
 * the object's yvelocity */
void add_gravity(ctrl_t *ctrl, moveable_t *obj)
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

float clip_movement (float movement, int distance, float backup)
{
    float ret;

    ret = movement;

    if (distance > 0 && (int)movement >= distance) {
        ret = (float)(distance - 1);
    } else if (distance == 0) {
        ret = backup;
    }

    return ret;
}

/* Performs collision detection and correction for tiles above a
 * moveable object */
void tile_collisions_top (ctrl_t *ctrl, moveable_t *obj)
{
    int udist;

    udist = tile_distance_up(ctrl, obj);

    if (udist > 0 && -(obj->yvelocity) >= udist) {
        obj->yvelocity = -(udist - 1);
    } else if (udist == 0) {
        obj->yvelocity = 1;
    }
}

/* Performs collision detection and correction for tiles below a
 * moveable object */
void tile_collisions_bottom (ctrl_t *ctrl, moveable_t *obj)
{
    int ddist;
    float mvmt;

    ddist = tile_distance_down(ctrl, obj);
    if (ddist >= 0) {
        add_gravity(ctrl, obj);

        if ((mvmt = clip_movement(obj->yvelocity, ddist, -1.0)) ==
                obj->yvelocity) {
            obj->grounded = 0;
        } else {
            obj->yvelocity = mvmt;
            obj->grounded = 1;
        }
    } else if (ddist == BELOW_MAP) {
        reset_map(ctrl);
    }
}
