//
//	Waiting for Input from Multiple Sources
//
/*
This is not a different input mode, but might be useful, if you are handling multiple devices. In my application I was handling input over a TCP/IP socket and input over a serial connection from another computer quasi-simultaneously. The program example given below will wait for input from two different input sources. If input from one source becomes available, it will be processed, and the program will then wait for new input. 

The approach presented below seems rather complex, but it is important to keep in mind that Linux is a multi-processing operating system. The select system call will not load the CPU while waiting for input, whereas looping until input becomes available would slow down other processes executing at the same time. 
*/

#include <sys/time.h>
      #include <sys/types.h>
      #include <unistd.h>
        
      main()
      {
        int    fd1, fd2;  /* input sources 1 and 2 */
        fd_set readfs;    /* file descriptor set */
        int    maxfd;     /* maximum file desciptor used */
        int    loop=1;    /* loop while TRUE */ 
        
        /* open_input_source opens a device, sets the port correctly, and
           returns a file descriptor */
        fd1 = open_input_source("/dev/ttyS0");   /* COM2 */
        if (fd1<0) exit(0);
        fd2 = open_input_source("/dev/ttyS1");   /* COM3 */
        if (fd2<0) exit(0);
        maxfd = MAX (fd1, fd2)+1;  /* maximum bit entry (fd) to test */
        
        /* loop for input */
        while (loop) {
          FD_SET(fd1, &readfs);  /* set testing for source 1 */
          FD_SET(fd2, &readfs);  /* set testing for source 2 */
          /* block until input becomes available */
          select(maxfd, &readfs, NULL, NULL, NULL);
          if (FD_ISSET(fd1))         /* input from source 1 available */
            handle_input_from_source1();
          if (FD_ISSET(fd2))         /* input from source 2 available */
            handle_input_from_source2();
        }
      }

/*
int res;
        struct timeval Timeout;

        /* set timeout value within input loop */
        Timeout.tv_usec = 0;  /* milliseconds */
        Timeout.tv_sec  = 1;  /* seconds */
        res = select(maxfd, &readfs, NULL, NULL, &Timeout);
        if (res==0)
        /* number of file descriptors with input = 0, timeout occurred. */ 
*/

