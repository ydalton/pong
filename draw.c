#include <assert.h>

#include "draw.h"

void blit_pixel(struct sdl_framebuffer *buf, unsigned int x, unsigned int y,
				unsigned char r, unsigned char g, unsigned int b)
{
	void *pixels = buf->pixels;
	uint32_t off = x + (y * buf->width);
	assert(off <= buf->width * buf->height * buf->bytes_per_pixel);
	uint32_t *fb_ptr = ((uint32_t *) pixels + off);
	*(fb_ptr) = b + (g << 8) + (r << 16);
}

void draw_rect(struct sdl_framebuffer *buf, unsigned int x, unsigned int y,
			   unsigned int w, unsigned int h, unsigned int r, unsigned int g,
			   unsigned int b)
{
	if(x > buf->width || y > buf->height)
		return;
	if(x+w > buf->width)
		w = buf->width - x;
	if(y+h > buf->height)
		h = buf->height - y;
	for(unsigned int i = y; i < y+h; i++) {
		for(unsigned int j = x; j < x+w; j++) {
			blit_pixel(buf, j, i, r, g, b);
		}
	}
}

void random_draw(struct sdl_framebuffer *buf)
{
	static int f = 0;
	for(unsigned int i = 0; i < buf->height; i++) {
		for(unsigned int j = 0; j < buf->width; j++) {
			blit_pixel(buf, j, i, i + ++f, 0, 0);
		}
	}
}
