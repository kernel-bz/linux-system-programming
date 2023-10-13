//
//	file name	: ch0701_time.c
//	author		: Jung,JaeJoon(rgbi3307@nate.com) on the www.kernel.bz
//	comments	: time example
//
#include <time.h>
#include <stdio.h>

int main(void)
{
	time_t t = 1;   ///long int
	int ret;

	ret = time (&t);	//epoch(1971-01-01) seconds + power on
	if (ret)
		perror ("stime");
	printf("time = %d, size = %d\n", t, sizeof(t));

	/**
	///set time to one second after the epoch (1971-01-01)
	ret = stime (&t);
	if (ret)
		perror ("stime");
	*/

	return 0;
}
