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
int zDelay = 1000;			/*	좀비 생성 쿨타임	*/
int zombie_pos[4][20];
int life = 10;
double best_time=0;

static struct user{
	double u_time;
	char u_name[30];
	int year;
	int month;
	int day;
	int hour;
	int min;
	int rank;
}temp_result;

int set_ticker(struct itimerval, int);	/*	set timer	*/
int playGame(void);		/*	게임시작	*/
void game_handler(int);
void draw(void);		/*	배경 그리기	*/
void new_zombie(void);	/* 	zDelay시간마다 좀비 이동시키기		*/
void draw_zombie(void);
int show_score(void);		/*	점수 보여주기	*/
int display_menu(void);

int main(void)
{
	int menu = 1;
	int i, j;
	initscr();			/*	turn on curses	*/
	clear();

	while(menu)
	{
		for(i=0;i<4;i++)
			for(j=0;j<25;j++)
				 zombie_pos[i][j] = 0;       // initialize zombie_pos
		echo();
		nocbreak();
		nodelay(stdscr, FALSE);
		
		menu = display_menu();	/*	메뉴그리고 입력받기	*/
		if(menu == 1)
		{	
			game = 0;
			noecho();
			cbreak();
			nodelay(stdscr, TRUE);
			game = playGame();
		}
		else if(menu == 2)
		{
			show_score();
		}
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
		addstr("1) Start New Game");
		move(i++, 0);
		addstr("2) Score");
		move(i++, 0);
		addstr("3) Quit");
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
	FILE *fp = NULL;
	time_t ptime;
	struct tm *t;
	double re_time;
	static struct sigaction sa;
	static struct itimerval new_timeset;
	struct timeval start_time, dead_time;
	memset(&sa, 0, sizeof (sa));
	sa.sa_handler = &game_handler;
	sigaction(SIGVTALRM, &sa, NULL);

	if(set_ticker(new_timeset, speed) == -1)
		perror("set_ticker");
	
	draw();		/*	배경 그리기	*/
	gettimeofday(&start_time,NULL);
	while(1)
	{
		
		if(life < 0)	/* 게임 종료	*/
		{
			game = 1;
			life = 10;
			set_ticker(new_timeset, 0);
			gettimeofday(&dead_time,NULL);
			re_time=
				(double)(dead_time.tv_sec)+(double)(dead_time.tv_usec)/1000000.0-
				(double)(start_time.tv_sec)-(double)(start_time.tv_usec)/1000000.0;
			clear();
			move(8,16);
			addstr("Game Over!");
			
			temp_result.u_time = re_time;
			printw("\nyour result: %lf\n",temp_result.u_time);
			printw("name: ");
			refresh();
			
			echo();
			nocbreak();
			nodelay(stdscr,FALSE);
			
			scanw("%s%*c",temp_result.u_name);
			
			if(temp_result.u_time >= best_time)
				best_time=temp_result.u_time;

			ptime = time(NULL);
			t=localtime(&ptime);

			temp_result.year = t->tm_year + 1900;
			temp_result.month = t->tm_mon + 1;
			temp_result.day = t->tm_mday;
			temp_result.hour = t->tm_hour;
			temp_result.min = t->tm_min;

			fp = fopen("result", "ab");
			fseek(fp,1,SEEK_END);
			fwrite(&temp_result, sizeof(struct user), 1, fp);
			fclose(fp);
			return 1;
		}
	}

	return 0;
}

int show_score(void){
	FILE *fp = NULL;
	char ch = 1;

	fp = fopen("result", "rb");

	if(fp == NULL)
		return 0;

	clear();

	printw("\n\t\t\t\t Line #1");
	printw("\n\t\t\t\t Score board");
	printw("\n\t\t Name\t\tScore\t  \t Date\t\t Time");

	while(1){
		fread(&temp_result, sizeof(struct user), 1, fp);
		if(!feof(fp)){
			printw("\n\t==================================================================");
			printw("\n\t\t%s\t\t%lf\t %d. %d. %d.  |  %d : %d\n", 
					temp_result.u_name, temp_result.u_time, temp_result.year, 
					temp_result.month, temp_result.day, temp_result.hour, 
					temp_result.min);
		}
		else{
			break;
		}
	}

	fclose(fp);

	noecho();
	cbreak();
	printw("\n\n\t Back to Menu : Press M");
	while(1){
		ch = getch();
		if(ch == 77 || ch == 109)
			break;
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
	refresh();
	int i;
	char ch;
	new_zombie();
	draw_zombie();
	move(0,0);
	printw("Remain Life : %2d", life);
	ch = getch();
	
	move(28,7);		/*	erase HIT	*/
	addstr("     ");
	move(28,18);
	addstr("     ");
	move(28,27);
	addstr("     ");
	move(28,36);
	addstr("     ");

	flushinp();
	
	switch(ch)
	{
		case 65	:
		case 97	:			/*	when you stroke 'a'	*/
			if(zombie_pos[0][15] == 1 || zombie_pos[0][16] == 1){
				zombie_pos[0][15] = 0;
				zombie_pos[0][16] = 0;
				move(28,7);
				addstr("*HIT*");
				move(21,9);
				addstr("  ");
				flushinp();
			}
			break;
		case 83 :		// s or S
		case 115 :
			if(zombie_pos[1][15] == 1 || zombie_pos[1][16] == 1){
				zombie_pos[1][15] = 0;
				zombie_pos[1][16] = 0;
				move(28,18);
				addstr("*HIT*");
				move(21,18);
                                addstr("  ");
				flushinp();
			}
			break;
		case 68 :		// d or D
		case 100 :
			if(zombie_pos[2][15] == 1 || zombie_pos[2][16] == 1){
				zombie_pos[2][15] = 0;
				 zombie_pos[2][16] = 0;
				 move(28,27);
				 addstr("*HIT*");
				move(21,27);
                                addstr("  ");
				 flushinp();
			}
            break;
		case 70 :		// f or F
		case 102 :
			if(zombie_pos[3][15] == 1 || zombie_pos[3][16] == 1){
				zombie_pos[3][15] = 0;
				zombie_pos[3][16] = 0;
				move(28,36);
				addstr("*HIT*");
				move(21,36);
                                addstr("  ");
				flushinp();
			}
			break;
		case 27	:
			game = 1;
			flushinp();
			break;
		default	:
			flushinp();
			break;
	}
	
	for(i=0;i<4;i++)
	{
		if(zombie_pos[i][17] == 1){
			zombie_pos[i][17] = 0;
			life--;
		}
	}
	//refresh();
}

void draw()
{				/*	배경을 그리는 함수	*/
         int i;

         clear();
         move(1,4);
         addstr("press each button below");

         for(i=0;i<17;i++){
                 move(i+5,5);
		if(i==15)
			addstr("|-->  <--|-->  <--|-->  <--|-->  <--|");
		else if(i==16)
			addstr("|==>  <==|==>  <==|==>  <==|==>  <==|");
		else
			addstr("|        |        |        |        |");
         }
		 move(20,43);
		 addstr("Hit!");
		 move(21,43);
		 addstr("Danger!");
         move(22,9);
         addstr("AA");
         move(22,18);
         addstr("SS");
         move(22,27);
         addstr("DD");
         move(22,36);
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
		for(j=20;j>0;j--)
			if(zombie_pos[i-1][j-1] == 1)		//	zombies move 1 step
			{
				zombie_pos[i-1][j-1] = 0;
				if(j<18)
					zombie_pos[i-1][j] = 1;
			}

	generate = rand()%2;			// the chance that zombie will generate
	ran_lane = rand()%4;
	if(generate == 1)
		zombie_pos[ran_lane][0] = 1;		//	generate new zombie at random lane
}

void draw_zombie()
{
	int x;
	int y;
	
	for(x=0;x<4;x++)
		for(y=0;y<20;y++)
			if(zombie_pos[x][y] == 1)
			{
				move(y+4, (x+1)*9);
				addstr("  ");
				if(y<17)
				{
				move(y+5, (x+1)*9);
				addstr(zombie);
				}
			}
}
