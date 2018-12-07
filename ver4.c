#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

#define CCHAR 0

int game = 0;			/*	게임시작 0 게임종료 1		*/
int speed = 500;		/*	게임 스피드		*/
int timelimit;			/*	스테이지 제한 시간		*/
char zombie[2] = "zz";		/*	좀비 모습	*/
int zDelay = 500;			/*	좀비 생성 쿨타임	*/
int zombie_pos[4][25];

int set_ticker(struct itimerval, int);	/*	set timer	*/
int playGame(void);		/*	게임시작	*/
void game_handler(int);
void draw(void);		/*	배경 그리기	*/
void new_zombie(void);	/* 	zDelay시간마다 좀비 이동시키기		*/
void draw_zombie(void);
void show_score(void);		/*	점수 보여주기	*/
int display_menu(void);

int main(void)
{
	int menu = 1;
	int i, j;
	for(i=0;i<4;i++)
		for(j=0;j<25;j++)
			zombie_pos[i][j] = 0;		// initialize zombie_pos
	initscr();			/*	turn on curses	*/
	clear();

	while(menu)
	{
		
		echo();
		nocbreak();
		nodelay(stdscr, FALSE);
		
		menu = display_menu();	/*	메뉴그리고 입력받기	*/
		if(menu == 1)
		{
			noecho();
			cbreak();
			nodelay(stdscr, TRUE);
			game = playGame();
		}/*
		else if(menu == 2)
		{
			show_score();
		}*/
		else if(menu == 3)
		{
			break;
		}
	}

	endwin();
	exit(0);
}

/* 메뉴를 보여줌 */
int display_menu(void)
{
	int i=0;
	int menu = 0;

	while(1)
	{
		i = 0;
		clear();
		move(i++, 0);
		addstr("Welcome to the 1st line");
		move(i++, 0);
		addstr("============================");
		move(i++, 0);
		addstr("Menu");
		move(i++, 0);
		addstr("============================");
		move(i++, 0);
		addstr("1) Start New Game\t   =");
		move(i++, 0);
		addstr("2) Score\t   =");
		move(i++, 0);
		addstr("3) Quit\t   =");
		move(i++, 0);
		addstr("============================");
		move(i++, 0);
		addstr("Choose : ");
		refresh();
		scanw("%d",&menu);
		if(menu < 1 || menu > 3)
		{
			continue;
		}
		else
		{
			return menu;		/* 입력받은 선택지 반환	*/
		}
	}
	//return 0;
}

int playGame(void)
{
	static struct sigaction sa;
	static struct itimerval new_timeset;
	
	memset(&sa, 0, sizeof (sa));
	sa.sa_handler = &game_handler;
	sigaction(SIGVTALRM, &sa, NULL);

	if(set_ticker(new_timeset, speed) == -1)
		perror("set_ticker");
	
	draw();		/*	배경 그리기	*/
	while(1)
	{
		
		if(game == 1)	/* 게임 종료	*/
		{
			set_ticker(new_timeset, 0);
			addstr("Game Over!");
			return 1;
		}
	}

	return 0;
}

int set_ticker(struct itimerval timeset, int n_msecs)
{
	long n_sec, n_usecs;

	n_sec = n_msecs/1000;
	n_usecs = (n_msecs%1000)*1000L;

	timeset.it_interval.tv_sec = n_sec;
	timeset.it_interval.tv_usec = n_usecs;
	timeset.it_value.tv_sec = n_sec;
	timeset.it_value.tv_usec = n_usecs;

	return setitimer(ITIMER_VIRTUAL, &timeset, NULL);
}

void game_handler(int signum)	/*	게임이 시작되면 0.1초마다 호출되며 배경그리기, 좀비이동, 키입력을 받는 함수	*/
{
	int hit = 0;
	char ch;
	new_zombie();
	draw_zombie();
	refresh();
	ch = getch();
	//addstr(ch);
	switch(ch)
	{
		case 65	:
		case 97	:			/*	when you stroke 'a'	*/
			move(0,0);
			addstr("*HIT*");
			break;
		case 27	:
			game = 1;
			break;
		default	:
			break;
	}
}

void draw()
{				/*	배경을 그리는 함수	*/
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

/*	generate new zombie randomly at random lane	*/
void new_zombie()
{
	int generate;
	int ran_lane;	// random lane where zombies are generated
	int i,j;

	srand((unsigned)time(NULL));

	for(i=4;i>0;i--)
		for(j=24;j>0;j--)
			if(zombie_pos[i-1][j-1] == 1)		//	zombies move 1 step
			{
				zombie_pos[i-1][j-1] = 0;
				zombie_pos[i-1][j] = 1;
			}

	generate = rand()%2;			// the chance that zombie will generate
	ran_lane = rand()%4;
	if(generate == 0)
		zombie_pos[ran_lane][0] = 1;		//	generate new zombie at random lane
}

void draw_zombie()
{
	int x;
	int y;
	
	for(x=0;x<4;x++)
		for(y=0;y<25;y++)
			if(zombie_pos[x][y] == 1)
			{
				move(y-1, (x+1)*9);
				addstr("  ");
				move(y, (x+1)*9);
				addstr(zombie);
			}
}
