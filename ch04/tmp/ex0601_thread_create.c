//build: gcc -o -lpthread

#include <pthread.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 

//pthread_mutex_t a_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t a_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP; //������ �����

 
//������ �Լ� 
void *t_function(void *data) 
{ 
    int id, rc; 
    int i = 0; 

	rc = pthread_mutex_lock(&a_mutex);

    id = *((int *)data); 
 
    while(1) 
    { 
        printf("%d : %d\n", id, i); 
        i++; 
        sleep(1); 
    } 

	rc = pthread_mutex_unlock(&a_mutex);
} 
 
int main() 
{ 
    pthread_t p_thread[2]; 
    int thr_id; 
    int status; 
    int a = 1; 
    int b = 2; 
 
    // ������ ���� �ƱԸ�Ʈ�� 1 �� �ѱ��.   
    thr_id = pthread_create (&p_thread[0], NULL, t_function, (void *)&a); 
    if (thr_id < 0) 
    { 
        perror("thread create error : "); 
        exit(0); 
    } 
 
    // ������ ���� �ƱԸ�Ʈ�� 2 �� �ѱ��.  
    thr_id = pthread_create (&p_thread[1], NULL, t_function, (void *)&b); 
    if (thr_id < 0) 
    { 
        perror("thread create error : "); 
        exit(0); 
    } 
 
    // ������ ���Ḧ ��ٸ���.  
    pthread_join(p_thread[0], (void **)&status); 
    pthread_join(p_thread[1], (void **)&status); 
 
    return 0; 
} 
