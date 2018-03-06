//
//	file name	: ch0802_malloc.c
//	author		: Jung,JaeJoon(rgbi3307@nate.com) on the www.kernel.bz
//	comments	: mallinfo example
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
        printf("mamp() addr=%p\r\n", p);
        return p;
    }
}

int main()
{
     struct mallinfo m;
     void *p1, *p2;
     char *c;
     int i;

     //p1 = mmap_anoymous(512*1024);
     p1 = malloc(16);
     printf("malloc() addr=%p\r\n", p1);
     c = (char *)p1;
     for (i=0; i<16; i++) {
        printf("%c", *c++);
     }
     printf("\n\n");

     p2 = calloc(1, 16);
     printf("calloc() addr=%p\r\n", p2);
     c = (char *)p2;
     for (i=0; i<16; i++) {
        printf("%c", *c++);
     }
     printf("\n\n");

#if 0
     m = mallinfo();
     printf("mallinfo: ----------------------------\n");
     printf("%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",
                    m.arena,    /* non-mmapped space allocated from system */
                    m.ordblks,  /* number of free chunks */
                    m.smblks,   /* number of fastbin blocks */
                    m.hblks,    /* number of mmapped regions */
                    m.hblkhd,   /* space in mmapped regions */
                    m.usmblks,  /* maximum total allocated space */
                    m.fsmblks,  /* space available in freed fastbin blocks */
                    m.uordblks, /* total allocated(malloc) space */
                    m.fordblks, /* total free space */
                    m.keepcost /* top-most, releasable (via malloc_trim) space */
    );

    printf("\n");
    printf("malloc_stats: -------------------------\n");
    malloc_stats();
#endif // 0

    while(1) pause();

    free(p2);
    free(p1);
    //munmap(p1, 512*1024);

    return 0;
}
