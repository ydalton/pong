/*
 * Copyright (c) 2023 ydalton
 */

#include <unistd.h>

#include "log.h"
#include "pong.h"
#include "config.h"

#if defined(BEEP) && defined(__linux__)
# include "beep.h"
# include <errno.h>
# include <fcntl.h>
# include <string.h>
#else
# undef BEEP
#endif

#define FPS 60
#define TICK_INTERVAL (1000 / FPS)

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

extern int errno;

static int handle_event(struct pong_t *p, SDL_Event *e)
{
	uint8_t should_quit = 0;
	switch(e->type) {
		case SDL_WINDOWEVENT: {
			SDL_Window *win = SDL_GetWindowFromID(e->window.windowID);
			SDL_Renderer *ren = SDL_GetRenderer(win);
			struct sdl_framebuffer *buf = &p->game.framebuffer;
			switch(e->window.event) {
				case SDL_WINDOWEVENT_SIZE_CHANGED:
				{
					/* cuz then i can keep 80 columns per line ;) */
					uint16_t current_width = e->window.data1;
					SDL_ResizeWindow(buf, ren, current_width, e->window.data2);
					/* set the enemy paddle to match the window edge */
					p->enemy_paddle.x = current_width - PONG_PADDLE_DISTANCE_WALL;
					p->enemy_paddle.y = e->window.data2/2;
				} break;
				case SDL_WINDOWEVENT_EXPOSED:
				{
					SDL_UpdateWindow(buf, ren);
				} break;
			}
			break;
		}
		case SDL_QUIT:
		{
			LOG_I("bye\n");
			should_quit = 1;
		} break;
	}
	return should_quit;
}

void draw_pong(struct pong_t *pong)
{
	draw_rect(&pong->game.framebuffer, 0, 0,
			  pong->game.framebuffer.width,
			  pong->game.framebuffer.height, BLACK);

	/* draw the dotted lines in the middle */
	uint8_t i = 0;
	while(i*20 < pong->game.framebuffer.width) {
		draw_rect(&pong->game.framebuffer,
				  pong->game.framebuffer.width/2,
				  i*20,
				  3,
				  15,
				  WHITE);
		i++;
	}
	/* draw the player paddle */
	draw_rect(&pong->game.framebuffer,
			  pong->player_paddle.x,
			  pong->player_paddle.y,
			  /* - PONG_PADDLE_HEIGHT/2, */
			  PONG_PADDLE_WIDTH,
			  PONG_PADDLE_HEIGHT,
			  WHITE);
	/* draw the enemy paddle */
	draw_rect(&pong->game.framebuffer,
			  pong->enemy_paddle.x,
			  pong->enemy_paddle.y,
			  PONG_PADDLE_WIDTH,
			  PONG_PADDLE_HEIGHT,
			  WHITE);
	/* draw the ball */
	draw_rect(&pong->game.framebuffer,
			  pong->ball.x,
			  pong->ball.y,
			  PONG_BALL_SIZE,
			  PONG_BALL_SIZE,
			  RED);
}

int main(void)
{
	if(geteuid() == 0) {
		LOG_E("Please don't run this as root...\n");
		return -1;
	}
#ifdef BEEP
	/* probing the speaker if it works */
	int fd = open(BEEP_FD, O_WRONLY);
	if(fd == -1) {
		LOG_E("Cannot open PC speaker: %s\n", strerror(errno));
		LOG_E("Quitting...\n");
		return -1;
	}
	close(fd);
#endif


	unsigned char running;
	uint16_t ticks;

	/* pong variables */
	struct pong_t pong = {
		.game = {
			.name = "Pong",
			.framebuffer = {
				.bytes_per_pixel = 4,
				.width = W_WIDTH,
				.height = W_HEIGHT
			}
		},
		.player_paddle = {
			.x = PONG_PADDLE_DISTANCE_WALL,
			.y = W_HEIGHT/2,
			.score = 0
		},
		.enemy_paddle = {
			.x = W_WIDTH - PONG_PADDLE_DISTANCE_WALL,
			.y = W_HEIGHT/2,
			.score = 0
		},
		.ball = {
			.x = W_WIDTH/2,
			.y = W_HEIGHT/2,
			.direction = 0.8,
			.speed = 10.0,
			.state = NORMAL
		},
	};


	game_init(&pong.game);

	running = 1;
	ticks = 0;
	struct Game *game = &pong.game;
	while(running) {
		SDL_Event ev;
		int16_t mouse_y;

		SDL_PollEvent(&ev);
		if(handle_event(&pong, &ev))
			break;
		SDL_GetMouseState(NULL, (int *) &mouse_y);

		mouse_y -= PONG_PADDLE_HEIGHT/2;
		mouse_y = MAX(0, mouse_y);

		/* check for out of bounds */
		mouse_y = MIN(game->framebuffer.height - PONG_PADDLE_HEIGHT, mouse_y);

		pong.player_paddle.y = mouse_y;
		draw_pong(&pong);
		SDL_UpdateWindow(&game->framebuffer, game->renderer);
		/* hack */
		if(SDL_GetTicks() - ticks > TICK_INTERVAL) {
			pong.ball.x += cosf(pong.ball.direction) * pong.ball.speed;
			pong.ball.y -= sinf(pong.ball.direction) * pong.ball.speed;
			pong.enemy_paddle.y = pong.ball.y - (float)PONG_PADDLE_HEIGHT/2;
			/* ball out of bounds */
			if(pong.ball.x < 0
			   || pong.ball.x > game->framebuffer.width - PONG_BALL_SIZE) {
				pong.ball.state = X_EDGE_HIT;
			}
			/* collision on side bounce */
			if(pong.ball.y < 0
			   || pong.ball.y > game->framebuffer.height - PONG_BALL_SIZE) {
				pong.ball.state = Y_EDGE_HIT;
			}

			/* player paddle hit */
			if(pong.ball.x < pong.player_paddle.x + PONG_PADDLE_WIDTH
			   && pong.ball.y >= pong.player_paddle.y
			   && pong.ball.y <= pong.player_paddle.y + PONG_PADDLE_HEIGHT) {
				pong.ball.state = PLAYER_PADDLE_HIT;
			}

			/* enemy paddle hit */
			if(pong.ball.x > pong.enemy_paddle.x - PONG_PADDLE_WIDTH
			   && pong.ball.y >= pong.enemy_paddle.y
			   && pong.ball.y <= pong.enemy_paddle.y + PONG_PADDLE_HEIGHT) {
				pong.ball.state = PLAYER_PADDLE_HIT;
			}

			switch(pong.ball.state) {
				case PLAYER_PADDLE_HIT:
				case ENEMY_PADDLE_HIT:
					pong.ball.direction = -pong.ball.direction + M_PI;
#ifdef BEEP
					beep(BEEP_FREQ, BEEP_LEN);
#endif
					break;
				case X_EDGE_HIT:
					pong.ball.x = (float) game->framebuffer.width/2;
					pong.ball.y = (float) game->framebuffer.height/2;
					(pong.ball.x > game->framebuffer.width)
						? pong.player_paddle.score++
						: pong.enemy_paddle.score++;
					/* pong.ball.direction += 0.726; */
					LOG_I("score\n");
					break;
				case Y_EDGE_HIT:
#ifdef BEEP
					beep(BEEP_FREQ*2, BEEP_LEN);
#endif
					pong.ball.direction = -pong.ball.direction;
					break;
				case NORMAL:
				default:
					break;
			}
			pong.ball.state = NORMAL;
			ticks = SDL_GetTicks();
		}
	}

	game_exit(&pong.game);
	return EXIT_SUCCESS;
}
