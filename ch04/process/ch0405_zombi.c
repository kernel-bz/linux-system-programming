//
//	file name	: ch0605_zombi.c
//	author		: Jung,JaeJoon(rgbi3307@nate.com) on the www.kernel.bz
//
#include <unistd.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdio.h> 
 
int main() 
{ 
    int pid; 
    int i = 1000; 
 
    pid = fork(); 
    if (pid == -1) { 
        perror("fork error "); 
        exit(0); 
    } else if (pid == 0) {	//child process --> zombi 
        printf("Child: pid=%d\n", getpid()); 
        while(1) { 
            printf("c(%d)\n", i); 
            i++; 
            sleep(1); 
        } 
    } else {	//parent process --> exit 
		printf("Parent: pid=%d\n", getpid()); 
        printf("Parent: Child pid=%d\n", pid); 
        sleep(1); 
        printf("Parent: exit\n"); 
        exit(0); 
    } 
} 

