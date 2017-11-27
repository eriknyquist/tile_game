#include "defs.h"
#include "menu.h"
#include "text.h"
#include "utils.h"
#include "colours.h"

#define OPTION_BORDER 20

void menu_init (menu_t *m)
{
    memset(m, 0, sizeof(menu_t));
}

int menu_add_option (menu_t *m, char *label,
    void (*setval)(int, ctrl_t*, game_t*), int selected)
{
    if (m->options[m->max].label[0] != 0)
        ++m->max;

    if (m->max >= (MAX_OPTIONS - 1))
        return -1;

    if (selected)
        m->li = m->max;

    memcpy(m->options[m->max].label, label, OPTION_MAX_LENGTH);
    m->options[m->max].setval = setval;
    return 0;
}

int menu_add_value (menu_t *m, char *label, int id, int selected)
{
    unsigned int vmax;

    vmax = m->options[m->max].max;
    if (vmax >= (MAX_OPTION_VALUES - 1))
        return -1;

    if (selected)
        m->options[m->max].vi = vmax;

    memcpy(m->options[m->max].values[vmax].label, label, OPTION_MAX_LENGTH);
    m->options[m->max].values[vmax].id = id;
    ++m->options[m->max].max;
    return 0;
}

static int menu_height (menu_t *m)
{
    unsigned int i, ret;

    for (i = 0, ret = 0; i <= m->max; ++i) {
        ret += text_texture_height(m->options[i].label);
    }

    return ret;
}

void menu_set_values (menu_t *m, ctrl_t *ctrl, game_t *game)
{
    unsigned int i, vi;

    for (i = 0; i <= m->max; ++i) {
        if (m->options[i].setval) {
            vi = m->options[i].vi;
            m->options[i].setval(m->options[i].values[vi].id, ctrl, game);
        }
    }
}

void menu_cursor_up (menu_t *menu)
{
    if (menu->li == 0) {
        menu->li = menu->max;
    } else {
        --menu->li;
    }
}

void menu_cursor_down (menu_t *menu)
{
    menu->li = (menu->li + 1) % (menu->max + 1);
}

void menu_current_up (menu_t *menu)
{
    unsigned int vi;

    vi = menu->options[menu->li].vi;
    menu->options[menu->li].vi = (vi + 1) % menu->options[menu->li].max;
}

void menu_current_down (menu_t *menu)
{
    int i;

    i = menu->options[menu->li].vi;
    if (i == 0) {
        menu->options[menu->li].vi = menu->options[menu->li].max - 1;
    } else {
        --menu->options[menu->li].vi;
    }
}

void menu_draw (ctrl_t *ctrl, menu_t *m, int swidth, int sheight)
{
    SDL_Rect r;
    char *ptr;
    char buf[(OPTION_MAX_LENGTH * 2) + 3];
    int i, height, width, pheight, x, y;

    height = menu_height(m);
    pheight = height + (20 * (m->max + 1));
    draw_bg_colour(ctrl, black);

    for (i = 0; i <= m->max; ++i) {
        if (m->options[i].max == 0) {
            ptr = m->options[i].label;
        } else {
            snprintf(buf, sizeof(buf), "%s: %s", m->options[i].label,
                m->options[i].values[m->options[i].vi].label);
            ptr = buf;
        }

        if ((width = text_texture_width(ptr)) < 0)
            continue;

        y = ((sheight - pheight) / 2) + ((pheight / (m->max + 1)) * i);
        x = (swidth - width) / 2;

        text_draw(ctrl, ptr, x, y);
        if (i == m->li) {
            r.x = x;
            r.y = y;
            r.w = width;
            r.h = height / (m->max + 1);
            draw_coloured_rect(ctrl, &r, map_fill);
        }
    }
}
