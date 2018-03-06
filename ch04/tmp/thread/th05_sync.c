//
//	Book: Advanced Programming in UNIX
//  Chapter 11. Threads
//	
//	Editted by: Jung,JaeJoon (rgbi3307@nate.com)
//	Web Site:	http://www.kernel.kr/
//  Date:		2009-07
//

//쓰레드 동기화

//#include <pthread.h>

//쓰레드 뮤텍스 초기화 함수
//뮤텍스를 기본적인 속성으로 초기화시, attr에 NULL을 전달.
/*
int pthread_mutex_init (pthread_mutex_t *restrict mutex,
						const pthread_mutexattr_t *restrict attr);
*/

//정적 할당(상수사용)
//pthread_mutex_t a_mutex = PTHREAD_MUTEX_INITIALIZER;


//쓰레드 뮤텍스 종료
//뮤텍스를 동적으로 할당(malloc)했을때는 아래 함수로 해제해야 함.
//int pthread_mutex_destroy (pthread_mutex_t *mutex);


//뮤텍스 잠금(lock)
//만일, 뮤텍스가 이미 잠겨(lock)있다면, 해당 뮤텍스가 풀려날때(unlock)까지 호출 쓰레드는 블락(block)된다.

//int pthread_mutex_lock (pthread_mutex_t *mutex);

//만일, 쓰레드가 블락될 여유가 없다면, 뮤텍스를 조건적으로 잠글 수 있도록 아래 함수 사용.
//pthread_mutex_trylock이 호출되는 시점에 뮤텍스가 잠겨 있지 않으면,
//뮤텍스를 잠그고, blocking은 발행하지 않는다. 
//반면에, 뮤텍스가 이미 잠겨(lock)있다면, pthread_mutex_trylock은 실패하고,
//EBUSY를 반환하면서 뮤텍스를 잠그지 않는다.
//int pthread_mutex_trylock (pthread_mutex_t *mutex);


//뮤텍스 잠금해제(unlock)
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

	//last reference (모든 쓰레드가 사용을 완료했는지 확인후, 메모리해제)
    if (--fp->f_count == 0) { 
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_destroy(&fp->f_lock);
        free(fp);
    } else {
        pthread_mutex_unlock(&fp->f_lock);
    }
}
