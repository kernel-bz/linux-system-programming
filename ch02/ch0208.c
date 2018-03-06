//
//	file name : ch0408.c
//	comments  : poll example
//	author	  : Jung,JaeJoon(rgbi3307@nate.com) on the www.kernel.bz
//
#include <stdio.h>
#include <unistd.h>
#include <sys/poll.h>

#define TIMEOUT     5

//poll timeout, in seconds
int main (void)
{
    struct pollfd fds[2];
    int ret;

    //watch stdin for input
    fds[0].fd = STDIN_FILENO;   ///0: keyboard
    fds[0].events = POLLIN;     ///for input
    //watch stdout for ability to write (almost always true)
    fds[1].fd = STDOUT_FILENO;  ///1: screen
    fds[1].events = POLLOUT;    ///for output

    while (1)
    {
        //All set, block!
        ret = poll (fds, 2, TIMEOUT * 1000);    ///timeout for 5000ms == 5s

        if (ret == -1) {
            printf ("poll(): error\n");
            return 1;
        }
        if (!ret) { ///ret==0
            printf ("%d seconds elapsed.\n", TIMEOUT);
            return 0;
        }

        if (fds[0].revents & POLLIN)
            printf ("stdin is readable\n");

        if (fds[1].revents & POLLOUT)
            printf ("stdout is writeable\n");

        sleep(1);
    }

    return 0;
}
