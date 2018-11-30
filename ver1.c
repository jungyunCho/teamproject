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

int game = 0;
int zombiePos = 0;
int speed = 500;

int set_ticker(struct itimerval, int);	/*	set timer	*/
int playGame(void);		/*	게임시작	*/
//int getkey(void);		/*	대기없이 문자를 바로 입력	*/
void game_handler(int);

int main(void)
{
	initscr();			/*	turn on curses	*/
	clear();

	while(1)
	{
		game = playGame();
	}

	endwin();
	exit(0);
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

	while(1)
	{
		if(game == 1)
		{
			set_ticker(new_timeset, 0);
			move(zombiePos+1,0);
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

void game_handler(int signum)
{
	int hit = 0;
	char ch;
	if(zombiePos < 10)
	{
		move(zombiePos, 0);
		addstr("Z");
		refresh();
		ch = getch();
		//addstr(ch);
		switch(ch)
		{
			case 65	:
			case 97	:			/*	when you stroke 'a'	*/
				addstr("*HIT*");
				break;
			default	:
				break;
		}
		move(zombiePos, 0);
		addstr(" ");
		zombiePos++;
	}
	else
	{
		game = 1;
	}
}
