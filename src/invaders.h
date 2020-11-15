#ifndef INVADERS_H
#define INVADERS_H

#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

#define ROWS 16
#define COLS 72

#define MAX_BULLETS 100

#define PLAYER_BULLET_SPEED 1

#define FRAME_RATE 20

#define PLAYER_CHAR "E"
#define LEFT_BULLET "<"
#define RIGHT_BULLET ">"

typedef enum {
	B_LEFT, B_RIGHT
} bullet_direction; 

typedef enum {
	UP, DOWN, LEFT, RIGHT, NONE
} player_move;

typedef struct {
	int x, y;
} player;

typedef struct {
	bullet_direction direction;
	int active; 	// 0 = inactive, 1 = active
	int x, y;
	int b_speed;
} bullet;

typedef struct {
	int rows, cols;
	int score;
	player ship;
	bullet bullets[MAX_BULLETS];
} game;

void move_player(game *g, player_move move);
void create_bullet(game *g, bullet_direction dir, int x, int y, int speed);
void move_bullets(game *g);

#endif
