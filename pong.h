#ifndef PONG_H_
#define PONG_H_

#include "game.h"

#define PONG_PADDLE_WIDTH 10
#define PONG_PADDLE_HEIGHT 75
#define PONG_PADDLE_DISTANCE_WALL 20
#define PONG_BALL_SIZE 10

/* because we need to pass it as a pointer to handle_event... */
struct pong_t {
    struct Game game;
    struct paddle {
        uint16_t x, y, score;
    } player_paddle, enemy_paddle;
    struct {
        float x, y, direction, speed;
        enum {
        NORMAL,
        X_EDGE_HIT,
        Y_EDGE_HIT,
        PLAYER_PADDLE_HIT,
        ENEMY_PADDLE_HIT,
        } state;
    } ball;
};

#endif // PONG_H_
