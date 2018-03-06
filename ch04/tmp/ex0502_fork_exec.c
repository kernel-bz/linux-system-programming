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
	
	while(1)   {     // ����� �Է��� ��ٸ���.     
		printf("# ");     
		fgets(buf, 255, stdin);     
		chop(buf);      
		
		// �Է��� quit ���, ���α׷��� �����Ѵ�.     
		if (strncmp(buf, "quit", 4) == 0)     
		{       
			exit(0);     
		}      
		
		// �Է��� ����� ���డ���� ���α׷��̶��     
		// fork ���� execl�� �̿��ؼ� �����Ѵ�.     
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
				// �θ� ���μ����� �ڽ����μ����� ����Ǳ� ��ٸ���.         
				int status;
				waitpid (pid, &status, WUNTRACED);
			}
		}
		else // ���� ���డ���� ���α׷��� �ƴ϶��, �����޽����� ���     
		{
			fprintf(stderr, "Command Not Found\n\n");
		}
	} 
} 
