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

bullet *deactivate_bullet(bullet *prev, bullet *current){

    // returns current->next

    bullet* temp = current;
    prev->next = current->next;
    current = current->next;
    free(temp);
    return current;
}

void deactivate_all_bullets(bullet *root){
    bullet* prev = root;
    bullet* current = root->next;
    while(current != 0){
        current = deactivate_bullet(prev, current);
    }
}

enemy *deactivate_enemy(enemy *prev, enemy *current){

    // returns current->next

    enemy* temp = current;
    prev->next = current->next;
    current = current->next;
    free(temp);
    return current;
}

void deactivate_all_enemies(enemy *root){
    enemy* prev = root;
    enemy* current = root->next;
    while(current != 0){
        current = deactivate_enemy(prev, current);
    }
}

void move_bullets(game *g){
    bullet* prev = g->root_b;
    bullet* current = g->root_b->next;
    while(current != 0){
        switch(current->direction){
            case B_LEFT:
                current->x -= current->b_speed;
                break;
            case B_RIGHT:
                current->x += current->b_speed;
                break;
        }
        if(current->x < 1 || current->x > COLS-2){
            current = deactivate_bullet(prev, current);
        } else{
            prev = prev->next;
            current = current->next;
        }
    }
}

void move_enemies(game *g){
    enemy* prev = g->root_e;
    enemy* current = g->root_e->next;
    while(current != 0){
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

        current->y = MIN(MAX(1, current->y), ROWS-2);

        if(current->x < 1){ // enemy has made it to the end
            current = deactivate_enemy(prev, current);
            g->base_lives--;
        } else{
            prev = prev->next;
            current = current->next;
        }
    }
}


void create_bullet(game *g, bullet_direction dir, int x, int y, int speed){
    bullet* new_bullet = malloc(sizeof(bullet));

    new_bullet->direction = dir;
    new_bullet->x = x;
    new_bullet->y = y;
    new_bullet->b_speed = speed;
    new_bullet->next = g->root_b->next; // linked list insertion

    g->root_b->next = new_bullet;

    return;
}

void create_enemy(game *g, int x, int y, int b_rate, int b_speed, int x_speed, int y_rate, 
    int period, char symbol, int birth_frame){
    enemy *new_enemy = malloc(sizeof(enemy));
    new_enemy->x = x;
    new_enemy->y = y;
    new_enemy->b_speed = b_speed;
    new_enemy->b_rate = b_rate;
    new_enemy->x_speed = x_speed;
    new_enemy->y_rate = y_rate;
    new_enemy->period = period;
    new_enemy->symbol = symbol;
    new_enemy->birth_frame = birth_frame;
    new_enemy->next = g->root_e->next; //linked list insertion

    g->root_e->next = new_enemy;

    return;
        
}

void generate_enemy_bullets(game *g){
    enemy *current = g->root_e->next;
    while(current != 0){
        int enemy_age = g->n_frame - current->birth_frame;
        if(enemy_age % current->b_rate == 0){
            create_bullet(g, B_LEFT, current->x - 1,
                current->y, ENEMY_BULLET_SPEED[g->level-1]);
        }
        current = current->next;
    }
}

void check_collisions(game *g){
    int player_x = g->ship.x;
    int player_y = g->ship.y;

    bullet* prev = g->root_b;
    bullet* current = g->root_b->next;

    while(current != 0){
        int deactivate = 0;
        int bullet_x = current->x;
        int bullet_y = current->y;

        // collision with enemy?
        enemy* prev_enemy = g->root_e;
        enemy* this_enemy = g->root_e->next;
        if(current->direction == B_RIGHT){ // no friendly fire
            while(this_enemy != 0){
                int enemy_x = this_enemy->x;
                int enemy_y = this_enemy->y;
                if ((enemy_y == bullet_y) && (abs(enemy_x - bullet_x) <= 
                    (current->b_speed + this_enemy->x_speed) / 2.0)){
                    
                    // deactivate bullet and enemy
                    this_enemy = deactivate_enemy(prev_enemy, this_enemy);
                    deactivate = 1;

                    // increment score
                    g->score += POINTS_PER_ENEMY;
                } else{
                    this_enemy = this_enemy->next;
                    prev_enemy = prev_enemy->next;
                }
            }
        }

        // collision with boss?
        if(current->direction == B_RIGHT && g->boss.active == 1){
            if((abs(g->boss.x - bullet_x) <= current->b_speed / 2.0) &&
                ((bullet_y >= g->boss.y) && (bullet_y <= g->boss.y + g->boss.height - 1))){
                    // deactivate bullet, take one life off boss
                    deactivate = 1;
                    g->boss.lives--;

                    // if boss is dead, deactivate and increment score
                    if(g->boss.lives <= 0){
                        g->boss.active = 0;
                        g->score += POINTS_PER_BIGENEMY;
                    }
                }
        }

        // collision with player
        if(current->direction == B_LEFT){
            if ((player_y == bullet_y) && (abs(bullet_x - player_x) <= current->b_speed / 2.0)){
                    
                // deactivate bullet
                deactivate = 1;

                // decrease lives
                g->ship.lives--;
            }
        }

        if(deactivate == 1){
            current = deactivate_bullet(prev, current);
        } else{
            prev = prev->next;
            current = current->next;
        }
        
    }

    // for each enemy
    enemy* prev_e = g->root_e;
    enemy* current_e = g->root_e->next;
    while(current_e != 0){
        int enemy_x = current_e->x;
        int enemy_y = current_e->y;
        if ((enemy_y == player_y) && (abs(enemy_x - player_x) <= 
            (current_e->x_speed) / 2.0)){
            
            // deactivate enemy
            current_e = deactivate_enemy(prev_e, current_e);
            
            //decrease lives
            g->ship.lives--;
        } else{
            prev_e = prev_e->next;
            current_e = current_e->next;
        }
    }

    // for the boss
    if(g->boss.active == 1){
        if((abs(g->boss.x - player_x) <= 1) &&
            ((player_y >= g->boss.y) && (player_y <= g->boss.y + g->boss.height - 1))){

            // deactivate enemy
            g->boss.active = 0;

            // decrease player lives by boss lives
            g->ship.lives -= g->boss.lives;
        }

    }

}
