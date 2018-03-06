//
//	file name : ch0407_2.c
//	comments  : select example
//	author	  : Jung,JaeJoon(rgbi3307@nate.com) on the www.kernel.bz
//
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>
#include <string.h>

#define TIMEOUT        5    ///초단위의select timeout
#define BUF_LEN     1024    ///바이트단위의read buffer

int main (void)
{
    struct timeval tv;
    fd_set readfds;
    fd_set writefds;
    int ret;

    char buf[BUF_LEN+1];
    int len;

_RETRY:
    memset(buf, 0, sizeof(buf));

    FD_ZERO (&readfds);  ///clear
    FD_ZERO (&writefds); ///clear

    FD_SET (STDIN_FILENO, &readfds);     ///fd==0
    FD_SET (STDOUT_FILENO, &writefds);   ///fd==1

    ///5초동안대기
    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;
    ///tv.tv_usec = 100;  ///0.1ms

    ///ret = select (STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
    ret = select (3, &readfds, &writefds, NULL, &tv);
    if (ret == -1) {
        printf ("select(): error\n");
        return 1;
    } else if (!ret) {  ///ret==0
        printf ("%d seconds elapsed.\n", TIMEOUT);
        return 0;
    }

    if (FD_ISSET (STDIN_FILENO, &readfds))
    {
        len = read (STDIN_FILENO, buf, BUF_LEN);
        if (len == -1) {
            printf ("read(): error\n");
            return 1;
        }
        if (len) {
            buf[len] = '\0';
            printf ("read(STDIN): %d: %s\n", len, buf);
        }
        ///return 0;
    }

    if (FD_ISSET (STDOUT_FILENO, &writefds))
    {
        len = write (STDOUT_FILENO, buf, sizeof(buf));
        ///len = write (STDOUT_FILENO, buf, strlen(buf));
        if (len == -1) {
            printf ("write(): error\n");
            return 1;
        }
        if (len) {
            printf ("write(STDOUT): %d: %s\n", len, buf);
        }
        ///return 0;
    }

    sleep(2);
    goto _RETRY;

    fprintf (stderr, "This should not happen!\n");
    return 1;
}
