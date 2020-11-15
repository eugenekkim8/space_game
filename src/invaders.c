#include <stdio.h>
#include <stdlib.h>
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
			
			// creating vertical oscillation behavior
			if(current->period != 0){
				int enemy_age = g->n_frame - current->birth_frame;
				if(enemy_age % current->y_rate == 0){ // make a vertical move
					int stage = enemy_age / current->y_rate;
					int direction = -1; // move down by default
					if(stage % (current->period * 4) < current->period * 2){
						direction = 1;
					}
				current->y -= direction;
				}
			}

			if(current->x < 1){ // enemy has made it to the end
				current->active = 0;
				g->base_lives--;
			}
			current->y = MIN(MAX(1, current->y), ROWS-2);
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

void create_enemy(game *g, int x, int y, int b_rate, int b_speed, int x_speed, int y_rate, 
	int period, char symbol, int birth_frame){
	for(int i = 0; i < MAX_ENEMIES; i++){
		if(g->enemies[i].active == 0){
			enemy *new_enemy = &g->enemies[i];

			new_enemy->active = 1;
			new_enemy->x = x;
			new_enemy->y = y;
			new_enemy->b_speed = b_speed;
			new_enemy->b_rate = b_rate;
			new_enemy->x_speed = x_speed;
			new_enemy->y_rate = y_rate;
			new_enemy->period = period;
			new_enemy->symbol = symbol;
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
					this_enemy->y, ENEMY_BULLET_SPEED[g->level-1]);
			}
		}
	}
}

void check_collisions(game *g){
	// for each bullet
	for(int i = 0; i < MAX_BULLETS; i++){
		if(g->bullets[i].active == 1){
			bullet *current = &g->bullets[i];
			int bullet_x = current->x;
			int bullet_y = current->y;

			// collision with enemy?
			if(current->direction == B_RIGHT){ // no friendly fire
				for(int i = 0; i < MAX_ENEMIES; i++){
					if(g->enemies[i].active == 1){
						int enemy_x = g->enemies[i].x;
						int enemy_y = g->enemies[i].y;
						if ((enemy_y == bullet_y) && (abs(enemy_x - bullet_x) <= 
							(current->b_speed + g->enemies[i].x_speed) / 2.0)){
							
							// deactivate bullet and enemy
							g->enemies[i].active = 0;
							current->active = 0;

							// increment score
							g->score += POINTS_PER_ENEMY;

							break; // move onto next bullet
						}
					}
				}
			}
			

			// collision with player
			int player_x = g->ship.x;
			int player_y = g->ship.y;
			
			if(current->direction == B_LEFT){
				if ((player_y == bullet_y) && (abs(bullet_x - player_x) <= current->b_speed / 2.0)){
						
					// deactivate bullet
					current->active = 0;

					// decrease lives
					g->ship.lives--;
				}
			}

		}
	}

	// for each ship
}
