//
//	file name: send.c
//	comments: Serial Communication Main
//	author: Jung,JaeJoon(rgbi3307@nate.com) at the www.kernel.bz
//

#include <stdio.h>
#include <string.h>
#include <termios.h>	//terminal control definitions
#include "serial.h"

int input_data (char *send, char *recv, int fd)
{
	int i = 0, c, n;

	printf ("\n<<Send: ");
	memset (recv, 0, BUFSIZE);	//buffer to read
	memset (send, 0, BUFSIZE);	//buffer to write from keyboard

	while ((c = getchar()) != (int)'\n' && i < BUFSIZE-2) {
		send[i++] = c;

		n = serial_read_poll (fd, recv, 10, 0);	//10 msec, timeout 메세지 출력않함
		if (n > 0) printf ("\n>>Recv: %s\n", recv);
	}
	send[i] = '\n';
	send[i+1] = 0;

	return i+1;
}

int main (void)
{
	int fd, n;
	char recv[BUFSIZE], send[BUFSIZE];
	struct termios options;

	fd = serial_open (DEV_NODE, 0);
	if (fd < 0) return -1;	

	//get current serial port settings
	tcgetattr(fd, &options);

	//시리얼 포트 설정
	serial_setting (fd);

	printf ("Serial Communication...\n");

	memset (recv, 0, sizeof(recv));	//buffer to read
	memset (send, 0, sizeof(send));	//buffer to write

	while (n = serial_read_poll (fd, recv, 500, 1) >= 0) {	//500 msec
		if (n > 0) printf (">>Recv: %s\n", recv);

		n = input_data (send, recv, fd);
		if (n > 0) n = serial_write_poll (fd, send, n, 100);	//100 msec
	}

	//restore current serial port settings
	tcsetattr(fd, TCSANOW, &options);

	serial_close (fd);

	return 0;
}
