#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "invaders.h"

int BOSS_X_RATE[MAX_LEVEL-1] 		= {2, 2, 3, 3, 4 };
int BOSS_LIVES[MAX_LEVEL-1]			= {12,20,30,40,50};
int BOSS_HEIGHT[MAX_LEVEL-1]		= {3, 3, 5, 4, 14};
char *BOSS_FILE_LOC[MAX_LEVEL-1]	= {"assets/small_spaceship.txt","assets/small_ufo.txt","assets/med_ufo.txt",
"assets/large_ufo.txt","assets/large_spaceship.txt"};

void create_boss(game *g){
	bigenemy *new_boss = &g->boss;
	int this_idx = g->level - 1;

	// is old boss still around? if so, return
	if(new_boss->active == 1){
		return;
	}

	new_boss->active = 1;
	new_boss->x_rate = BOSS_X_RATE[this_idx];
	new_boss->lives = BOSS_LIVES[this_idx];
	new_boss->height = BOSS_HEIGHT[this_idx];
	new_boss->file_loc = BOSS_FILE_LOC[this_idx];
	new_boss->x = COLS - 2;
	new_boss->y = floor((ROWS - new_boss->height) / 2.0);
	new_boss->birth_frame = g->n_frame;
}

void move_boss(game *g){
	if((g->n_frame - g->boss.birth_frame) % g->boss.x_rate == 0){
		g->boss.x--;
	}

	// has boss gotten to the base
	if(g->boss.x < 1){
		g->boss.active = 0;
		
		// damage to base lives = boss's HP
		g->base_lives -= g->boss.lives;
	}
}
