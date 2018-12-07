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
#include <time.h>
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
void move_zombie(int fall_speed, int k, int ran_pla);
void set_cr_noecho_mode();
void set_nodelay();
int tty_mode();

void main(){
	int i;
	int j;
	int level;
	char c;
	int count=0;
	char cnt[100];
	int ran_pla;
        int k;


	//set_cr_noecho_mode();
	//set_nodelay();

	initscr();
	noecho();
	cbreak();
	nodelay(stdscr,TRUE);
	draw();
       // tty_mode(0);
//      noecho();
//      cbreak();

        while(1)
	{
                move(i,5);
		srand((unsigned)time(NULL));
	        ran_pla = rand()%4;

		for(k=5; k<25; k)
		{

			move_zombie(FALL_5, k++, ran_pla);

			if(ran_pla==0&&(c=getch()) == 'a')
			{
				count++;
				break;
			}
			if(ran_pla==1&&(c=getch()) == 's')
			{
				count++;
                                break;
			}
			if(ran_pla==2&&(c=getch()) == 'd')
			{
				count ++;
                                break;
			}
			if(ran_pla==3&&(c=getch()) == 'f')
			{
				count++;
                                break;
			}

		}
                refresh();
                sleep(1);
                j++;
		if(count == 10)
		break;
        }
        sprintf(cnt,"%d",count);
        move(j+1,5);
        addstr(cnt);
        refresh();
        sleep(5);

        //tty_mode(1);
        endwin();

}

void draw(){
	 int i;

	 clear();
	 move(1,4);
	 addstr("press each button below");

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

void move_zombie(int fall_speed, int j, int ran_pla){

	switch(ran_pla){
		case 0:
				move(j,6);
			       	addstr("   zz   ");
				refresh();
				usleep(fall_speed);
				move(j,6);
				addstr("        ");
				break;
		case 1:
				move(j,15);
                                addstr("   zz   ");
                                refresh();
                                usleep(fall_speed);
                                move(j,15);
                                addstr("        ");
                                break;

		case 2: 
				move(j,24);
                                addstr("   zz   ");
                                refresh();
                                usleep(fall_speed);
                                move(j,24);
                                addstr("        ");
                                break;

				
		case 3:
				move(j,33);
                                addstr("   zz   ");
                                refresh();
                                usleep(fall_speed);
                                move(j,33);
                                addstr("        ");
                                break;

	}
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
	int termflags;
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

