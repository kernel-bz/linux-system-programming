//
//	Book: Advanced Programming in UNIX
//  Chapter 11. Threads
//
//	Editted by: Jung,JaeJoon (rgbi3307@nate.com)
//	Web Site:	http://www.kernel.kr/
//  Date:		2009-07
//

#include <pthread.h>

//������ ID ��ȣ ��
//��ȯ��: ���ٸ� 0�� �ƴϰ�, �ٸ��ٸ� 0
//-----------------------------------------------------------------------------
//int pthread_equal (pthread_t tid1, pthread_t tid2);

//������ ID �� ��������
//��ȯ��: ȣ��� �������� ������ ID
//-----------------------------------------------------------------------------
//pthread_t pthread_self (void);

//������ ����
//��ȯ��: ������ 0, ���н� ������ȣ
//������ tidp�� ����Ű�� �޸� ��ġ���� ���Ӱ� ������ ������ ID�� �����Ǹ�,
//�̶� pthread_create�� ���������� ������ ����� ��ȯ�Ѵ�.
//attr ���ڴ� �پ��� ������ �Ӽ����� �����ϴµ� ����Ѵ�.
//NULL�� �����Ͽ� �⺻ �Ӽ����� �����带 ���� �ϵ��� �Ѵ�.
//���Ӱ� ������ ������� start_rtn �Լ��� �ּҿ��� ������ �����Ѵ�.
//�� �Լ��� arg��� �ϳ��� �μ��� �����µ�, �� �μ��� ������ ������ ����(typeless) �������̴�.
//start_rtn �Լ����� �Ѱ� �̻��� �μ��� �����ϰ��� �Ѵٸ�,
//�μ����� �����͸� ����ü�� �����ؼ� arg���� ����ü �ּҸ� �Ѱ��ش�.
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
//Solaris ���� ������

$ ./a.out
    main thread: pid 7225 tid 1 (0x1)
    new thread:  pid 7225 tid 4 (0x4)

//FreeBSD ���� ������

$ ./a.out
    main thread: pid 14954 tid 134529024 (0x804c000)
    new thread:  pid 14954 tid 134530048 (0x804c400)

//Mac OS X���� ������

$ ./a.out
    main thread: pid 779 tid 2684396012 (0xa000a1ec)
    new thread:  pid 779 tid 25166336 (0x1800200)

//���������� ������ (clone system call, ���μ��� id�� �޶���)
//new thread�� ���� ��µ�

$ ./a.out
    new thread:  pid 6628 tid 1026 (0x402)
    main thread: pid 6626 tid 1024 (0x400)

*/

