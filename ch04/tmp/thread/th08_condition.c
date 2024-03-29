//
//	Book: Advanced Programming in UNIX
//  Chapter 11. Threads
//	
//	Editted by: Jung,JaeJoon (rgbi3307@nate.com)
//	Web Site:	http://www.kernel.kr/
//  Date:		2009-07
//


//Condition Variables
//-----------------------------------------------------------------------------

/*
#include <pthread.h>

//정적할당: PTHREAD_COND_INITIALIZER

//동적할당:
int pthread_cond_init (pthread_cond_t *restrict cond
					   , pthread_condattr_t *restrict attr);

int pthread_cond_destroy (pthread_cond_t *cond);


int pthread_cond_wait (pthread_cond_t *restrict cond
					  , pthread_mutex_t *restrict mutex);

int pthread_cond_timedwait (pthread_cond_t *restrict cond
						   , pthread_mutex_t *restrict mutex
						   , const struct timespec *restrict timeout);

int pthread_cond_signal(pthread_cond_t *cond);

int pthread_cond_broadcast(pthread_cond_t *cond);

*/


//Figure 11.14. Using condition variables
//-----------------------------------------------------------------------------
#include <pthread.h>

struct msg {
    struct msg *m_next;
    /* ... more stuff here ... */
};
struct msg *workq;
pthread_cond_t qready = PTHREAD_COND_INITIALIZER;
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

void process_msg (void)
{
    struct msg *mp;

    for (;;) {
        pthread_mutex_lock(&qlock);
        while (workq == NULL)
            pthread_cond_wait(&qready, &qlock);
        mp = workq;
        workq = mp->m_next;
        pthread_mutex_unlock(&qlock);
        /* now process the message mp */
    }
}

void enqueue_msg(struct msg *mp)
{
    pthread_mutex_lock(&qlock);
    mp->m_next = workq;
    workq = mp;
    pthread_mutex_unlock(&qlock);
    pthread_cond_signal(&qready);
}

