//
//	Book: Advanced Programming in UNIX
//  Chapter 11. Threads
//	
//	Editted by: Jung,JaeJoon (rgbi3307@nate.com)
//	Web Site:	http://www.kernel.kr/
//  Date:		2009-07
//

//pthread_cancel�� pthread_exit�� �μ��� PTHREAD_CANCELED�� ���ȰͰ� ���� ����.
//pthread_cancel�� �����尡 ����ɶ����� ������� �ʴ´ٶ�� ���� ����� ����.
//��ȯ��: ������ 0, ���н� ��������

//int pthread_cancel (pthread_t tid);

//������� ����ɶ� ȣ��Ǵ� �Լ����� �迭(����)�� �� �ִ�.
//���Լ��� thread cleanup handlers�� �˷��� ������,
//���μ����� ����ɶ� ȣ��Ǵ� atexit �Լ��͵� �����ϴ�.
//�ϳ��� �����帶�� �������� cleanup handlers�� ���� �� ������,
//�� �ڵ鷯���� ���ÿ� �����Ǳ� ������, ��û�� �������� ����ȴ�.

/*
void pthread_cleanup_push (void (*rtn)(void *), void *arg);
void pthread_cleanup_pop (int execute);
*/

//pthread_cleanup_push �Լ��� rtn �Լ��� �������ϰ�,
//�ϳ��� �μ��� arg�� �Բ�, �Ʒ��� ���� ����ȴ�.
//(1)pthread_exit�� ȣ���Ѵ�.
//(2)��� �䱸�� �����Ѵ�.
//(3)0�� �ƴ� execute �μ��� ����Ͽ� pthread_cleanup_pop�� ȣ���Ѵ�.

//����, execute �μ��� 0���� �����Ǿ� ������, cleanup �Լ��� ȣ����� �ʴ´�.
//pthread_cleanup_pop�� pthread_cleanup_push�� ���ؼ� �ֱٿ� ������ cleanup handler�� �����Ѵ�.

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

/* ������

//pthread_exit �Լ��� ���ؼ� �����尡 ����ɶ�, cleanup �ڵ鷯�� ȣ��ȴ�.
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


//�⺻������, �������� ���� ���´� pthread_join�� ȣ��ɶ� ���� �����ȴ�.
//�����尡 �и��ɶ�, pthread_join �Լ��� ������¸� �������� ����� �� ����.
//�����带 �и��ϱ� ���� pthread_join �Լ� ȣ���� �����Ѵٸ�, EINVAL�� ��ȯ�ȴ�.
//pthread_detach �Լ��� �����带 �и���ų �� �ִ�.

//��ȯ��: ������ 0, ���н� ������ȣ
//int pthread_detach(pthread_t tid);

