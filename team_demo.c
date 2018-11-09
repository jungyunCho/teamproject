#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define FALL_1 800000
#define FALL_2 600000
#define FALL_3 400000
#define FALL_4 300000
#define FALL_5 200000
#define DONE_1 10
#define DONE_2 15
#define DONE_3 20
#define DONE_4 25
#define DONE_5 30
void draw();
void move_zombie(int fall_speed);

void main(){
	int i;
	int level;

	initscr();
	draw();
	for(i=0; i<DONE_5; i++){
		move_zombie(FALL_5);
	}
	getch();
	endwin();
}

void draw(){
	 int i;

	 clear();
	 move(1,4);
	 addstr("press each button below on right timming");

	 for(i=5;i<25;i++){
		 move(i,5);
		 addstr("|        |        |        |        |");
	 }
	 move(25,9);
	 addstr("AA");
	 move(25,18);
	 addstr("SS");
	 move(25,27);
	 addstr("DD");
	 move(25,36);
	 addstr("FF");
	 refresh();
}

void move_zombie(int fall_speed){
	int ran_pla;
	int i;

	srand((unsigned)time(NULL));
	ran_pla = rand()%4;

	switch(ran_pla){
		case 0:
			for(i=5;i<25;i++){
				move(i,6);
				addstr("   zz   ");
				refresh();
				/*if((i==24)&&){

				}*/
				usleep(fall_speed);
				move(i,6);
				addstr("        ");

			}
			break;
		case 1:
			for(i=5;i<25;i++){
				move(i,15);
				addstr("   zz   ");
				refresh();
				usleep(fall_speed);
				move(i,15);
				addstr("        ");
			}
			break;
		case 2:
			for(i=5;i<25;i++){
				move(i,24);
				addstr("   zz   ");
				refresh();
				usleep(fall_speed);
				move(i,24);
				addstr("        ");
			}
			break;
		case 3:
			for(i=5;i<25;i++){
				move(i,33);
				addstr("   zz   ");
				refresh();
				usleep(fall_speed);
				move(i,33);
				addstr("        ");
			}
			break;
	}
}


