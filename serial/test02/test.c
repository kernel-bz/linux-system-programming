#include <stdio.h>
#include <string.h>

int main(void)
{
	char buf[5] = {1,2,3,4,0};
	printf("strlen = %d\n", strlen(buf));
	return 0;
}
