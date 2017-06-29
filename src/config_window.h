#ifndef CONFIG_WINDOW_H_
#define CONFIG_WINDOW_H_

#define CONFIG_WINDOW_WIDTH 1024
#define CONFIG_WINDOW_HEIGHT 576

void config_window_init (ctrl_t *ctrl, game_t *game);
int draw_config_window (ctrl_t *ctrl, game_t *game);
void close_config_window (ctrl_t *ctrl);

#endif
