
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

///signal handler function
static void signal_handler(int signo)
{
    if (signo == SIGINT)
        printf ("Caught SIGINT!\n");
    else if (signo == SIGTERM)
        printf ("Caught SIGTERM!\n");
    else if (signo == SIGHUP)
        printf ("Caught SIGHUP!\n");
    else {
        fprintf(stderr, "Unexpected signal!\n");
        exit (EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

int main(void)
{
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        fprintf(stderr, "Cannot handle SIGINT!\n");
        exit (EXIT_FAILURE);
    }

    if (signal(SIGTERM, signal_handler) == SIG_ERR) {
        fprintf(stderr, "Cannot handle SIGTERM!\n");
        exit (EXIT_FAILURE);
    }

    if (signal(SIGPROF, signal_handler) == SIG_ERR) {
        fprintf(stderr, "Cannot handle SIGPROF!\n");
        exit (EXIT_FAILURE);
    }

    if (signal(SIGHUP, signal_handler) == SIG_ERR) {
        fprintf(stderr, "Cannot handle SIGHUP!\n");
        exit (EXIT_FAILURE);
    }

    ///for (;;)
    while(1) {
        pause();
        ///sleep(10);
        ///raise(SIGHUP);
    }

    return 0;
}
