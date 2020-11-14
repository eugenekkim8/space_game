#ifndef INVADERS_H
#define INVADERS_H

#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

#define ROWS 16
#define COLS 72

#define PLAYER_CHAR "E"

typedef enum {
	UP, DOWN, LEFT, RIGHT, NONE
} player_move;

typedef struct {
	int x, y;
} player;

typedef struct {
	int rows, cols;
	int score;
	player ship;
} game;

void move_player(game *g, player_move move);

#endif
