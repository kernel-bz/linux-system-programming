#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

struct itimerval old_timer;

void delay(clock_t n)
{
    clock_t start = clock();    ///long int
    clock_t c1, c2;

    printf("delay(): start=%ld\n", start);

    c2 = clock() - start;
    while(c2 < n) {
        c1 = clock();
        c2 = c1 - start;
    }
    printf("c1=%ld, c2=%ld\n", c1, c2);
}

void pause_timer()
{
    struct itimerval zero_timer = { {0}, {0} };

    setitimer(ITIMER_REAL,  &zero_timer, &old_timer);
}

void resume_timer()
{
    setitimer(ITIMER_REAL, &old_timer, NULL);
}

void alarm_handler (int signo)
{
	printf ("Timer hit!\n");
}

void foo (void)
{
	///struct itimerval delay;
	int ret;
	signal (SIGALRM, alarm_handler);

	old_timer.it_value.tv_sec = 3;
	old_timer.it_value.tv_usec = 0;
	old_timer.it_interval.tv_sec = 1;
	old_timer.it_interval.tv_usec = 0;

	ret = setitimer (ITIMER_REAL, &old_timer, NULL);
	///ret = setitimer (ITIMER_VIRTUAL, &old_timer, NULL);
	///ret = setitimer (ITIMER_PROF, &old_timer, NULL);
	if (ret) {
		perror ("setitimer");
		return;
	}
	///pause ();
}

int main (void)
{
    ///int i;

	foo ();

	while(1)
	{
        ///sleep(10);

        printf("pause_timer()\r\n");
        pause_timer();
        sleep(5);   ///scheduling

        printf("resume_timer()\r\n");

        resume_timer();
        ///sleep(10);   ///collison with SIGALRM (man 3 sleep)
        delay(10000000);  ///us, 10s
	}
	return 0;
}
