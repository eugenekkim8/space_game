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

void move_bullets(game *g){
	for(int i = 0; i < MAX_BULLETS; i++){
		if(g->bullets[i].active == 1){
			bullet *current = &g->bullets[i];
			switch(current->direction){
				case B_LEFT:
					current->x--;
					break;
				case B_RIGHT:
					current->x++;
					break;
			}
			if(current->x < 1 || current->x > COLS-2){
				current->active = 0;
			}
		}
	}
}


void create_bullet(game *g, bullet_direction dir, int x, int y, int speed){
	for(int i = 0; i < MAX_BULLETS; i++){
		if(g->bullets[i].active == 0){
			bullet *new_bullet = &g->bullets[i];

			new_bullet->active = 1;
			new_bullet->direction = dir;
			new_bullet->x = x;
			new_bullet->y = y;
			new_bullet->b_speed = speed;
			return;
		}
	}
	
	//TODO: is there a better system for this
	
}
