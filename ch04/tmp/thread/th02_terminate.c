//
//	Book: Advanced Programming in UNIX
//  Chapter 11. Threads
//	
//	Editted by: Jung,JaeJoon (rgbi3307@nate.com)
//	Web Site:	http://www.kernel.kr/
//  Date:		2009-07
//

//#include <pthread.h>

//���� �����尡 ����Ǵ� ������ ���
//1. ������� ���۷�ƾ���� ������ ��ȯ�ȴ�. ��ȯ���� �������� exit �ڵ��̴�.
//2. ������� ���� ���μ��������� �ٸ� �����忡 ���� ��ҵ� �� �ִ�.
//3. ������� pthread_exit�� ȣ���� �� �ִ�.
//-----------------------------------------------------------------------------
//void pthread_exit (void *rval_ptr);

//rval_ptr�� ���ϰ�, ���ɾ��ٸ� NULL�� ����.

//�����带 �ڵ������� �и���(detached) ���·� ����.
//�ڿ��� ȸ���� �� �ְ� ��.
//�����尡 �̹� �и��� �����̸� ����(EINVAL)�� ��ȯ��
//�����尡 ��ҵǸ� rval_ptr�� ���� ������ �޸� ��ġ�� PTHREAD_CANCELED�� ������.
//����: 0
//����: ������ȣ
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

/* ������

$ ./a.out
    thread 1 returning
    thread 2 exiting
    thread 1 exit code 1
    thread 2 exit code 2

//�����尡 ���� ��ƾ���� �ܼ��ϰ� ��ȯ�ɶ���, pthread_exit�� ���ؼ� ����ɶ�,
//���� ���´� pthread_join�� ���� �Ǵٸ� �����忡 ���ؼ� ���� �� �ִ�.
*/


