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
    } 
    
	// �ڽ����μ����� �����Ű�� �ڵ� 
    else if (pid == 0) 
    { 
        printf("�ڽ�: �� PID�� %d\n", getpid()); 
        while(1) 
        { 
            printf("-->%d\n", i); 
            i++; 
            sleep(1); 
        } 
    } 
    // �θ����μ����� �����Ű�� �ڵ� 
    else 
    { 
		printf("�θ�: �� PID�� %d\n", getpid()); 
        printf("�θ�: ���� ���� �ڽ��� PID�� %d\n", pid); 
        sleep(1); 
        printf("�θ�: exit\n"); 
        exit(0); 
    } 
} 
