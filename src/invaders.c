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
					current->x -= current->b_speed;
					break;
				case B_RIGHT:
					current->x += current->b_speed;
					break;
			}
			if(current->x < 1 || current->x > COLS-2){
				current->active = 0;
			}
		}
	}
}

void move_enemies(game *g){
	for(int i = 0; i < MAX_ENEMIES; i++){
		if(g->enemies[i].active == 1){
			enemy *current = &g->enemies[i];
			current->x -= current->x_speed;
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
}

void create_enemy(game *g, int x, int y, int b_rate, int b_speed, int x_speed, int birth_frame){
	for(int i = 0; i < MAX_ENEMIES; i++){
		if(g->enemies[i].active == 0){
			enemy *new_enemy = &g->enemies[i];

			new_enemy->active = 1;
			new_enemy->x = x;
			new_enemy->y = y;
			new_enemy->b_speed = b_speed;
			new_enemy->b_rate = b_rate;
			new_enemy->x_speed = x_speed;
			new_enemy->birth_frame = birth_frame;
			return;
		}
	}
}

void generate_enemy_bullets(game *g){
	for(int i = 0; i < MAX_ENEMIES; i++){
		if(g->enemies[i].active == 1){
			enemy *this_enemy = &g->enemies[i];
			int enemy_age = g->n_frame - this_enemy->birth_frame;
			if(enemy_age % this_enemy->b_rate == 0){
				create_bullet(g, B_LEFT, this_enemy->x - 1,
					this_enemy->y, ENEMY_BULLET_SPEED);
			}
		}
	}
}

