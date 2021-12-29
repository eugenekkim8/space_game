#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "invaders.h"
#include "util.h"

                            //level #:       1,  2,  3,  4,  5,  6 
int ENEMY_GEN_RATE[MAX_LEVEL]        =     { 30, 20, 30, 20, 20, 20};
int ENEMY_BULLET_SPEED[MAX_LEVEL]    =     { 2 , 2 , 2 , 4 , 2 , 4 };
int ENEMY_BULLET_RATE[MAX_LEVEL]     =     { 72, 36, 72, 72, 24, 18};
int ENEMY_X_SPEED[MAX_LEVEL]         =     { 1 , 1 , 1 , 2 , 1 , 1 };
int ENEMY_Y_RATE[MAX_LEVEL]          =     { 20, 20, 20, 20, 20, 10};
int ENEMY_PERIOD[MAX_LEVEL]          =     { 0 , 0 , 2 , 0 , 0 , 2 };
char ENEMY_SYMBOL[MAX_LEVEL]         =     {'K','#','X','G','M','*'};

void ncurses_init(){
    initscr();
    cbreak();                // pass key presses, not signals
    noecho();
    keypad(stdscr, TRUE);    // allow arrow keys
    timeout(0);                // no blocking on getch()
    curs_set(0);            // invisible cursor
}

player *player_init(){
    player *obj = malloc(sizeof(player));
    obj->x = 3;
    obj->y = ROWS/2 - 1;
    obj->lives = PLAYER_LIVES;
    return obj;
}

bigenemy *boss_init(){
    bigenemy *obj = malloc(sizeof(bigenemy));
    obj->file_loc = malloc(50);
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
    obj->boss = *boss_init();
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

    // display boss, if any
    bigenemy bs = g->boss;
    if(bs.active == 1){
        mv_wprint_file(w, bs.file_loc, bs.y, bs.x);
    }

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
    wprintw(w, " Ship Lives: (%02d) ", MAX(g->ship.lives, 0));
    for(int i = 0; i < g->ship.lives; i++)
        waddch(w, 'X');
    wmove(w, 1, 18);
    wprintw(w, " Base Lives: (%02d) ", MAX(g->base_lives, 0));
    for(int i = 0; i < g->base_lives; i++)
        waddch(w, 'X');
    // display boss lives if active
    if(g->boss.active == 1){
        wmove(w, 3, 18);
        wprintw(w, " Boss Lives: (%02d) ", g->boss.lives);
        for(int i = 0; i < floor(g->boss.lives / 10.0); i++)
            waddch(w, '*');
        for(int j = 0; j < g->boss.lives - 10 * floor(g->boss.lives / 10.0); j++)
            waddch(w, 'X');
    }
    wnoutrefresh(w);
}

void game_pause(WINDOW *w){
    wclear(w);
    wmove(w, 1, 1);
    wprintw(w, "Game paused.");
    wmove(w, 2, 1);
    wprintw(w, "Press any key to resume.");
    wnoutrefresh(w);
    doupdate();
    timeout(-1);
    if(getch()){
        timeout(0);
    } 
}

void intro_screen(){
    // "Space Game" logo
    mv_print_file("assets/intro.txt", 1, 2);                           
    timeout(-1);
    if(getch()){
        timeout(0);
        clear();
        refresh();
    }
}

int main(){
    game *g;
    WINDOW *board, *score;
    player_move move = NONE;
    bool running = true;

    srand(time(NULL));

    ncurses_init();
    intro_screen();

    g = game_init();

    board = newwin(g->rows, g->cols, 0, 0);
    score = newwin(4, g->cols, g->rows + 1, 0);

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

        // advance boss, if active
        if(g->boss.active == 1){
            move_boss(g);
        }

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
            case 'p':
                game_pause(board);
                break;
            case ' ':
                create_bullet(g, B_RIGHT, g->ship.x + 1, g->ship.y, PLAYER_BULLET_SPEED);
                break;
            case 'n': // cheat code, for testing!
                g->level++;
                g->score+=POINTS_PER_LEVEL; 
                break;
            default:
                move = NONE;
        }

        move_player(g, move);

        // advance level?
        if(g->score > g->level * POINTS_PER_LEVEL && g->level < MAX_LEVEL){
            create_boss(g);
            g->level++;
        }

        g->n_frame++;

        // display game over screen
        if(!running){
            wmove(board, 1, 1);
            wprintw(board, "Game over.");
            wmove(board, 2, 1);
            wprintw(board, "Press q to quit, or p to play again.");
            wnoutrefresh(board);
            doupdate();
            timeout(-1);
            while(1){
                char x = getch();
                if(x == 'p'){
                    g = game_init();
                    timeout(0);
                    running = true;
                    break;
                } else if(x == 'q'){
                    break;
                }
            } 
        }
    }

    wclear(stdscr);
    endwin();
}
