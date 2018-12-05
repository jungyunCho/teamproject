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
void set_cr_noecho_mode();
void set_nodelay();
int tty_mode();

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
void set_cr_noecho_mode(){
	struct termios ttystate;
	tcgetattr(0, &ttystate);
	ttystate.c_lflag &= ~ICANON;
	ttystate.c_lflag &= ~ECHO;
	ttystate.c_cc[VMIN] =1;
	tcsetattr(0,TCSANOW, &ttystate);
}
void set_nodelay(){
	int  termflags;
	termflags = fcntl(0,F_GETFL);
	termflags |= O_NDELAY;
	fcntl(0,F_SETFL, termflags);
}
int tty_mode(int how){
	static struct termios og_mode;
	static int og_flags;
	if(how == 0){
		tcgetattr(0, &og_mode);
		og_flags = fcntl(0, F_GETFL);
	}
	else{
		tcsetattr(0, TCSANOW, &og_mode);
		fcntl(0,F_SETFL,og_flags);
	}
}
