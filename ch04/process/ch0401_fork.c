//
//	file name	: ch0601_fork.c
//	author		: Jung,JaeJoon(rgbi3307@nate.com) on the www.kernel.bz
//	comments	: simple shell program using fork
//
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main()
{
    int pid;
    int i;

    i = 1000;
    pid = fork();
    printf("fork(): pid=%d\n", pid);

	if (pid == -1)
    {
        perror("fork error ");
        exit(0);
    }
	// 자식프로세스가 실행시키는 코드
    else if (pid == 0)
    {
        printf("Child: pid=%d\n", getpid());
        while(1)
        {
            ///i=1000
            printf("-->%d\n", i);
            i++;
            sleep(1);
        }
    }
	// 부모프로세스가 실행시키는 코드
    else
    {
		printf("Parent: pid=%d\n", getpid());
        //printf("부모 : 내가 낳은 자식의 PID는 %d\n", pid);
        ///i=1000
        while(1)
        {
           printf("==>%d\n", i);
           i += 4;
           sleep(1);
        }
    }
}
