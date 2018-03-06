//
//	file name	: ch0602_fork_exec.c
//	author		: Jung,JaeJoon(rgbi3307@nate.com) on the www.kernel.bz
//	comments	: simple shell program using fork,exec
//
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
	int pid;

	printf("My Shell\n");
	while(1) {
		printf("$$ ");
		fgets(buf, 255, stdin);
		chop(buf);

		if (strncmp(buf, "quit", 4) == 0)	//quit for exit
			exit(0);

		if (access (buf, X_OK) == 0) {	//is it executable?
			pid = fork();
			if (pid < 0) {
				fprintf(stderr, "Fork Error");
			} else if (pid == 0) {
				if (execl(buf, buf, NULL) == -1)
					fprintf(stderr, "Command Exec Error\n\n");
				///exit(0);	//child exit
			} else {	//Parent(pid > 0)
				int status;
				waitpid (pid, &status, WUNTRACED);	//wait until child(pid) is finished.
				printf ("child(%d) is finished\n", pid);
			}
		} else {	//not executable
			fprintf(stderr, "Command Not Found\n\n");
		}
	}
}
