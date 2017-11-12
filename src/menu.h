#ifndef MENU_H_
#define MENU_H_

#define OPTION_MAX_LENGTH 50
#define MAX_OPTIONS 32
#define MAX_OPTION_VALUES 32

typedef struct option option_t;
typedef struct menu menu_t;
typedef struct value value_t;

struct value {
    char label[OPTION_MAX_LENGTH];
    int id;
};

struct option {
    char label[OPTION_MAX_LENGTH];
    value_t values[MAX_OPTION_VALUES];
    void (*setval)(int, ctrl_t*, game_t*);
    int x;
    int y;
    uint8_t vi;
    uint8_t max;
};

struct menu {
    option_t options[MAX_OPTIONS];
    uint8_t max;
    uint8_t li;
};

/* Menu-building functions
 *
 * Use these functions to initialise a new menu and add new
 * options & values to it */

/* Initialise new empty menu */
void menu_init (menu_t *m);

/* Add a new top-level option to the menu */
int menu_add_option (menu_t *m, char *label,
    void (*setval)(int id, ctrl_t*, game_t*), int selected);

/* Add a new value to the most recently added option*/
int menu_add_value (menu_t *m, char *label, int id, int selected);


/* Menu management functions
 *
 * Use these functions to draw the current state of the menu and invoke
 * option-specific callback functions */

/* Draw current state of menu */
void menu_draw (ctrl_t *ctrl, menu_t *m, int swidth, int sheight);

/* Call all the callbacks registered with menu_add_option */
void menu_set_values (menu_t *m, ctrl_t *ctrl, game_t *game);


/* Menu cursor movement
 *
 * Use these functions to move the on-screen menu cursor and
 * cycle through values for the currently selected option */

void menu_cursor_up (menu_t *menu);
void menu_cursor_down (menu_t *menu);
void menu_current_up (menu_t *menu);
void menu_current_down (menu_t *menu);

#endif
