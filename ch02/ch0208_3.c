//
//	file name : ch0408.c
//	comments  : poll example
//	author	  : Jung,JaeJoon(rgbi3307@nate.com) on the www.kernel.bz
//
#include <stdio.h>
#include <unistd.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <string.h>

#define TIMEOUT     5

//poll timeout, in seconds
int main (void)
{
    struct pollfd fds[4];
    int ret, fd;
    char buf[80];

    //watch stdin for input
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;
    //watch stdout for ability to write (almost always true)
    fds[1].fd = STDOUT_FILENO;
    fds[1].events = POLLOUT;

    ///fd = open("/dev/ttyAMA0", O_RDWR | O_NONBLOCK);  ///serial;
    fd = open("/dev/ttyAMA0", O_RDWR);  ///serial;
    printf("serial fd=%d\n", fd);

    fds[2].fd = fd;
    fds[2].events = POLLIN;

    fds[3].fd = fd;
    fds[3].events = POLLOUT;

    while (1)
    {
        memset(buf, 0, sizeof(buf));

        ret = poll (fds, 4, TIMEOUT * 1000);

        if (ret == -1) {
            printf ("poll(): error\n");
            return 1;
        }
        if (!ret) { ///ret==0
            printf ("%d seconds elapsed.\n", TIMEOUT);
            return 0;
        }

        if (fds[0].revents & POLLIN) {
            ///printf ("stdin is readable\n");
            read(fds[0].fd, buf, sizeof(buf));
        }

        if (fds[1].revents & POLLOUT) {
            ///printf ("stdout is writeable\n");
            write(fds[1].fd, buf, strlen(buf));
        }

        if (fds[2].revents & POLLIN) {
            printf ("serial is readable\n");
            read(fds[2].fd, buf, sizeof(buf));
	    printf("serial read: %s\n", buf);
        }

        if (fds[3].revents & POLLOUT) {
            printf ("serial is writeable\n");
            //write(fds[3].fd, buf, strlen(buf));
        }

        sleep(1);
    }

    return 0;
}
