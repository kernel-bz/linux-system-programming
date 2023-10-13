//
//	file name	: th05.c
//	author		: Jung,JaeJoon (rgbi3307@nate.com) on the www.kernel.bz
//	comments	: pthread mutex
//  editted by  :
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

struct foo {
    int             f_count;
    pthread_mutex_t f_lock;
    //more stuff here ...
};

///allocate
struct foo *foo_alloc (void)
{
    struct foo *fp;

    int size = sizeof(struct foo);
    printf("struct foo size=%d\n", size);

    if ((fp = malloc(sizeof(struct foo))) != NULL) {
        printf("struct foo address=%p\n", fp);
        fp->f_count = 1;
        if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
            free(fp);
            return(NULL);
        }
        ///... continue initialization ...
    }
    return(fp);
}

///release
void foo_free (struct foo *fp)
{
	pthread_mutex_destroy(&fp->f_lock);
	free(fp);
	printf("foo released.\n");
}

///hold for plus
void foo_hold_plus (struct foo *fp)
{
	if(!fp) return;

    	fp->f_count++;

	//printf("thread=%lu, count=%d\n", pthread_self(), fp->f_count);
	///if (fp->f_count==0) foo_free(fp);
}

///hold for minus
void foo_hold_minus (struct foo *fp)
{
	if(!fp) return;

    	fp->f_count--;

	//printf("thread=%lu, count=%d\n", pthread_self(), fp->f_count);
	///if (fp->f_count==0) foo_free(fp);
}

void *thr_fn1(void *arg)
{
    struct foo *fp = (struct foo *)arg;
    while(1) {
        pthread_mutex_lock(&fp->f_lock);    ///Locking

        foo_hold_plus(fp);  ///fp->f_count++; 2
        sleep(1);
        foo_hold_minus(fp); ///fp->f_count--; 1

        printf("thread1=%lu, f_count=%d\n", pthread_self(), fp->f_count);

        pthread_mutex_unlock(&fp->f_lock);  ///Unlocking

        sleep(1);
    }

    return ((void*)0);
}

void *thr_fn2(void *arg)
{
    struct foo *fp = (struct foo *)arg;
    while(1) {
        pthread_mutex_lock(&fp->f_lock);    ///Locking

        foo_hold_plus(fp);  ///fp->f_count++; 2
        sleep(2);
        foo_hold_minus(fp); ///fp->f_count--; 1

        printf("thread2=%lu, f_count=%d\n", pthread_self(), fp->f_count);

        pthread_mutex_unlock(&fp->f_lock);  ///Unlocking

        sleep(1);
    }
    return ((void*)0);
}

int main(void)
{
	int err;
	pthread_t tid1, tid2;
	struct foo *fp;

	fp = foo_alloc();

	err = pthread_create(&tid1, NULL, thr_fn1, fp);
	if(!err) printf("thread1 starting = %lu\n", tid1);

	err = pthread_create(&tid2, NULL, thr_fn2, fp);
	if(!err) printf("thread2 starting = %lu\n", tid2);

	pthread_detach(tid1);
	pthread_detach(tid2);

	while(1) pause();

	foo_free(fp);
	return 0;
}
