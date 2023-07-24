#ifndef SDLTEST_H_
#define SDLTEST_H_

#include <SDL2/SDL.h>

#define WHITE 255, 255, 255
#define BLACK 0, 0, 0
#define RED 255, 0, 0

#define W_WIDTH 640
#define W_HEIGHT 480

struct sdl_framebuffer{
	void *pixels;
	SDL_Texture *texture;
	uint16_t width;
	uint16_t height;
	uint8_t bytes_per_pixel;
};

void blit_pixel(struct sdl_framebuffer *buf, unsigned int x, unsigned int y,
				unsigned char r, unsigned char g, unsigned int b);

void draw_rect(struct sdl_framebuffer *buf, unsigned int x, unsigned int y,
			   unsigned int w, unsigned int h, unsigned int r, unsigned int g,
			   unsigned int b);

void random_draw(struct sdl_framebuffer *buf);

#endif // SDLTEST_H_
