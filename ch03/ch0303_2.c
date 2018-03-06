//
//	file name : ch0503.c
//	comments  : readv example
//	author	  : Jung,JaeJoon(rgbi3307@nate.com) on the www.kernel.bz
//
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>

int main ()
{
    ///char foo[47], bar[50], baz[48];
    char foo[48], bar[51], baz[49];
    struct iovec iov[3];
    ssize_t nr;
    int fd, i;

    fd = open ("buccaneer.txt", O_RDONLY);
    if (fd == -1) {
        perror ("open");
        return 1;
    }
    //iovec 구조체를설정한다.
    iov[0].iov_base = foo;
    iov[0].iov_len = sizeof (foo)-1;
    iov[1].iov_base = bar;
    iov[1].iov_len = sizeof (bar)-1;
    iov[2].iov_base = baz;
    iov[2].iov_len = sizeof (baz)-1;

    //한번의호출로구조체에읽어들인다.
    nr = readv (fd, iov, 3);
    if (nr == -1) {
        perror ("readv");
        return 1;
    }

    foo[47] = '\0';
    bar[50] = '\0';
    baz[48] = '\0';
    for (i = 0; i < 3; i++) {
        printf ("%d: %s", i, (char *) iov[i].iov_base);
    }
    printf("foo=%p: %s", foo, foo);
    printf("bar=%p: %s", bar, bar);
    printf("baz=%p: %s", baz, baz);

    printf("-----------------------------\n");
    printf("%p, %p, %p\n", iov[0].iov_base, iov[1].iov_base, iov[2].iov_base);
    printf("%p, %p, %p\n", foo, bar, baz);

    char *p;
    p = iov[0].iov_base;
    for(i=0; i<sizeof (foo); i++) {
        printf("%c", *p);
        p++;
    }
    printf("-----------------------------\n");
    p = iov[1].iov_base;
    for(i=0; i<sizeof (bar); i++)
        printf("%c", *p++);
    printf("-----------------------------\n");
    p = iov[2].iov_base;
    for(i=0; i<sizeof (baz); i++)
        printf("%c", *p++);
    printf("-----------------------------\n");

    if (close (fd)) {
        perror ("close");
        return 1;
    }

    return 0;
}
