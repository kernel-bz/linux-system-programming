//
//	file name : ch0510.c
//	comments  : file mmap example
//	author	  : Jung,JaeJoon(rgbi3307@nate.com) on the www.kernel.bz
//
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main (int argc, char *argv[])
{
    struct stat sb;
    off_t len;
    char *p;
    int fd;

    if (argc < 2) {
        fprintf (stderr, "usage: %s <file>\n", argv[0]);
        return 1;
    }

    fd = open (argv[1], O_RDWR);
    if (fd == -1) {
        perror ("open");
        return 1;
    }
    if (fstat (fd, &sb) == -1) {
        perror ("fstat");
        return 1;
    }
    if (!S_ISREG (sb.st_mode)) {
        fprintf (stderr, "%s is not a file\n", argv[1]);
        return 1;
    }

    p = mmap (0, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED) {
        perror ("mmap");
        return 1;
    } else {
        printf("mmap succeeded: %p\n", p);
    }

    if (close (fd) == -1) {
        perror ("close");
        return 1;
    }

    p[0] = 'S';
    p[1] = 'O';
    p[2] = 'G';
    p[3] = 'A';
    p[4] = 'N';
    p[5] = 'G';
    for (len = 0; len < sb.st_size; len++)
        putchar (p[len]);
    ///puts(p);

    if (munmap (p, sb.st_size) == -1) {
        perror ("munmap");
        return 1;
    }

    return 0;
}
