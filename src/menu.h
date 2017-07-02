#ifndef MENU_H_
#define MENU_H_

#define OPTION_MAX_LENGTH 50
#define MAX_OPTIONS 32
#define MAX_OPTION_VALUES 32

typedef struct option option_t;
typedef struct menu menu_t;

struct option {
    char label[OPTION_MAX_LENGTH];
    char values[MAX_OPTION_VALUES][OPTION_MAX_LENGTH];
    void (*setval)(struct option*, ctrl_t*, game_t*);
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

void menu_init (menu_t *m);
int menu_add_option (menu_t *m, char *label,
    void (*setval)(option_t*, ctrl_t*, game_t*), int selected);
int menu_add_value (menu_t *m, char *value, int selected);
void menu_draw (ctrl_t *ctrl, menu_t *m, int swidth, int sheight);
void menu_set_values (menu_t *m, ctrl_t *ctrl, game_t *game);

#endif