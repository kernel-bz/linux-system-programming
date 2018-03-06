//
//	Book: Advanced Programming in UNIX
//  Chapter 11. Threads
//
//	Editted by: Jung,JaeJoon (rgbi3307@nate.com)
//	Web Site:	http://www.kernel.kr/
//  Date:		2009-07
//

#include <pthread.h>

//쓰레드 ID 상호 비교
//반환값: 같다면 0이 아니값, 다르다면 0
//-----------------------------------------------------------------------------
//int pthread_equal (pthread_t tid1, pthread_t tid2);

//쓰레드 ID 값 가져오기
//반환값: 호출된 쓰레드의 쓰레드 ID
//-----------------------------------------------------------------------------
//pthread_t pthread_self (void);

//쓰레드 생성
//반환값: 성공시 0, 실패시 오류번호
//포인터 tidp가 가리키는 메모리 위치에는 새롭게 생성된 쓰레드 ID가 설정되며,
//이때 pthread_create는 성공적으로 수행한 결과를 반환한다.
//attr 인자는 다양한 쓰레드 속성들을 설정하는데 사용한다.
//NULL로 설정하여 기본 속성으로 쓰레드를 생성 하도록 한다.
//새롭게 생성된 쓰레드는 start_rtn 함수의 주소에서 실행을 시작한다.
//이 함수는 arg라는 하나의 인수를 가지는데, 이 인수는 데이터 형식이 없는(typeless) 포인터이다.
//start_rtn 함수에게 한개 이상의 인수를 전달하고자 한다면,
//인수들의 데이터를 구조체로 저장해서 arg에게 구조체 주소를 넘겨준다.
//-----------------------------------------------------------------------------
/*
int pthread_create (pthread_t *restrict tidp
				   , const pthread_attr_t *restrict attr
				   , void *(*start_rtn)(void)
				   , void *restrict arg);
*/

#include "apue.h"

pthread_t ntid;

void printids (const char *s)
{
    pid_t      pid;
    pthread_t  tid;

    pid = getpid ();
    tid = pthread_self ();
    printf ("%s pid %u tid %u (0x%x)\n", s, (unsigned int)pid,
				(unsigned int)tid, (unsigned int)tid);
}

void *thr_fn (void *arg)
{
    printids ("new thread: ");
    return ((void *)0);
}

int main(void)
{
    int     err;

    err = pthread_create (&ntid, NULL, thr_fn, NULL);
    if (err != 0)
        err_quit("can't create thread: %s\n", strerror(err));
    printids ("main thread:");
    sleep (1);
    exit (0);
}

/*
//Solaris 에서 실행결과

$ ./a.out
    main thread: pid 7225 tid 1 (0x1)
    new thread:  pid 7225 tid 4 (0x4)

//FreeBSD 에서 실행결과

$ ./a.out
    main thread: pid 14954 tid 134529024 (0x804c000)
    new thread:  pid 14954 tid 134530048 (0x804c400)

//Mac OS X에서 실행결과

$ ./a.out
    main thread: pid 779 tid 2684396012 (0xa000a1ec)
    new thread:  pid 779 tid 25166336 (0x1800200)

//리눅스에서 실행결과 (clone system call, 프로세스 id가 달라짐)
//new thread가 먼저 출력됨

$ ./a.out
    new thread:  pid 6628 tid 1026 (0x402)
    main thread: pid 6626 tid 1024 (0x400)

*/

