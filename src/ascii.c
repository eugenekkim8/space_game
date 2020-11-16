// Helper functions to print ASCII art

#include <ncurses.h>
#include <stdio.h>
#include "invaders.h"

void mv_print_file(char* file_loc, int x, int y){
	FILE *fp = fopen(file_loc, "r");
	if(fp == NULL){
		printf("Can't open %s\n", file_loc);
	}
	char c;
	int row = x;
	move(row, y); 
	while((c = getc(fp)) != EOF){
		if(c == '\n'){
			row++;
			move(row, y);
		} else{
			addch(c);
		}
	}
	fclose(fp);
}

void mv_wprint_file(WINDOW *w, char* file_loc, int x, int y){
	FILE *fp = fopen(file_loc, "r");
	if(fp == NULL){
		printf("Can't open %s\n", file_loc);
	}
	char c;
	int row = x;
	move(row, y); 
	while((c = getc(fp)) != EOF){
		if(c == '\n'){
			row++;
			wmove(w, row, y);
		} else{
			waddch(w, c);
		}
	}
	fclose(fp);
}
