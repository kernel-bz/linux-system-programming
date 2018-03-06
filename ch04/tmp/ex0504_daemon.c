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
	
	if (pid == -1)     
	{         
		perror("fork error ");         
		exit(0);     

	} else if (pid == 0) {	//�ڽ����μ����� �����Ű�� �ڵ�
		printf("�ڽ�: �� PID�� %d\n", getpid());
		close(0);	//ǥ���Է� ����
		close(1);	//ǥ����� ����
		close(2);	//ǥ�ؿ��� ����
		setsid();	//�͹̳� ����, ������ �ڱ�ȯ�� ����

		while(1)
		{
			printf("-->%d\n", i);
			i++;
			sleep(1);
		}
	
	} else {	//�θ����μ����� �����Ű�� �ڵ�
		printf("�θ�: �� PID�� %d\n", getpid());
		printf("�θ�: ���� ���� �ڽ��� PID�� %d\n", pid);
		sleep(1);
		printf("�θ�: exit\n");
		exit(0);
	} 
} 

//���μ��� Ȯ��
//$ ps -efjc | grep daemon  

