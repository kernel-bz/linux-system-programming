//
//	Book: Advanced Programming in UNIX
//  Chapter 11. Threads
//	
//	Editted by: Jung,JaeJoon (rgbi3307@nate.com)
//	Web Site:	http://www.kernel.kr/
//  Date:		2009-07
//

//������ ����ȭ

//#include <pthread.h>

//������ ���ؽ� �ʱ�ȭ �Լ�
//���ؽ��� �⺻���� �Ӽ����� �ʱ�ȭ��, attr�� NULL�� ����.
/*
int pthread_mutex_init (pthread_mutex_t *restrict mutex,
						const pthread_mutexattr_t *restrict attr);
*/

//���� �Ҵ�(������)
//pthread_mutex_t a_mutex = PTHREAD_MUTEX_INITIALIZER;


//������ ���ؽ� ����
//���ؽ��� �������� �Ҵ�(malloc)�������� �Ʒ� �Լ��� �����ؾ� ��.
//int pthread_mutex_destroy (pthread_mutex_t *mutex);


//���ؽ� ���(lock)
//����, ���ؽ��� �̹� ���(lock)�ִٸ�, �ش� ���ؽ��� Ǯ������(unlock)���� ȣ�� ������� ���(block)�ȴ�.

//int pthread_mutex_lock (pthread_mutex_t *mutex);

//����, �����尡 ����� ������ ���ٸ�, ���ؽ��� ���������� ��� �� �ֵ��� �Ʒ� �Լ� ���.
//pthread_mutex_trylock�� ȣ��Ǵ� ������ ���ؽ��� ��� ���� ������,
//���ؽ��� ��װ�, blocking�� �������� �ʴ´�. 
//�ݸ鿡, ���ؽ��� �̹� ���(lock)�ִٸ�, pthread_mutex_trylock�� �����ϰ�,
//EBUSY�� ��ȯ�ϸ鼭 ���ؽ��� ����� �ʴ´�.
//int pthread_mutex_trylock (pthread_mutex_t *mutex);


//���ؽ� �������(unlock)
//int pthread_mutex_unlock (pthread_mutex_t *mutex);

#include <stdlib.h>
#include <pthread.h>

struct foo {
    int             f_count;
    pthread_mutex_t f_lock;
    /* ... more stuff here ... */
};

struct foo *foo_alloc (void) /* allocate the object */
{
    struct foo *fp;

    if ((fp = malloc(sizeof(struct foo))) != NULL) {
        fp->f_count = 1;
        if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
            free(fp);
            return(NULL);
        }
        /* ... continue initialization ... */
    }
    return(fp);
}

void foo_hold (struct foo *fp) /* add a reference to the object */
{
    pthread_mutex_lock(&fp->f_lock);
    fp->f_count++;
    pthread_mutex_unlock(&fp->f_lock);
}


void foo_rele (struct foo *fp) /* release a reference to the object */
{
    pthread_mutex_lock(&fp->f_lock);

	//last reference (��� �����尡 ����� �Ϸ��ߴ��� Ȯ����, �޸�����)
    if (--fp->f_count == 0) { 
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_destroy(&fp->f_lock);
        free(fp);
    } else {
        pthread_mutex_unlock(&fp->f_lock);
    }
}
