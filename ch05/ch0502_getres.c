//
//	file name	: ch0702_getres.c
//	author		: Jung,JaeJoon(rgbi3307@nate.com) on the www.kernel.bz
//	comments	: clock_getres example
//
#include <stdio.h>
#include <time.h>

int main(void)
{
	clockid_t clocks[] = {
		CLOCK_REALTIME,                         ///Wall Time
		CLOCK_MONOTONIC,                        ///Up Time
		CLOCK_PROCESS_CPUTIME_ID,       ///Process
		CLOCK_THREAD_CPUTIME_ID,        ///Thread
		(clockid_t) -1
	};
	int i;

	for (i = 0; clocks[i] != (clockid_t) -1; i++) {
		struct timespec ts;
		int ret;
		ret = clock_getres (clocks[i], &ts);
		if (ret)
			perror ("clock_gettime error!");
		else
            printf ("clock=%d sec=%ld nsec=%ld\n",
						clocks[i], ts.tv_sec, ts.tv_nsec);
	}
	return 0;
}

