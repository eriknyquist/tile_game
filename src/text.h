#ifndef TEXT_H_
#define TEXT_H_

void text_init (ctrl_t *ctrl);
void text_quit (ctrl_t *ctrl);
int text_draw (ctrl_t *ctrl, char *s, int x, int y);
int text_texture_width (char *s);
int text_texture_height (char *s);

#endif
