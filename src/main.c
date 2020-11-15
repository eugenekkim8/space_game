#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "invaders.h"
#include "util.h"

int ENEMY_GEN_RATE[MAX_LEVEL]		= 	{ 30, 20, 30, 15, 20, 20};
int ENEMY_BULLET_SPEED[MAX_LEVEL]	= 	{ 2 , 2 , 2 , 4 , 2 , 3 };
int ENEMY_BULLET_RATE[MAX_LEVEL]	= 	{ 72, 36, 72, 72, 24, 18};
int ENEMY_X_SPEED[MAX_LEVEL]		= 	{ 1 , 1 , 1 , 2 , 1 , 1 };
int ENEMY_Y_RATE[MAX_LEVEL]			= 	{ 20, 20, 20, 20, 20, 10};
int ENEMY_PERIOD[MAX_LEVEL]			= 	{ 0 , 0 , 2 , 0 , 0 , 3 };
char ENEMY_SYMBOL[MAX_LEVEL]		=	{'K','#','X','G','M','*'};

void ncurses_init(){
	initscr();
	cbreak();				// pass key presses, not signals
	noecho();
	keypad(stdscr, TRUE);	// allow arrow keys
	timeout(0);				// no blocking on getch()
	curs_set(0);			// invisible cursor
}

player *player_init(){
	player *obj = malloc(sizeof(player));
	obj->x = 3;
	obj->y = ROWS/2 - 1;
	obj->lives = PLAYER_LIVES;
	return obj;
}

game *game_init(){
	game *obj = malloc(sizeof(game));
	obj->rows = ROWS;
	obj->cols = COLS;
	obj->ship = *player_init();
	obj->score = 0;
	obj->n_frame = 0;
	obj->level = 1;
	obj->base_lives = BASE_LIVES;
	for (int i = 0; i < MAX_BULLETS; i++){
		obj->bullets[i].active = 0;
	}
	for (int j = 0; j < MAX_ENEMIES; j++){
		obj->enemies[j].active = 0;
	}
	return obj;
}

void display_board(WINDOW *w, game *g){
	wclear(w);
	box(w, 0, 0);
	wmove(w, ROWS/2 - 2, 0);
	wprintw(w, "B");
	wmove(w, ROWS/2 - 1, 0);
	wprintw(w, "A");
	wmove(w, ROWS/2, 0);
	wprintw(w, "S");
	wmove(w, ROWS/2 + 1, 0);
	wprintw(w, "E");

	// display player
	player ship = g->ship;
	wmove(w, ship.y, ship.x);
	wprintw(w, PLAYER_CHAR);

	// display bullets
	for(int i = 0; i < MAX_BULLETS; i++){
		if(g->bullets[i].active == 0){
			continue;
		}
		wmove(w, g->bullets[i].y, g->bullets[i].x);
		switch(g->bullets[i].direction){
			case B_LEFT:
				wprintw(w, LEFT_BULLET);
				break;
			case B_RIGHT:
				wprintw(w, RIGHT_BULLET);
				break;
		}
	}

	// display enemies
	for(int i = 0; i < MAX_ENEMIES; i++){
		if(g->enemies[i].active == 0){
			continue;
		}
		wmove(w, g->enemies[i].y, g->enemies[i].x);
		wprintw(w, &g->enemies[i].symbol);
	}

	wnoutrefresh(w);
}

void display_score(WINDOW *w, game *g){
	wclear(w);
	wprintw(w, " Score: %d", g->score);
	wmove(w, 1, 0);
	wprintw(w, " Level: %d", g->level);
	wmove(w, 0, 18);
	wprintw(w, " Ship Lives: %d", g->ship.lives);
	wmove(w, 1, 18);
	wprintw(w, " Base Lives: %d", g->base_lives);
	wnoutrefresh(w);
}

int main(){
	game *g;
	WINDOW *board, *score;
	player_move move = NONE;
	bool running = true;

	srand(time(NULL));

	ncurses_init();
	g = game_init();

	board = newwin(g->rows, g->cols, 0, 0);
	score = newwin(4, 36, g->rows + 1, 0);

	while(running){
		display_board(board, g);
		display_score(score, g);

		//is game over?
		if(g->ship.lives <= 0 || g->base_lives <= 0){
			running = false;
		}

		doupdate();
		sleep_milli(FRAME_RATE);

		// advance bullets 
		move_bullets(g);

		// calculate collisions 
		check_collisions(g);

		// advance enemies 
		move_enemies(g);

		// generate enemies
		int this_level = g->level;
		if(g->n_frame % ENEMY_GEN_RATE[this_level-1] == 0){
			int enemy_y = rand() % (ROWS - 2) + 1;
			create_enemy(g, COLS-2, enemy_y, ENEMY_BULLET_RATE[this_level-1], 
				ENEMY_BULLET_SPEED[this_level-1], ENEMY_X_SPEED[this_level-1], 
				ENEMY_Y_RATE[this_level-1], ENEMY_PERIOD[this_level-1], 
				ENEMY_SYMBOL[this_level-1], g->n_frame);
		}

		// generate bullets
		generate_enemy_bullets(g);
		
		// see how player wants to move 
		switch(getch()){
			case KEY_UP:
				move = UP;
				break;
			case KEY_DOWN:
				move = DOWN;
				break;
			case KEY_LEFT:
				move = LEFT;
				break;
			case KEY_RIGHT:
				move = RIGHT;
				break;
			case 'q':
				running = false;
				break;
			case ' ':
				create_bullet(g, B_RIGHT, g->ship.x + 1, g->ship.y, PLAYER_BULLET_SPEED);
				break;
			default:
				move = NONE;
		}

		move_player(g, move);

		// advance level?
		if(g->score > g->level * POINTS_PER_LEVEL){
			g->level = MIN(g->level + 1, MAX_LEVEL);
		}

		g->n_frame++;

	}

	// show game over scren:
	wclear(board);
	wmove(board, 0, 0);
	wprintw(board, "Game over.");
	wnoutrefresh(board);
	doupdate();
	sleep(1);


	wclear(stdscr);
	endwin();
}
