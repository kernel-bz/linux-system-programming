//
//	Book: Advanced Programming in UNIX
//  Chapter 11. Threads
//	
//	Editted by: Jung,JaeJoon (rgbi3307@nate.com)
//	Web Site:	http://www.kernel.kr/
//  Date:		2009-07
//

//pthread_exit�� �Ķ���� �����Ҷ�, �ڵ�����(���û��) ����� ������.
//�Ű������� ����ü�� ����Ҷ�, ȥ���� �������� �ذ��ϱ� ����
//����ü�� ���������� ����ϰų�, malloc�� ����Ͽ� ����ü�� �Ҵ��Ѵ�.

#include "apue.h"
#include <pthread.h>

struct foo {
    int a, b, c, d;
};

void printfoo (const char *s, const struct foo *fp)
{
    printf(s);
    printf("  structure at 0x%x\n", (unsigned)fp);
    printf("  foo.a = %d\n", fp->a);
    printf("  foo.b = %d\n", fp->b);
    printf("  foo.c = %d\n", fp->c);
    printf("  foo.d = %d\n", fp->d);
}

void *thr_fn1 (void *arg)
{

    struct foo  foo = {1, 2, 3, 4};

    printfoo ("thread 1:\n", &foo);
    pthread_exit ((void *)&foo);
}

void *thr_fn2 (void *arg)
{
    printf("thread 2: ID is %d\n", pthread_self());
    pthread_exit((void *)0);
}

int main (void)
{
    int         err;
    pthread_t   tid1, tid2;
    struct foo  *fp;

    err = pthread_create (&tid1, NULL, thr_fn1, NULL);
    if (err != 0)
        err_quit("can't create thread 1: %s\n", strerror(err));
    err = pthread_join (tid1, (void *)&fp);
    if (err != 0)
        err_quit("can't join with thread 1: %s\n", strerror(err));

    sleep(1);
    printf("parent starting second thread\n");
    err = pthread_create (&tid2, NULL, thr_fn2, NULL);
    if (err != 0)
        err_quit("can't create thread 2: %s\n", strerror(err));

    sleep(1);
    printfoo ("parent:\n", fp);
    exit(0);
}

/* 
//���������� ������
$ ./a.out
   thread 1:
     structure at 0x409a2abc
     foo.a = 1
     foo.b = 2
     foo.c = 3
     foo.d = 4
   parent starting second thread
   thread 2: ID is 32770
   parent:
     structure at 0x409a2abc
     foo.a = 0
     foo.b = 32770
     foo.c = 1075430560
     foo.d = 1073937284

//�������� �޸� ����, �����Ϸ�, ������ ���̺귯�� ����� ���� �޶���
//FreeBSD���� ������
$ ./a.out
   thread 1:
     structure at 0xbfafefc0
     foo.a = 1
     foo.b = 2
     foo.c = 3
     foo.d = 4
   parent starting second thread
   thread 2: ID is 134534144
   parent:
     structure at 0xbfafefc0
     foo.a = 0
     foo.b = 134534144
     foo.c = 3
     foo.d = 671642590


*/
