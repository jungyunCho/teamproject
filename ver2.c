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
int zDelay = 10000;			/*	좀비 생성 쿨타임	*/
char zombie_pos[4][25];

int set_ticker(struct itimerval, int);	/*	set timer	*/
int playGame(void);		/*	게임시작	*/
//int getkey(void);		/*	대기없이 문자를 바로 입력	*/
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
			zombie_pos[i][j] = 0;
	initscr();			/*	turn on curses	*/
	clear();

	while(menu)
	{
		menu = display_menu();	/*	메뉴그리고 입력받기	*/
		if(menu == 1)
		{
			game = playGame();
		}
		else if(menu == 2)
		{
			show_score();
		}
		else if(menu == 3)
		{
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
	int menu = 0;

	while(1)
	{
		clear();
		move(i++, 0);
		addstr("1호선");
		move(i++, 0);
		addstr("============================");
		move(i++, 0);
		addstr("게 임 메 뉴");
		move(i++, 0);
		addstr("============================");
		move(i++, 0);
		addstr("1) 게임 시작\t   =");
		move(i++, 0);
		addstr("2) 점수 보기\t   =");
		move(i++, 0);
		addstr("3) 게임 종료\t   =");
		move(i++, 0);
		addstr("============================");
		move(i++, 0);
		addstr("선택 : ");
		scanf("%d",&menu);
		if(menu < 1 || menu > 3)
		{
			continue;
		}
		else
		{
			return menu;		/* 입력받은 선택지 반환	*/
		}
	}
	return 0;
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
/*
int getkey(void)
{
	char ch;
	int error;
	static struct termios Otty, Ntty;

	fflush(stdout);
	tcgetattr(0, &Otty);
	Ntty = Otty;
	Ntty.c_iflag = 0;
	Ntty.c_oflag = 0;
	Ntty.c_lflag &= ~ICANON;
#if 1
	Ntty.c_lflag &= ~ECHO;
#else
	Ntty.c_lflag |= ECHO;
#endif
	Ntty.c_cc[VMIN] = CCHAR;
	Ntty.c_cc[VTIME] = CTIME;
#if 1
#define FLAG TCSAFLUSH
#else
#define FLAG TCSANOW
#endif
	if(0 == (error = tcsetattr(0, FLAG, &Ntty)))
	{
		error = read(0, &ch, 1);
		error += tcsetattr(0, FLAG, &Otty);
	}

	return (error == 1 ? (int) ch : -1);
}
*/
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

/*	좀비 랜덤 생성	*/
void new_zombie()
{
	int ran_pla;	// 랜덤으로 정해지는 생성 레인
	int i;

	srand((unsigned)time(NULL));
	ran_pla = rand()%4;
	zombie_pos[ran_pla][0] = 1;		//	레인 첫번째 줄에 좀비 생성
}



void draw_zomebie()
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
}
