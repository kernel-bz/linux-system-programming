#include <stdio.h>			//표준 I/O 루틴
#include <pthread.h>		//pthread 함수와 데이타 스트럭쳐
#include <string.h>
#include <unistd.h>

typedef struct
{
	char			fname[20];
	unsigned int	length;
} SND_FILE;

pthread_mutex_t a_mutex = PTHREAD_MUTEX_INITIALIZER;

//새 쓰레드가 실행시킬 함수
void* do_loop(void* data)
{
    int i;			//숫자를 찍을 카운터
    int j;			//지연용 카운터
	int rc;
    //int me = *((int*)data);     //쓰레드 구분 숫자
	SND_FILE	*snd_file;

	rc = pthread_mutex_lock(&a_mutex);

	snd_file = (SND_FILE*)data;

    for (i=0; i<10; i++) 
	{
		printf("'%s' - Got '%d'\n", snd_file->fname, snd_file->length);

		for (j=0; j<500000; j++);	//지연 루프
		sleep(1);        
    }

	rc = pthread_mutex_unlock(&a_mutex);

    //쓰레드 없애기(리턴시 자동종료됨)
    pthread_exit(NULL);
}

//보통의 C 프로그램처럼 main에서 시작합니다
int main(int argc, char* argv[])
{
    int			thr_id;         //새 쓰레드용 쓰레드 ID
    pthread_t	p_thread1;       //쓰레드 구조체
	pthread_t	p_thread2;       //쓰레드 구조체
	SND_FILE	snd_file;

    /* 'do_loop()를 실행시킬 새 쓰레드 만들기 */
	strcpy(snd_file.fname, "test");
	snd_file.length = 10;
    thr_id = pthread_create(&p_thread1, NULL, do_loop, (void*)&snd_file);
  
	strcpy(snd_file.fname, "test2");
	snd_file.length = 20;
	//main()함수에서도 'do_loop()' 실행시키기
    //do_loop ((void*)&snd_file);
	thr_id = pthread_create(&p_thread2, NULL, do_loop, (void*)&snd_file);

	sleep(30);
    
    //NOT REACHED
    return 0;
}

