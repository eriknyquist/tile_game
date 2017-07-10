#include "defs.h"
#include "SDL2/SDL_image.h"

#define MAX_BMPS 128

static unsigned int num;
static SDL_Texture *bmps[MAX_BMPS];

SDL_Texture *load_bitmap (ctrl_t *ctrl, char *filename, SDL_Rect *rect)
{
    SDL_Texture *texture;
    SDL_Surface *surface;

    if (num >= (MAX_BMPS - 1)) {
        fprintf(stderr, "Error loading file %s: maximum of %d bitmaps already"
            " loaded\n", filename, MAX_BMPS);
        exit(1);
    }

    if ((surface = IMG_Load(filename)) == NULL) {
        fprintf(stderr, "Error loading file %s: %s\n", filename,
            SDL_GetError());
        exit(1);
    }

    if ((texture = SDL_CreateTextureFromSurface(ctrl->rend, surface)) == NULL) {
        SDL_FreeSurface(surface);
        fprintf(stderr, "Error converting file %s: %s\n", filename,
            SDL_GetError());
        exit(1);
    }

    if (rect != NULL)
        SDL_QueryTexture(texture, NULL, NULL, &rect->w, &rect->h);

    SDL_FreeSurface(surface);
    bmps[num++] = texture;
    return texture;
}

void destroy_bitmaps (void)
{
    unsigned int i;

    for (i = 0; i < MAX_BMPS; ++i) {
        SDL_DestroyTexture(bmps[i]);
    }
}
