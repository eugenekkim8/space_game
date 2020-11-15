#ifndef INVADERS_H
#define INVADERS_H

#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

#define ROWS 16
#define COLS 72

#define MAX_BULLETS 100
#define MAX_ENEMIES 10

#define FRAME_RATE 50

#define ENEMY_GEN_RATE 20 // number of frames

#define PLAYER_BULLET_SPEED 2
#define ENEMY_BULLET_SPEED 2
#define ENEMY_BULLET_RATE 30
#define ENEMY_X_SPEED 1

#define PLAYER_CHAR "E"
#define ENEMY_CHAR "K"
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

typedef struct{
	int active;
	int x, y;
	int b_rate;
	int b_speed;
	int x_speed; 
	int birth_frame;
} enemy;

typedef struct {
	int rows, cols;
	int score;
	player ship;
	bullet bullets[MAX_BULLETS];
	enemy enemies[MAX_ENEMIES];
	int n_frame;
} game;

void move_player(game *g, player_move move);
void move_bullets(game *g);
void move_enemies(game *g);
void create_bullet(game *g, bullet_direction dir, int x, int y, int speed);
void create_enemy(game *g, int x, int y, int b_rate, int b_speed, int x_speed, int birth_frame);
void generate_enemy_bullets(game *g);


#endif
