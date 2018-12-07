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
char zombie[8] = "   zz   ";		/*	좀비 모습	*/
int zDelay = 10000;			/*	좀비 생성 쿨타임	*/
char zombie_pos[4][20];

//int set_ticker(struct itimerval, int);	/*	set timer	*/
//int playGame(void);		/*	게임시작	*/
//void game_handler(int);
//void draw();		/*	배경 그리기	*/
//void new_zombie();	/* 	zDelay시간마다 좀비 이동시키기		*/
//void draw_zombie();
//void show_score();		/*	점수 보여주기	*/
int display_menu();

int score=0;
int main(void)
{
	int menu = 1;
	int i, j;
//	for(i=0;i<4;i++)
//		for(j=0;j<20;j++)
//			zombie_pos[i][j] = 0;
	initscr();			/*	turn on curses	*/
	clear();

	while(menu)
	{
		menu = display_menu();	/*	메뉴그리고 입력받기	*/
		if(menu == 1)
		{
			clear();
			addstr("11111");
			refresh();
			sleep(2);
			//game = playGame();
		}
		else if(menu == 2)
		{
			clear();
			addstr("22222");
			refresh();
			sleep(2);
			//show_score();
		}
		else if(menu == 3)
		{
			clear();
			addstr("33333");
			refresh();
			sleep(2);
			exit(0);
		}
	}

	endwin();
	exit(0);
}

/* 메뉴를 보여줌 */
int display_menu(void)
{
	int i=0;
	int menu;

	while(1)
	{
		clear();
		move(i++, 0);
		addstr("Line #1");
		move(i++, 0);
		addstr("============================");
		move(i++, 0);
		addstr("Game Menu");
		move(i++, 0);
		addstr("============================");
		move(i++, 0);
		addstr("1) Start Game\t\t   =");
		move(i++, 0);
		addstr("2) Score\t\t   =");
		move(i++, 0);
		addstr("3) End Game\t\t   =");
		move(i++, 0);
		addstr("============================");
		move(i++, 0);
		addstr("User : ");
		refresh();
		scanw("%d",&menu);
		if(menu < 1 || menu > 3)
		{
			i=0;
			continue;
		}
		else
		{
			return menu;		/* 입력받은 선택지 반환	*/
		}
	}
	//return 0;
}

/*int playGame(void)
{
	static struct sigaction sa;
	static struct itimerval new_timeset;
	
	memset(&sa, 0, sizeof (sa));
	sa.sa_handler = &game_handler;
	sigaction(SIGVTALRM, &sa, NULL);

	if(set_ticker(new_timeset, speed) == -1)
		perror("set_ticker");
	
	draw();		//	배경 그리기
	while(1)
	{
		
		if(game == 1)	// 게임 종료
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

void game_handler(int signum)	//게임이 시작되면 0.1초마다 호출되며 배경그리기, 좀비이동, 키입력을 받는 함수
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
		case 97	:			//when you stroke 'a'
			addstr("*HIT*");
			score++;
			break;
		case 27	:
			game = 1;
			break;
		default	:
			break;
	}
}

void draw(void)
{				//	배경을 그리는 함수
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

//	좀비 랜덤 생성	
void new_zombie(void)
{
	int ran_pla;	// 랜덤으로 정해지는 생성 레인
	int i;

	srand((unsigned)time(NULL));
	ran_pla = rand()%4;
	zombie_pos[ran_pla][0] = 1;		//	레인 첫번째 줄에 좀비 생성
}



void draw_zomebie(void)
{
	int x;		// 레인 번호
	int y;
	for(x=0;x<4;x++)
		for(y=0;y<25;y++)
			if(zombie_pos[x][y] == 1)
			{
				move(x*9-5, y);
				addstr(zombie);
			}
}*/

