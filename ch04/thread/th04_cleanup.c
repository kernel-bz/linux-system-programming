//
//	file name	: th04.c
//	Editted by: Jung,JaeJoon (rgbi3307@nate.com) on the www.kernel.bz
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void cleanup (void *arg)
{
    printf("cleanup: %s\n", (char *)arg);
}

void hello (void *arg)
{
    printf("hello: Bood-Bye: %s\n", (char *)arg);
}

void *thr_fn1 (void *arg)
{
    printf("thread 1 is starting\n");
    pthread_cleanup_push(cleanup, "thread 1 first handler");
    pthread_cleanup_push(cleanup, "thread 1 second handler");
    pthread_cleanup_push(hello, "thread 1 third handler");
    pthread_cleanup_push(hello, "thread 1 fourth handler");
    printf("thread 1 push complete\n");

    /**
    if (arg)
        return((void *)1);
    */
    pthread_cleanup_pop(1);     ///hello(): fourth
    pthread_cleanup_pop(2);     ///hello(): third
    pthread_cleanup_pop(0);     ///cleanup(): second (not execute)
    pthread_cleanup_pop(0);     ///cleanup(): first (not execute)

    return((void *)1);
}

void *thr_fn2 (void *arg)
{
    printf("thread 2 is starting\n");
    pthread_cleanup_push(cleanup, "thread 2 first handler");
    pthread_cleanup_push(cleanup, "thread 2 second handler");

    printf("thread 2 push complete. arg=%d\n", (int)arg);

    /**
    if (arg)
        pthread_exit((void *)2);
    */
    pthread_cleanup_pop(1); ///cleanup(): second
    pthread_cleanup_pop(1); ///cleanup(): first

    pthread_exit((void *)2);
}

int main (void)
{
    int         err;
    pthread_t   tid1, tid2;
    void        *tret;

    ///err = pthread_create (&tid1, NULL, thr_fn1, (void *)1);
    err = pthread_create (&tid1, NULL, thr_fn1, NULL);
    if (err)
        printf("can't create thread 1: %s\n", strerror(err));

    err = pthread_create (&tid2, NULL, thr_fn2, (void *)2);
    ///err = pthread_create (&tid2, NULL, thr_fn2, NULL);
    if (err)
        printf("can't create thread 2: %s\n", strerror(err));

    //err = pthread_cancel (tid1);
    err = pthread_join (tid1, &tret);
    //err = pthread_detach(tid1);
    if (err)
        printf("can't join with thread 1: %s\n", strerror(err));
    else
        printf("thread 1 exit code %d\n", (int)tret);

    //err = pthread_cancel(tid2);
    err = pthread_join (tid2, &tret);
	//err = pthread_detach(tid2);
    if (err)
        printf("can't join with thread 2: %s\n", strerror(err));
    else
        printf("thread 2 exit code %d\n", (int)tret);

    while(1) pause();
    exit(0);
}
