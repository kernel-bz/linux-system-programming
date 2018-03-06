//
//	file name: serial.c
//	comments: Serial Port Module
//	author: Jung,JaeJoon(rgbi3307@nate.com) at the www.kernel.bz
//

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
//for select
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
//for poll
#include <sys/poll.h>

#include "serial.h"

//Open serial port.
//Returns the file descriptor on success or -1 on error.
int serial_open (char *device, int mode)
{
	int fd;

	//O_NOCTTY: 터미널 장치(키보드)로 부터 간섭받지 않도록 함
	//O_NDELAY: DCD(Data Carrier Detect) 신호를 점검하지 않음
	//O_NONBLOCK: read에서 block되지 않음
	if (mode == 0) fd = open (device, O_RDWR | O_NOCTTY);
	else if (mode == 1) fd = open (device, O_RDWR | O_NOCTTY | O_NDELAY);
	else if (mode == 2) fd = open (device, O_RDWR | O_NOCTTY | O_NONBLOCK);
	else fd = open (device, O_RDWR | O_NOCTTY);

	if (fd == -1) printf ("serial_open(%s) error!\n", device);
	
	return fd;
}


//Writing Data to the Port
//return size of writing
int serial_write (int fd, char *buf, int len)
{
	int n;

	//n = write (fd, buf, BUFSIZE); 
	n = write (fd, buf, len);
	if (n < 0) printf ("serial_write() error!\n");
	return n;
}

//wait: seconds
int serial_write_select (int fd, char *buf, int len, int wait)
{
	int            n;
	//int            socket;
	//int            fd;
	int            max_fd;
	fd_set         output;
	struct timeval timeout;

	//Initialize the output set
	FD_ZERO (&output);
	FD_SET (fd, &output);
	//FD_SET (sock, &input);

	max_fd = fd + 1;

	//Initialize the timeout structure
	timeout.tv_sec  = wait;
	timeout.tv_usec = 0;

	//Do the select
	//int select(int max_fd, fd_set *input, fd_set *output, fd_set *error, struct timeval *timeout);
	n = select (max_fd, NULL, &output, NULL, &timeout);

	//See if there was an error
	if (n < 0)
		printf ("writing(select) failed.\n");
	else if (n == 0)
		printf ("writing(select) timeout(%d sec).\n", wait);
	else
	{
		//We have output
		if (FD_ISSET (fd, &output)) {
			//n = write (fd, buf, sizeof(buf));
			n = write (fd, buf, len);
			if (n < 0) printf ("serial_write() error!\n");
		}
		//if (FD_ISSET(sock, &input))
		//	process_socket();
	}

	return n;
}

//wait: seconds
int serial_write_poll (int fd, char *buf, int len, int wait)
{
	struct pollfd fds[1];
	int n;
 
	//watch stdin for input 
    //fds[0].fd = STDIN_FILENO; 
    //fds[0].events = POLLIN; 
     
	//watch stdout for ability to write (almost always true) 
    //fds[1].fd = STDOUT_FILENO; 
    //fds[1].events = POLLOUT;

	fds[0].fd = fd; 
    fds[0].events = POLLOUT; 

	//All set, block! 
	n = poll (fds, 1, wait * 1000);	//msec

	if (n < 0)
		printf ("writing(poll) failed.\n");
	else if (n == 0)
		printf ("writing(poll) timeout(%d sec).\n", wait);
	else
	{
		if (fds[0].revents & POLLOUT) {
			//n = write (fd, buf, strlen(buf));
			n = write (fd, buf, len);
			if (n < 0) printf ("serial_write() error!\n");
		}
		//if (fds[1].revents & POLLOUT) 
        //      printf ("stdout is writeable\n"); 
	}
	return n;
}


