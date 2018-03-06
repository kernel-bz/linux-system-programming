//
//	file name	: ch0801_mmap.c
//	author		: Jung,JaeJoon(rgbi3307@nate.com) on the www.kernel.bz
//	comments	: anoymous mapping example
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>

void *mmap_anoymous(int size)
{
    void *p;

    p = mmap(NULL, size, PROT_READ | PROT_WRITE
                    , MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (p == MAP_FAILED)  {
        printf("mmap() error\r\n");
        return NULL;
    } else {
        printf("mmap_anoymous: mamp() addr=%p\r\n", p);
        return p;
    }
}

void *mmap_dev_zero(int size)
{
    void *p;
    int fd;

    fd = open("/dev/zero", O_RDWR);
    if (fd < 0) {
        printf("open() error\r\n");
        return NULL;
    }

    p = mmap(NULL, size, PROT_READ | PROT_WRITE
                    , MAP_PRIVATE, fd, 0);

     close(fd) ;

    if (p == MAP_FAILED)  {
        printf("mmap() error\r\n");
        return NULL;
    } else {
        printf("mmap_dev_zero: mamp() addr=%p\r\n", p);
        return p;
    }
}

void *malloc_example(int size)
{
    char *p;

    p = malloc(size);
    if (!p) {
        printf("malloc() error\r\n");
        return NULL;
    } else {
        printf("malloc() addr=%p\r\n", p);
        return p;
    }
}

int main()
{
    void *p1, *p2, *p3;
    int psize;
    size_t size;

    p1 = mmap_anoymous(512*1024);

    psize = getpagesize();  ///4KB
    printf("page size = %d\r\n", psize);

    p2 = mmap_dev_zero(psize);

    p3 = malloc_example(8*1024);
    size = malloc_usable_size(p3);
    printf("malloc usable size = %d\r\n", size);

    ///...

    while(1) pause();

    munmap(p1, 512*1024);
    munmap(p2, psize);
    free(p3);

    return 0;
}
