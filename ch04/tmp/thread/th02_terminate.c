//
//	Book: Advanced Programming in UNIX
//  Chapter 11. Threads
//	
//	Editted by: Jung,JaeJoon (rgbi3307@nate.com)
//	Web Site:	http://www.kernel.kr/
//  Date:		2009-07
//

//#include <pthread.h>

//단일 쓰레드가 종료되는 세가지 방법
//1. 쓰레드는 시작루틴에서 간단히 반환된다. 반환값은 쓰레드의 exit 코드이다.
//2. 쓰레드는 같은 프로세스내에서 다른 쓰레드에 의해 취소될 수 있다.
//3. 쓰레드는 pthread_exit를 호출할 수 있다.
//-----------------------------------------------------------------------------
//void pthread_exit (void *rval_ptr);

//rval_ptr는 리턴값, 관심없다면 NULL로 설정.

//쓰레드를 자동적으로 분리된(detached) 상태로 만듬.
//자원을 회수할 수 있게 됨.
//쓰레드가 이미 분리된 상태이면 실패(EINVAL)를 반환함
//쓰레드가 취소되면 rval_ptr에 의해 지정된 메모리 위치는 PTHREAD_CANCELED로 설정됨.
//성공: 0
//실패: 오류번호
//int pthread_join(pthread_t thread, void **rval_ptr);


#include "apue.h"
#include <pthread.h>

void *thr_fn1 (void *arg)
{
    printf ("thread 1 returning\n");
    return ((void *)1);
}

void *thr_fn2 (void *arg)
{
    printf("thread 2 exiting\n");
    pthread_exit ((void *)2);
}

int main (void)
{
    int         err;
    pthread_t   tid1, tid2;
    void        *tret;

    err = pthread_create(&tid1, NULL, thr_fn1, NULL);
    if (err != 0)
        err_quit("can't create thread 1: %s\n", strerror(err));

    err = pthread_create(&tid2, NULL, thr_fn2, NULL);
    if (err != 0)
        err_quit("can't create thread 2: %s\n", strerror(err));

    err = pthread_join(tid1, &tret);
    if (err != 0)
        err_quit("can't join with thread 1: %s\n", strerror(err));

    printf("thread 1 exit code %d\n", (int)tret);

    err = pthread_join(tid2, &tret);
    if (err != 0)
        err_quit("can't join with thread 2: %s\n", strerror(err));

    printf("thread 2 exit code %d\n", (int)tret);

    exit(0);
}

/* 실행결과

$ ./a.out
    thread 1 returning
    thread 2 exiting
    thread 1 exit code 1
    thread 2 exit code 2

//쓰레드가 시작 루틴에서 단순하게 반환될때나, pthread_exit에 의해서 종료될때,
//종료 상태는 pthread_join에 의한 또다른 쓰레드에 의해서 취득될 수 있다.
*/


