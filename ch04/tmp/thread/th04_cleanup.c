//
//	Book: Advanced Programming in UNIX
//  Chapter 11. Threads
//	
//	Editted by: Jung,JaeJoon (rgbi3307@nate.com)
//	Web Site:	http://www.kernel.kr/
//  Date:		2009-07
//

//pthread_cancel은 pthread_exit의 인수에 PTHREAD_CANCELED이 사용된것과 같은 역할.
//pthread_cancel은 쓰레드가 종료될때까지 대기하지 않는다라는 것을 기억해 두자.
//반환값: 성공시 0, 실패시 오류숫자

//int pthread_cancel (pthread_t tid);

//쓰레드는 종료될때 호출되는 함수들을 배열(정렬)할 수 있다.
//이함수는 thread cleanup handlers로 알려져 있으며,
//프로세스가 종료될때 호출되는 atexit 함수와도 유사하다.
//하나의 쓰레드마다 여러개의 cleanup handlers가 있을 수 있으며,
//이 핸들러들은 스택에 보관되기 때문에, 요청된 역순으로 실행된다.

/*
void pthread_cleanup_push (void (*rtn)(void *), void *arg);
void pthread_cleanup_pop (int execute);
*/

//pthread_cleanup_push 함수는 rtn 함수를 스케쥴하고,
//하나의 인수인 arg와 함께, 아래와 같이 수행된다.
//(1)pthread_exit를 호출한다.
//(2)취소 요구에 응답한다.
//(3)0이 아닌 execute 인수를 사용하여 pthread_cleanup_pop를 호출한다.

//만일, execute 인수가 0으로 설정되어 있으면, cleanup 함수는 호출되지 않는다.
//pthread_cleanup_pop은 pthread_cleanup_push에 의해서 최근에 설정된 cleanup handler를 제거한다.

#include <pthread.h>

void cleanup (void *arg)
{
    printf("cleanup: %s\n", (char *)arg);
}

void *thr_fn1 (void *arg)
{
    printf("thread 1 start\n");
    pthread_cleanup_push(cleanup, "thread 1 first handler");
    pthread_cleanup_push(cleanup, "thread 1 second handler");
    printf("thread 1 push complete\n");
    if (arg)
        return((void *)1);
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    return((void *)1);
}

void *thr_fn2 (void *arg)
{
    printf("thread 2 start\n");
    pthread_cleanup_push(cleanup, "thread 2 first handler");
    pthread_cleanup_push(cleanup, "thread 2 second handler");
    printf("thread 2 push complete\n");
    if (arg)
        pthread_exit((void *)2);
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    pthread_exit((void *)2);
}

int main (void)
{
    int         err;
    pthread_t   tid1, tid2;
    void        *tret;

    err = pthread_create (&tid1, NULL, thr_fn1, (void *)1);
    if (err != 0)
        err_quit("can't create thread 1: %s\n", strerror(err));

    err = pthread_create (&tid2, NULL, thr_fn2, (void *)1);
    if (err != 0)
        err_quit("can't create thread 2: %s\n", strerror(err));

    err = pthread_join (tid1, &tret);
      if (err != 0)
        err_quit("can't join with thread 1: %s\n", strerror(err));
    printf("thread 1 exit code %d\n", (int)tret);

    err = pthread_join (tid2, &tret);
    if (err != 0)
        err_quit("can't join with thread 2: %s\n", strerror(err));
    printf("thread 2 exit code %d\n", (int)tret);

    exit(0);
}

/* 실행결과

//pthread_exit 함수에 의해서 쓰레드가 종료될때, cleanup 핸들러가 호출된다.
$ ./a.out
    thread 1 start
    thread 1 push complete
    thread 2 start
    thread 2 push complete
    cleanup: thread 2 second handler
    cleanup: thread 2 first handler
    thread 1 exit code 1
    thread 2 exit code 2

*/


//기본적으로, 쓰레드의 종료 상태는 pthread_join이 호출될때 까지 유보된다.
//쓰레드가 분리될때, pthread_join 함수는 종료상태를 보기위해 대기할 수 없다.
//쓰레드를 분리하기 위한 pthread_join 함수 호출이 실패한다면, EINVAL이 반환된다.
//pthread_detach 함수는 쓰레드를 분리시킬 수 있다.

//반환값: 성공시 0, 실패시 오류번호
//int pthread_detach(pthread_t tid);

