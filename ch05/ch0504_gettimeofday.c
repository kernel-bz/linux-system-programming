#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

int main(void)
{
	long hz;
	hz = sysconf (_SC_CLK_TCK);
	if (hz == -1) perror ("sysconf");
	printf("HZ=%ld\n", hz);

	struct timeval tv;
	int ret;

	ret = gettimeofday (&tv, NULL);
    	if (ret)
        	perror ("gettimeofday");
    	else
        	printf ("seconds=%ld useconds=%ld\n",
                	(long) tv.tv_sec, (long) tv.tv_usec);

    /**
	struct timeval tv = { .tv_sec = 1327719434,
						  .tv_usec = 457437};

	ret = settimeofday (&tv, NULL);
	if (ret)
		perror ("settimeofday");
    */

	return 0;
}
