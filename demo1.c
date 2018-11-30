#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#define FALL_1 800000
#define FALL_2 600000
#define FALL_3 400000
#define FALL_4 300000
#define FALL_5 50000


int perf, good, bad;

void draw();
void move_zombie(int fall_speed);
void setup();
int set_ticker(int n_msecs);

void main(){
	int i=0;
	int c;
	int level;

	setup();
	addstr("press 1 to start\n");
	refresh();
	while(1){
		if((c=getch())=='1')
			break;
	}
	draw();
	
	while(i<10){
		move_zombie(FALL_5);
		i++;
	}
	getch();
	endwin();
}

/*int set_ticker(int n_msecs){
	struct itimerval new_timeset;
	long n_sec, n_usecs;

	n_sec = n_msecs/1000;
	n_usecs = (n_msecs%1000)*1000L;

	new_timeset.it_interval.tv_sec = n_sec;
	new_timeset.it_interval.tv_usec = n_usecs;
	new_timeset.it_value.tv_sec = n_sec;
	new_timeset.it_value.tv_usec = n_usecs;

	return settimer(ITIMER_REAL, &new_timeset, NULL);
}*/

void draw(){
	 int i;

	 clear();
	 move(1,4);
	 addstr("press each key below");

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

void setup(){
	initscr();
	noecho();
	cbreak();
	clear();
}

/*void move2(int fall_speed){
	int ran_pla;
	int c, i;
	int perf,good,bad;

	srand((unsigned)time(NULL));
	ran_pla = rand()%4;

	switch(ran_pla){
		casw 0:
			for(i=5;i<25;i++){
				move(i,6);
				addstr("   zz   ");
				refresh();
				set_ticker(fall_speed);
				move(i,6);
				*/
				

void move_zombie(int fall_speed){
	int ran_pla;
	int c;
	int i;
	//int c_perf,c_good;

	srand((unsigned)time(NULL));
	ran_pla = rand()%4;

	switch(ran_pla){
		case 0:
			for(i=5;i<25;i++){
				move(i,6);
				addstr("   zz   ");
				refresh();
				iif((c=getch()!='a')&&i==23){
					good++;
					//addstr("good");
					refresh();
				}
				else if((c=getch()!='a')&&i==24){
					perf++;
					//addstr("perf");
					refresh();
				}
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
