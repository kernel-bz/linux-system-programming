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
#include <fcntl.h>
#include <string.h>

#define TIMEOUT        5    ///초단위의select timeout
#define BUF_LEN     1024    ///바이트단위의read buffer

int main (void)
{
    struct timeval tv;
    fd_set readfds, writefds, otherfds;
    int ret, fd;

    char buf[BUF_LEN+1] = {0,};
    int len;

    ///fd = open("/dev/ttyAMA0", O_RDWR);  ///serial
    //fd = open("/dev/ttyAMA0", O_RDWR | O_NONBLOCK);  ///serial
    fd = open("/dev/ttyS0", O_RDWR | O_NONBLOCK);  ///serial

_RETRY:
    memset(buf, 0, sizeof(buf));

    //stdin에서입력대기
    FD_ZERO (&readfds); ///feadfds[] = 0;
    FD_ZERO (&writefds); ///writefds[] = 0;
    FD_ZERO (&otherfds); ///otherfds[] = 0;

    FD_SET (STDIN_FILENO, &readfds);    ///feadfds[0] = 0 (keyboard)
    FD_SET (fd, &readfds);              ///feadfds[1] = fd

    FD_SET (STDOUT_FILENO, &writefds);  ///writefds[0] = 1 (screen)
    FD_SET (fd, &writefds);  ///writefds[1] = fd

    FD_SET (fd, &otherfds);  ///otherfds[0] = fd

    //5초동안대기
    tv.tv_sec = TIMEOUT;    ///5sec
    tv.tv_usec = 0;
    ///tv.tv_usec = 100;       ///100usec

    ret = select (4, &readfds, &writefds, NULL, &tv);
    if (ret == -1) {
        printf ("select(): error\n");
        return 1;
    } else if (!ret) {  ///ret==0
        printf ("%d seconds elapsed.\n", TIMEOUT);
        return 0;
    }

    if (FD_ISSET (STDIN_FILENO, &readfds))  ///keyboard
    {
        ///printf("FD_ISSET() for keyboard\n");
        len = read (STDIN_FILENO, buf, BUF_LEN);
        if (len == -1) {
            printf ("read(STDIN): error\n");
            return 1;
        }
        if (len > 0) {
            buf[len] = '\0';
            printf ("read(STDIN): %d: %s\n", len, buf);
        }
        ///return 0;
    }

    if (FD_ISSET (STDOUT_FILENO, &writefds))  ///screen
    {
        ///printf("FD_ISSET() for screen\n");

        ///len = write (STDOUT_FILENO, buf, BUF_LEN);
        len = write (STDOUT_FILENO, buf, strlen(buf));
        if (len == -1) {
            printf ("write(STDOUT): error\n");
            return 1;
        }
        if (len > 0) {
            buf[len] = '\0';
            printf ("write(STDOUT): %d: %s\n", len, buf);
        }
        ///return 0;
    }

    //if (FD_ISSET (fd, &otherfds))  ///serial writable
    if (FD_ISSET (fd, &readfds))  ///serial writable
    {
        printf("FD_ISSET() for serial read\n");

        len = read (fd, buf, BUF_LEN);
        if (len == -1) {
            printf ("read(SERIAL): error\n");
            ///return 1;
        }
        if (len > 0) {
            buf[len] = '\0';
            printf ("read(SERIAL): %d: %s\n", len, buf);
        }
        ///return 0;
    }

    if (FD_ISSET (fd, &writefds))  ///serial writable
    {
        printf("FD_ISSET() for serial write\n");

        len = write (fd, buf, BUF_LEN);
        if (len == -1) {
            printf ("write(SERIAL): error\n");
            ///return 1;
        }
        if (len > 0) {
            buf[len] = '\0';
            printf ("write(SERIAL): %d: %s\n", len, buf);
        }
        ///return 0;
    }
    sleep(2);
    goto _RETRY;

    fprintf (stderr, "This should not happen!\n");
    return 1;
}
