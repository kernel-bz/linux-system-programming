#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/wait.h> 
#include <sys/types.h>  

#define chop(str) str[strlen(str)-1] = 0x00;  

int main (int argc, char **argv) 
{   
	char buf[256];
	printf("My Shell\n");   
	int pid;   
	
	while(1)   {     // 사용자 입력을 기다린다.     
		printf("# ");     
		fgets(buf, 255, stdin);     
		chop(buf);      
		
		// 입력이 quit 라면, 프로그램을 종료한다.     
		if (strncmp(buf, "quit", 4) == 0)     
		{       
			exit(0);     
		}      
		
		// 입력한 명령이 실행가능한 프로그램이라면     
		// fork 한후 execl을 이용해서 실행한다.     
		if (access (buf, X_OK) == 0)
		{
			pid = fork();
			if (pid < 0)
			{
				fprintf(stderr, "Fork Error");
			}
			if (pid == 0)
			{
				if (execl(buf, buf, NULL) == -1)
					fprintf(stderr, "Command Exec Error\n\n");
				exit(0);
			}
			if (pid > 0)
			{
				// 부모 프로세스는 자식프로세스가 종료되길 기다린다.         
				int status;
				waitpid (pid, &status, WUNTRACED);
			}
		}
		else // 만약 실행가능한 프로그램이 아니라면, 에러메시지를 출력     
		{
			fprintf(stderr, "Command Not Found\n\n");
		}
	} 
} 