//Reading Data from the Port
//입력포트에 읽을 데이터가 있으면 읽은 크기를 반환함.
//입력포트에 읽을 데이터가 없으면 데이터가 입력될때 까지 block(sleep) 됨.
int serial_read (int fd, char *buf, int mode)
{
	int n, len=0;

	if (mode == 0) 
		//일반적인 방식으로 read 동작.(데이터를 읽을때까지 block됨)
		fcntl (fd, F_SETFL, 0);
	else
		//포트에 데이터가 없으면 read가 0을 반환하고 block되지 않도록 함.
		fcntl (fd, F_SETFL, FNDELAY);


	memset (buf, 0, BUFSIZE);
	n = read (fd, buf, BUFSIZE);
	len += n;
	while (buf[n-1] != '\n' && buf[n-1] != '\r') {
		buf += n;
		n = read (fd, buf, BUFSIZE);
		len += n;
	}

	buf[len] = 0;		//set end of string, so we can printf

	return len;
}

//wait: seconds
int serial_read_select (int fd, char *buf, int wait)
{
	int            n;
	int            max_fd;
	fd_set         input;
	struct timeval timeout;

	//Initialize the input set
	FD_ZERO (&input);
	FD_SET (fd, &input);
	//FD_SET (sock, &input);

	max_fd = fd + 1;

	//Initialize the timeout structure
	timeout.tv_sec  = wait;
	timeout.tv_usec = 0;

	//Do the select
	//int select(int max_fd, fd_set *input, fd_set *output, fd_set *error, struct timeval *timeout);
	n = select (max_fd, &input, NULL, NULL, &timeout);

	//See if there was an error
	if (n < 0)
		printf ("reading(select) failed.\n");
	else if (n == 0)
		printf ("reading(select) timeout(%d sec).\n", wait);
	else
	{
		//We have input
		if (FD_ISSET (fd, &input)) {
			n = serial_read (fd, buf, 0);
		}
		//if (FD_ISSET(sock, &input))
		//	process_socket();
	}

	return n;
}

//wait: seconds
int serial_read_poll (int fd, char *buf, int wait)
{
	struct pollfd fds[1];
	int n;
 
	//watch stdin for input 
    //fds[0].fd = STDIN_FILENO; 
    //fds[0].events = POLLIN; 
     
	//watch stdout for ability to write (almost always true) 
    //fds[1].fd = STDOUT_FILENO; 
    //fds[1].events = POLLOUT;

	fds[0].fd = fd; 
    fds[0].events = POLLIN; 

	//All set, block! 
	n = poll (fds, 1, wait * 1000);	//msec

	if (n < 0)
		printf ("reading(poll) failed.\n");
	else if (n == 0)
		printf ("reading(poll) timeout(%d sec).\n", wait);
	else
	{
		if (fds[0].revents & POLLIN) {
			n = serial_read (fd, buf, 0);
		}
		//if (fds[1].revents & POLLOUT) 
        //      printf ("stdout is writeable\n"); 
	}
	return n;
}


int serial_close (int fd)
{
	return close (fd);
}


void serial_setting(int fd)
{
	struct termios options;

	bzero(&options, sizeof(options));

	//options.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
	options.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;	//ICANON

	options.c_cc[VINTR] = 0;
	options.c_cc[VQUIT] = 0;
	options.c_cc[VERASE] = 0;
	options.c_cc[VKILL] = 0;
	options.c_cc[VEOF] = 4;
	options.c_cc[VTIME] = 5;
	options.c_cc[VMIN] = 1;		//blocking read until 1 chars received
	options.c_cc[VSWTC] = 0;
	options.c_cc[VSTART] = 0;
	options.c_cc[VSTOP] = 0;
	options.c_cc[VSUSP] = 0;
	options.c_cc[VEOL] = 0;
	options.c_cc[VREPRINT] = 0;
	options.c_cc[VDISCARD] = 0;
	options.c_cc[VWERASE] = 0;
	options.c_cc[VLNEXT] = 0;
	options.c_cc[VEOL2] = 0;

	tcflush(fd, TCIFLUSH);

	tcsetattr(fd, TCSANOW, &options);
}