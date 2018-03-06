
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

///signal handler function
static void signal_handler(int signo)
{
    if (signo == SIGINT)
        printf ("Caught SIGINT!\n");
    else if (signo == SIGTERM) {
        printf ("Caught SIGTERM!\n");
        exit(EXIT_SUCCESS);
    } else if (signo == SIGHUP)
        printf ("Caught SIGHUP!\n");
    else if (signo == SIGUSR1)
        printf ("Caught SIGUSR1!\n");
    else {
        fprintf(stderr, "Unexpected signal!\n");
        exit (EXIT_FAILURE);
    }

    ///exit(EXIT_SUCCESS);
}

int main(void)
{
    int pid, child;

    if (signal(SIGUSR1, signal_handler) == SIG_ERR) {
        fprintf(stderr, "Cannot handle SIGHUP!\n");
        exit (EXIT_FAILURE);
    }

    pid = fork();
    if (pid==0) {
        while(1) {
            printf("running child pid=%d\n", getpid());
            sleep(1);
        }
    }

    child = pid;

    ///for (;;)
    while(1) {
        ///pause();
        sleep(5);
        raise(SIGUSR1);

        sleep(2);
        ///pid = getpid();
        kill(child, SIGUSR1);
        ///kill(child, SIGTERM);
    }

    return 0;
}
