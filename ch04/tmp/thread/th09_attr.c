//
//	Book: Advanced Programming in UNIX
//  Chapter 12. Thread Control
//	
//	Editted by: Jung,JaeJoon (rgbi3307@nate.com)
//	Web Site:	http://www.kernel.kr/
//  Date:		2009-07
//

/*
]$getconf _POSIX_THREAD_PROCESS_SHARED
]$getconf GNU_LIBPTHREAD_VERSION
]$sysconf PTHREAD_DESTRUCTOR_ITERATIONS
]$sysconf PTHREAD_KEYS_MAX
]$sysconf PTHREAD_STACK_MIN
]$sysconf PTHREAD_THREADS_MAX
*/

//Thread Attributes
//-----------------------------------------------------------------------------

/*
#include <pthread.h>

//������ �Ӽ� ����ü pthread_attr_t �ʱ�ȭ
int pthread_attr_init (pthread_attr_t *attr);

//������ �Ӽ� ����ü �ʱ�ȭ ����(�Ҵ�� �޸� ����)
int pthread_attr_destroy (pthread_attr_t *attr);

//POSIX.1 thread attributes (pthread_attr_t ����ü)
.detachstate	//�и��� ������ �Ӽ�
.guardsize:		//������ ���� ���� �ִ� guard buffer size(bytes)
.stackaddr;		//������ ������ ���� ���� �ּ�
.stacksize:		//������ ���� ũ��(bytes)


int pthread_attr_getdetachstate (const pthread_attr_t *restrict attr, int *detachstate);

int pthread_attr_setdetachstate (pthread_attr_t *attr, int detachstate);

*/

//����: �и��� ���·� ������ ����
//-----------------------------------------------------------------------------

#include "apue.h"
#include <pthread.h>

int makethread (void *(*fn)(void *), void *arg)
{
    int             err;
    pthread_t       tid;
    pthread_attr_t  attr;

    err = pthread_attr_init (&attr);
    if (err != 0)
        return(err);
    err = pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
    if (err == 0)
        err = pthread_create (&tid, &attr, fn, arg);

    pthread_attr_destroy (&attr);
    return(err);
}


//������ ���� �Ӽ� ����
//-----------------------------------------------------------------------------

/*
int pthread_attr_getstack(const pthread_attr_t *restrict attr
						  , void **restrict stackaddr
						  , size_t *restrict stacksize);

int pthread_attr_setstack(const pthread_attr_t *attr
						  , void *stackaddr
						  , size_t *stacksize);

int pthread_attr_getstacksize(const pthread_attr_t *restrict attr
						  , size_t *restrict stacksize);

int pthread_attr_setstacksize(pthread_attr_t *attr
						  , size_t stacksize);

int pthread_attr_getguardsize(const pthread_attr_t *restrict attr
							  , size_t *restrict guardsize);

int pthread_attr_setguardsize(pthread_attr_t *attr
							  , size_t guardsize);

int pthread_getconcurrency(void);

int pthread_setconcurrency(int level);

*/

