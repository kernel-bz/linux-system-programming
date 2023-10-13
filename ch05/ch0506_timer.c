#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

void alarm_handler (int signo)
{
	printf ("Timer hit!\n");
}

void my_timer(void)
{
	timer_t timer;
	int ret;

	signal (SIGALRM, alarm_handler);

	//ret = timer_create (CLOCK_PROCESS_CPUTIME_ID, NULL, &timer);
	ret = timer_create (CLOCK_REALTIME, NULL, &timer);
	if (ret)
		perror ("timer_create");

	struct itimerspec ts;
	ts.it_interval.tv_sec = 4;
	ts.it_interval.tv_nsec = 0;
	ts.it_value.tv_sec = 1;
	ts.it_value.tv_nsec = 0;
	ret = timer_settime (timer, 0, &ts, NULL);
	if (ret)
		perror ("timer_settime");

	pause();
}

int main(void)
{
	//signal (SIGALRM, alarm_handler);
	my_timer();
	while(1) pause();
	return 0;
}

