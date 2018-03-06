//
//	Book: Advanced Programming in UNIX
//  Chapter 11. Threads
//	
//	Editted by: Jung,JaeJoon (rgbi3307@nate.com)
//	Web Site:	http://www.kernel.kr/
//  Date:		2009-07
//

//ReaderWriter Locks (sharedexclusive locks)
//-----------------------------------------------------------------------------

//뮤텍스는 두가지 상태(locked 혹은 unlocked)로, 하나의 쓰레드가 한번사용.
//readerwriter 락은 세가지 상태 가능:
//locked in read mode, locked in write mode, and unlocked
//write mode(exclusive mode)에서는 한번에 하나의 쓰레드만이 readerwriter lock을 잡을(hold) 수 있으나,
//read mode(shared mode)에서는 동시에 여러개의 쓰레드가 readerwriter lock을 잡을(hold) 수 있다.

/*
//readerwriter lock 초기화

int pthread_rwlock_init (pthread_rwlock_t *restrict rwlock
						, const pthread_rwlockattr_t *restrict attr);

//자원할당 해제
int pthread_rwlock_destroy (pthread_rwlock_t *rwlock);

리턴값: 성공시 0, 실패시 오류번호
*/
 
/*
//read lock
int pthread_rwlock_rdlock (pthread_rwlock_t *rwlock);

//write lock
int pthread_rwlock_wrlock (pthread_rwlock_t *rwlock);

//Regardless 
int pthread_rwlock_unlock (pthread_rwlock_t *rwlock);

리턴값: 성공시 0, 실패시 오류번호
*/

/*
int pthread_rwlock_tryrdlock (pthread_rwlock_t *rwlock);

int pthread_rwlock_trywrlock (pthread_rwlock_t *rwlock);
*/


//Figure 11.13. Using readerwriter locks
//-----------------------------------------------------------------------------
#include <stdlib.h>
#include <pthread.h>

struct job {
    struct job *j_next;
    struct job *j_prev;
    pthread_t   j_id;   /* tells which thread handles this job */
    /* ... more stuff here ... */
};

struct queue {
    struct job      *q_head;
    struct job      *q_tail;
    pthread_rwlock_t q_lock;
};

/*
* Initialize a queue.
*/
int queue_init (struct queue *qp)
{
    int err;

    qp->q_head = NULL;
    qp->q_tail = NULL;
    err = pthread_rwlock_init (&qp->q_lock, NULL);
    if (err != 0)
        return(err);

    /* ... continue initialization ... */

    return(0);
}

/*
 * Insert a job at the head of the queue.
 */
void job_insert (struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    jp->j_next = qp->q_head;
    jp->j_prev = NULL;
    if (qp->q_head != NULL)
        qp->q_head->j_prev = jp;
    else
        qp->q_tail = jp;      /* list was empty */
    qp->q_head = jp;
    pthread_rwlock_unlock(&qp->q_lock);
}

/*
 * Append a job on the tail of the queue.
 */
void job_append (struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    jp->j_next = NULL;
    jp->j_prev = qp->q_tail;
    if (qp->q_tail != NULL)
        qp->q_tail->j_next = jp;
    else
        qp->q_head = jp;   /* list was empty */
    qp->q_tail = jp;
    pthread_rwlock_unlock(&qp->q_lock);
}

/*
 * Remove the given job from a queue.
 */
void job_remove (struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    if (jp == qp->q_head) {
        qp->q_head = jp->j_next;
        if (qp->q_tail == jp)
            qp->q_tail = NULL;
    } else if (jp == qp->q_tail) {
        qp->q_tail = jp->j_prev;
        if (qp->q_head == jp)
            qp->q_head = NULL;
    } else {
        jp->j_prev->j_next = jp->j_next;
        jp->j_next->j_prev = jp->j_prev;
    }
    pthread_rwlock_unlock(&qp->q_lock);
}
/*
 * Find a job for the given thread ID.
 */
struct job *job_find (struct queue *qp, pthread_t id)
{
    struct job *jp;

    if (pthread_rwlock_rdlock(&qp->q_lock) != 0)
        return(NULL);

    for (jp = qp->q_head; jp != NULL; jp = jp->j_next)
         if (pthread_equal(jp->j_id, id))
             break;

    pthread_rwlock_unlock(&qp->q_lock);
    return(jp);
}

