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

	ret = time (&t);
	if (ret)
		perror ("stime");
    printf("time = %d\n", t);

    /**
	///set time to one second after the epoch
	ret = stime (&t);
	if (ret)
		perror ("stime");
    */

	return 0;
}
