//
//	file name : ch0407.c
//	comments  : select example
//	author	  : Jung,JaeJoon(rgbi3307@nate.com) on the www.kernel.bz
//
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>

#define TIMEOUT        5    ///초단위의select timeout
#define BUF_LEN     1024    ///바이트단위의read buffer

int main (void)
{
    struct timeval tv;
    fd_set readfds;
    int ret;

    //stdin에서입력대기
    FD_ZERO (&readfds);
    FD_SET (STDIN_FILENO, &readfds);	//keyboard: 0
    //5초동안대기
    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;

    printf("stdin=%d\n", STDIN_FILENO);

_retry:
    ret = select (STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
    if (ret == -1) {
        printf ("select(): error\n");
        return 1;
    } else if (!ret) {  ///ret==0
        printf ("%d seconds elapsed.\n", TIMEOUT);
        return 0;
    }

    if (FD_ISSET (STDIN_FILENO, &readfds))
    {
        char buf[BUF_LEN+1];
        int len;
        len = read (STDIN_FILENO, buf, BUF_LEN);
        if (len == -1) {
            printf ("read(): error\n");
            return 1;
        }
        if (len) {
            buf[len] = '\0';
            printf ("read: %s\n", buf);
        }
        //return 0;
	goto _retry;
    }

    fprintf (stderr, "This should not happen!\n");
    return 1;
}
