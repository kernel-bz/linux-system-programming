//
//	file name	: th02.c
//	author		: Jung,JaeJoon(rgbi3307@nate.com) on the www.kernel.bz
//	comments	: thread exit
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void *thr_fn1 (void *arg)
{
	int i;
	for (i = 0; i < 10; i++) {
    		printf ("thread 1 is running(%d)\n", i);
		sleep(1);
	}
    	return ((void *)1);
}

void *thr_fn2 (void *arg)
{
	int i;
	for (i = 0; i < 20; i++) {
    		printf("thread 2 is running(%d)\n", i);
		sleep(1);
	}
    	pthread_exit ((void *)2);
}

int main (void)
{
	int         err;
	pthread_t   tid1, tid2;
	void        *tret = (void*)0;

	err = pthread_create(&tid1, NULL, thr_fn1, NULL);
	if (err)
		printf("can't create thread 1: %s\n", strerror(err));
	else
		printf("tid1=%lu\n", tid1);

	err = pthread_create(&tid2, NULL, thr_fn2, NULL);
	if (err)
		printf("can't create thread 2: %s\n", strerror(err));
	else
		printf("tid2=%lu\n", tid2);

	//err = pthread_cancel(tid1);		//terminate immedately
	//err = pthread_join(tid1, &tret);	//wait for termination
	err = pthread_detach(tid1);	///free immediately when it terminates
	if (err)
		printf("can't join with thread 1: %s\n", strerror(err));
	else
		printf("thread 1(%lu) exit code=%d\n", tid1, (int)tret);

	//err = pthread_cancel(tid2);		//terminate immedately
	//err = pthread_join(tid2, &tret);	//wait for termination
	err = pthread_detach(tid2);	///free immediately when it terminates
	if (err)
		printf("can't join with thread 2: %s\n", strerror(err));
	else
		printf("thread 2(%lu) exit code=%d\n", tid2, (int)tret);

	//exit(0);
	while(1) pause();
}
