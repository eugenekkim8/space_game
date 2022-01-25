#ifndef INVADERS_H
#define INVADERS_H

#include <ncurses.h>

#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

#define ROWS 16
#define COLS 72

#define FRAME_RATE 70

#define PLAYER_LIVES 3
#define BASE_LIVES 10

#define PLAYER_BULLET_SPEED 2

#define POINTS_PER_ENEMY 10
#define POINTS_PER_BIGENEMY 30
#define POINTS_PER_LEVEL 200
#define MAX_LEVEL 6

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
    int lives;
} player;

typedef struct bullet bullet;

struct bullet {
    bullet_direction direction;
    int x, y;
    int b_speed;
    bullet* next; 
};

typedef struct enemy enemy;

struct enemy {
    int x, y;
    int b_rate;
    int b_speed;
    int x_speed; 
    int y_rate;
    int period;
    char symbol;
    int birth_frame;
    enemy* next;
};

typedef struct {
    int active;
    int x, y;
    int x_rate;
    int lives;
    int height; 
    char* file_loc;
    int birth_frame;
} bigenemy;

typedef struct {
    int rows, cols;
    int score;
    player ship;
    bullet* root_b;
    enemy* root_e;
    bigenemy boss;  
    int n_frame;
    int base_lives;
    int level;
} game;

extern int ENEMY_GEN_RATE[MAX_LEVEL];     
extern int ENEMY_BULLET_SPEED[MAX_LEVEL];
extern int ENEMY_BULLET_RATE[MAX_LEVEL];
extern int ENEMY_X_SPEED[MAX_LEVEL];
extern int ENEMY_Y_RATE[MAX_LEVEL];
extern int ENEMY_PERIOD[MAX_LEVEL];
extern char ENEMY_SYMBOL[MAX_LEVEL];

// -1 because boss occurs during level transitions
extern int BOSS_X_RATE[MAX_LEVEL-1];
extern int BOSS_LIVES[MAX_LEVEL-1];
extern int BOSS_HEIGHT[MAX_LEVEL-1];
extern char *BOSS_FILE_LOC[MAX_LEVEL-1];

void move_player(game *g, player_move move);
void deactivate_all_bullets(bullet *root);
void deactivate_all_enemies(enemy *root);
void move_bullets(game *g);
void move_enemies(game *g);
void create_bullet(game *g, bullet_direction dir, int x, int y, int speed);
void create_enemy(game *g, int x, int y, int b_rate, int b_speed, int x_speed, int y_rate, int period, char symbol, int birth_frame);
void generate_enemy_bullets(game *g);
void check_collisions(game *g);

void mv_print_file(char* file_loc, int y, int x);
void mv_wprint_file(WINDOW *w, char* file_loc, int y, int x);

void create_boss(game *g);
void move_boss(game *g);

#endif
