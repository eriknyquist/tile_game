#ifndef BITMAP_MANAGER_H_
#define BITMAP_MANAGER_H_

SDL_Texture *load_bitmap (ctrl_t *ctrl, char *filename, SDL_Rect *rect);
void destroy_bitmaps (void);

#endif
