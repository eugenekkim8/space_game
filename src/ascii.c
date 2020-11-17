// Helper functions to print ASCII art

#include <ncurses.h>
#include <stdio.h>
#include "invaders.h"

void mv_print_file(char* file_loc, int y, int x){
	FILE *fp = fopen(file_loc, "r");
	if(fp == NULL){
		printf("Can't open %s\n", file_loc);
	}
	char c;
	int row = y;
	move(row, x); 
	while((c = getc(fp)) != EOF){
		if(c == '\n'){
			row++;
			move(row, x);
		} else{
			addch(c);
		}
	}
	fclose(fp);
}

// cuts off strings to prevent wrapping
void mv_wprint_file(WINDOW *w, char* file_loc, int y, int x){
	FILE *fp = fopen(file_loc, "r");
	if(fp == NULL){
		printf("Can't open %s\n", file_loc);
	}
	char c;
	int row = y;
	int col = x; 
	wmove(w, row, col); 
	while((c = getc(fp)) != EOF){
		if(c == '\n'){
			row++;
			col = x;
		} else if (col <= COLS - 2){
			mvwaddch(w, row, col, c);
			col++;
		}
	}
	fclose(fp);
}
