#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "invaders.h"
#include "util.h"

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
	// printf("%d", sizeof(player));
	obj->x = 3;
	obj->y = ROWS/2 - 1;
	return obj;
}

game *game_init(){
	game *obj = malloc(sizeof(game));
	obj->rows = ROWS;
	obj->cols = COLS;
	obj->ship = *player_init();
	obj->score = 0;
	for (int i = 0; i < MAX_BULLETS; i++){
		obj->bullets[i].active = 0;
		obj->bullets[i].direction = B_RIGHT;
	}
	return obj;
}

void display_board(WINDOW *w, game *g){
	wclear(w);
	box(w, 0, 0);
	// display player
	player ship = g->ship;
	wmove(w, ship.y, ship.x);
	wprintw(w, PLAYER_CHAR);

	// display enemies

	// display bullets
	for(int i = 0; i < MAX_BULLETS; i++){
		if(g->bullets[i].active == 0){
			continue;
		}
		wmove(w, g->bullets[i].y, g->bullets[i].x);
		switch(g->bullets[i].direction){
			case B_LEFT:
				wprintw(w, LEFT_BULLET);
			case B_RIGHT:
				wprintw(w, RIGHT_BULLET);

		}
	}

	wnoutrefresh(w);
}

void display_score(WINDOW *w, game *g){
	wclear(w);
	wprintw(w, " Score: %d", g->score);
	wnoutrefresh(w);
}

int main(){
	game *g;
	WINDOW *board, *score;
	player_move move = NONE;
	bool running = true;

	ncurses_init();
	g = game_init();

	board = newwin(g->rows, g->cols, 0, 0);
	score = newwin(3, 24, g->rows + 1, 0);

	while(running){
		display_board(board, g);
		display_score(score, g);
		doupdate();
		sleep_milli(FRAME_RATE);

		// advance bullets [2]
		move_bullets(g);

		// calculate collisions
		// advance enemies [3]
		// generate enemies/bullets...
		
		// see how player wants to move [1]
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

	}

	wclear(stdscr);
	endwin();
}
