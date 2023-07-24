#ifndef GAME_H_
#define GAME_H_

#include <SDL2/SDL.h>
#include "draw.h"

struct Game {
    SDL_Window *win;
    SDL_Renderer *renderer;
    struct sdl_framebuffer framebuffer;
    char *name;
};

int game_init(struct Game *g);
void game_exit(struct Game *g);
void SDL_ResizeWindow(struct sdl_framebuffer *buf, SDL_Renderer *ren, int w,
                      int h);
void SDL_UpdateWindow(struct sdl_framebuffer *buf, SDL_Renderer *ren);

#endif // GAME_H_
