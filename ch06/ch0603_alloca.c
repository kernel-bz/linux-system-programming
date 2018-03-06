//
//	file name	: ch0803_alloca.c
//	author		: Jung,JaeJoon(rgbi3307@nate.com) on the www.kernel.bz
//	comments	: alloca example
//
#include <stdio.h>
#include <stdlib.h>
#include <alloca.h>
#include <string.h>

char GlobalBuffer[512];

void alloca_test(char *s1, char *s2)
{
    char *str;

    str = alloca(strlen(s1) + strlen(s2) + 1);
    printf("str=%s, %p\r\n", str, str);

    strcpy(str, s1);
    strcat(str, s2);

    printf("str=%s\r\n", str);
}

void internal_array_test(void)
{
    int i;
    ///char foo[10];

    for (i=0; i<10; i++) {
        char foo[i+1];  ///1,2,3,4....10
        printf("foo[%d]=[%s], size=%d, addr=%p\r\n", i, foo, sizeof(foo), foo);
    }
}

int main()
{
    char internal_buffer[512];

    printf("internal_buffer address: %p\n", internal_buffer);
    printf("GlobalBuffer address: %p\n", GlobalBuffer);

    alloca_test("Hello.", "World.");
    internal_array_test();

    return 0;
}
