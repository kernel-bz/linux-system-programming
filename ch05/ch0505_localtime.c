#include <stdio.h>
#include <time.h>
/**
struct tm {
  int tm_sec;   ///초
  int tm_min;   ///분
  int tm_hour;  ///시 (0--23)
  int tm_mday;  ///일 (1--31)
  int tm_mon;   ///월 (0--11)
  int tm_year;  ///년 (+ 1900)
  int tm_wday;  ///요일 (0--6; 일요일 = 0)
  int tm_yday;  ///올해 몇번째 날 (0--365)
  int tm_isdst; ///서머타임 여부
};
*/

int main (void)
{
	time_t t = time (NULL);

	printf("time: %ld\n", t);
	printf ("the time a mere line ago: %s", ctime (&t));

	struct tm *tms;
	///tms = gmtime (&t);  ///GMT
	tms = localtime (&t);
	printf ("year-month-day hh:mm:ss %d, %d-%d-%d %d:%d:%d\n"
                , tms->tm_year
		, tms->tm_year % 100 + 2000, tms->tm_mon+1, tms->tm_mday
		, tms->tm_hour, tms->tm_min, tms->tm_sec);
	return 0;
}
