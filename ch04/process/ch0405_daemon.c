//
//	file name	: ch0605_daemon.c
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

	} else if (pid == 0) {	//child process --> daemon
		printf("Child: pid=%d\n", getpid());
		close(0);	//close stdin
		close(1);	//close stdout
		close(2);	//clode stderr
		setsid();	//terminal close, create new session and process group

		while(1) {
			///printf("c(%d)\n", i);
			i++;
			sleep(1);
		}

	} else {	//parent process
		printf("Parent: pid=%d\n", getpid());
		printf("Parent: child pid=%d\n", pid);
		sleep(2);
		printf("Parent: exit\n");
		exit(0);
	}
}

//process display
//$ ps -efjc | grep daemon

