//
//	file name	: th03_arg.c
//	author		: Jung,JaeJoon (rgbi3307@nate.com) on the www.kernel.bz
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

struct foo {
    int a, b, c, d;
};

void print_foo (const char *s, const struct foo *fp)
{
    puts(s);
    printf("  structure at 0x%X\n", (unsigned)fp);
    printf("  foo.a = %d\n", fp->a);
    printf("  foo.b = %d\n", fp->b);
    printf("  foo.c = %d\n", fp->c);
    printf("  foo.d = %d\n", fp->d);
}

void *thr_fn1 (void *arg)
{
    ///static struct foo  foo = {1, 2, 3, 4};
    struct foo *foo;

    foo = (struct foo *)arg;

    printf("thread 1: ID is %lu\n", pthread_self());
    print_foo ("thread 1:\n", foo);
    pthread_exit ((void *)foo);
    ///return ((void *)foo);
}

void *thr_fn2 (void *arg)
{
    printf("thread 2: ID is %lu\n", pthread_self());
    pthread_exit((void *)0);
}

int main (void)
{
    int         err;
    pthread_t   tid1, tid2;
    struct foo  foo = {10, 20, 30, 40};
    struct foo  *fp;
    void        *tret = (void*)0;

    fp = &foo;

    err = pthread_create (&tid1, NULL, thr_fn1, (void *)fp);
    if (err)
        printf("can't create thread 1: %s\n", strerror(err));

    err = pthread_create (&tid2, NULL, thr_fn2, NULL);
    if (err)
        printf("can't create thread 2: %s\n", strerror(err));

    err = pthread_join(tid1, &tret);
    ///err = pthread_detach(tid1);
    if (err)
        printf("can't detach with thread 1: %s\n", strerror(err));
    else
        printf("tret = %p\n", (struct foo *)tret);

    fp = (struct foo *)tret;

    err = pthread_detach(tid2);
    if (err)
        printf("can't detach with thread 2: %s\n", strerror(err));

    sleep(1);
    print_foo ("Parent:\n", fp);
    exit(0);
}
