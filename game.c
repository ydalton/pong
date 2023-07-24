#include <sys/mman.h>

#include "game.h"
#include "log.h"

int game_init(struct Game *g)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOG_E("Cannot initialize SDL: %s", SDL_GetError());
        return EXIT_FAILURE;
    }
    LOG_I("hi\n");
    if(!g->name)
         g->name = "";
    g->win = SDL_CreateWindow(g->name,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              g->framebuffer.width,
                              g->framebuffer.height,
                              SDL_WINDOW_RESIZABLE);
    if(!g->win) {
        LOG_E("Cannot create window: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    g->renderer = SDL_CreateRenderer(g->win, -1, 0);
    SDL_ResizeWindow(&g->framebuffer,
                     g->renderer,
                     g->framebuffer.width,
                     g->framebuffer.height);

    return EXIT_SUCCESS;
}

void game_exit(struct Game *g)
{
    SDL_DestroyWindow(g->win);
    SDL_Quit();
}

void SDL_UpdateWindow(struct sdl_framebuffer *buf, SDL_Renderer *ren)
{
    SDL_UpdateTexture(buf->texture,
                      NULL,
                      buf->pixels,
                      buf->width * buf->bytes_per_pixel);
    SDL_RenderCopy(ren, buf->texture, NULL, NULL);
    SDL_RenderPresent(ren);
}

void SDL_ResizeWindow(struct sdl_framebuffer *buf, SDL_Renderer *ren,
                             int w, int h)
{
    int bytes_per_pixel = buf->bytes_per_pixel;
    if(buf->pixels)
        munmap(buf->pixels, buf->width * buf->height * bytes_per_pixel);

    if(buf->texture)
        SDL_DestroyTexture(buf->texture);

    buf->width = w;
    buf->height = h;
    buf->texture = SDL_CreateTexture(ren,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                w,
                                h);
    buf->pixels = mmap(NULL,
                  w * h * bytes_per_pixel,
                  PROT_READ | PROT_WRITE,
                  MAP_ANONYMOUS | MAP_PRIVATE,
                  -1,
                  0);
}
