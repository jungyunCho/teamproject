#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <termios.h>
#include <ncurses.h>
#include <fcntl.h>

void draw();

void main(){
	char c;
	int count,i;
	char cnt[100];

	initscr();
	//tty_mode(0);
//	set_cr_noecho_mode();
	noecho();
	cbreak();
	nodelay(stdscr,TRUE);
	//set_nodelay();

	while((c=getch())!='q'){
		move(i,5);
		addstr("press any letter");
		refresh();
		sleep(1);
		if(c=='a'){
			count++;
		}
		i++;
	}
	sprintf(cnt,"%d",count);
	move(i+1,5);
	addstr(cnt);
	refresh();
	sleep(5);
	tty_mode(1);
	endwin();
}
void draw(){
	int i;
	clear();
	move(1,4);
	addstr("Press each button below");
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
