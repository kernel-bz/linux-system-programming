//
//	file name	: th01_create.c
//	author		: Jung,JaeJoon(rgbi3307@nate.com) on the www.kernel.bz
//	comments	: thread create
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void printids (const char *s)
{
    pid_t      pid;
    pthread_t  tid;

    pid = getpid ();
    tid = pthread_self ();
    printf ("%s pid=%u, tid=%lu\n", s, pid, tid);
}

void *thr_fn (void *arg)
{
    printids ("new thread: ");

    while(1) pause();

    return ((void *)0);
}

int main(void)
{
    int err;
    pthread_t tidp;

    err = pthread_create (&tidp, NULL, thr_fn, NULL);
    if (err) {
        printf("can't create thread: %s\n", strerror(err));
		exit(-1);
    } else printf("tidp = %lu\n", tidp);

    printids ("main thread:");
    sleep (1);

    //exit (0);

    while(1) pause();
}
