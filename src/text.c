#include "defs.h"
#include "utils.h"
#include "colours.h"

#define ASCII_LOW 0x20
#define ASCII_HIGH 0x7e
#define CACHE_SIZE (ASCII_HIGH - ASCII_LOW)

typedef struct glyph {
    SDL_Texture *texture;
    SDL_Rect rect;
} glyph_t;

static glyph_t cache[CACHE_SIZE];
static TTF_Font *font;

static int char_to_index (char c)
{
    if (c < ASCII_LOW || c > ASCII_HIGH)
        return -1;

    return c - ASCII_LOW;
}

static void add_glyph (ctrl_t *ctrl, char c)
{
    SDL_Surface *surface;
    int i;

    if((i = char_to_index(c)) < 0)
        return;

    if ((surface = TTF_RenderGlyph_Solid(font, (uint16_t) c, white)) == NULL) {
        fprintf(stderr, "Error: %s\n", TTF_GetError());
        exit(1);
    }

    if ((cache[i].texture =
            SDL_CreateTextureFromSurface(ctrl->rend, surface)) == NULL) {
        fprintf(stderr, "Error: %s\n", TTF_GetError());
        exit(1);
    }

    SDL_QueryTexture(cache[i].texture, NULL, NULL, &cache[i].rect.w,
        &cache[i].rect.h);

    SDL_FreeSurface(surface);
}

static void draw_glyph (ctrl_t *ctrl, int i, int x, int y)
{
    cache[i].rect.x = x;
    cache[i].rect.y = y;

    SDL_RenderCopy(ctrl->rend, cache[i].texture, NULL, &cache[i].rect);
}

void text_init (ctrl_t *ctrl)
{
    unsigned char c;

    if (TTF_Init() < 0) {
        fprintf(stderr, "Error: %s\n", TTF_GetError());
        exit(1);
    }

    if ((font = TTF_OpenFont("fonts/FreeMono.ttf", 48)) == NULL) {
        fprintf(stderr, "Error: %s\n", TTF_GetError());
        exit(1);
    }


    memset(cache, 0, (sizeof(glyph_t) * CACHE_SIZE));

    for (c = ASCII_LOW; c <= ASCII_HIGH; ++c) {
        add_glyph(ctrl, c);
    }
}

void text_quit (ctrl_t *ctrl)
{
    unsigned int i;

    for (i = 0; i < CACHE_SIZE && cache[i].rect.w > 0; ++i) {
        if (cache[i].texture != NULL)
            SDL_DestroyTexture(cache[i].texture);
    }

    TTF_CloseFont(font);
}

int text_texture_height (char *s)
{
    int i, ret;

    for (ret = 0; *s; ++s) {
        if ((i = char_to_index(*s)) < 0)
            return -1;

        if (cache[i].rect.h > ret)
            ret = cache[i].rect.h;
    }

    return ret;
}

int text_texture_width (char *s)
{
    int i, ret;

    for (ret = 0; *s; ++s) {
        if ((i = char_to_index(*s)) < 0)
            return -1;

        ret += cache[i].rect.w;
    }

    return ret;
}

int text_draw (ctrl_t *ctrl, char *s, int x, int y)
{
    int i;

    for (; *s; ++s) {
        if ((i = char_to_index(*s)) < 0)
            return -1;

        draw_glyph(ctrl, i, x, y);
        x += cache[i].rect.w;
    }

    return 0;
}

