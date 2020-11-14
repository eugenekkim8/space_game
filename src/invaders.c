#include <stdio.h>
#include "invaders.h"

void move_player(game *g, player_move move){
	player *ship = &g->ship;
	switch(move){
		case LEFT:
			ship->x--;
			break;
		case RIGHT:
			ship->x++;
			break;
		case UP:
			ship->y--;
			break;
		case DOWN:
			ship->y++;
			break;
		default:
			break;
	}

	// keep ship within bounds
	ship->x = MIN(MAX(1, ship->x), COLS-2);
	ship->y = MIN(MAX(1, ship->y), ROWS-2);
}
