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

//���ؽ��� �ΰ��� ����(locked Ȥ�� unlocked)��, �ϳ��� �����尡 �ѹ����.
//readerwriter ���� ������ ���� ����:
//locked in read mode, locked in write mode, and unlocked
//write mode(exclusive mode)������ �ѹ��� �ϳ��� �����常�� readerwriter lock�� ����(hold) �� ������,
//read mode(shared mode)������ ���ÿ� �������� �����尡 readerwriter lock�� ����(hold) �� �ִ�.

/*
//readerwriter lock �ʱ�ȭ

int pthread_rwlock_init (pthread_rwlock_t *restrict rwlock
						, const pthread_rwlockattr_t *restrict attr);

//�ڿ��Ҵ� ����
int pthread_rwlock_destroy (pthread_rwlock_t *rwlock);

���ϰ�: ������ 0, ���н� ������ȣ
*/
 
/*
//read lock
int pthread_rwlock_rdlock (pthread_rwlock_t *rwlock);

//write lock
int pthread_rwlock_wrlock (pthread_rwlock_t *rwlock);

//Regardless 
int pthread_rwlock_unlock (pthread_rwlock_t *rwlock);

���ϰ�: ������ 0, ���н� ������ȣ
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

