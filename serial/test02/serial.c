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

#include "serial.h"

//Open serial port.
//Returns the file descriptor on success or -1 on error.
int serial_open (char *device, int mode)
{
	int fd;

	//O_NOCTTY: �͹̳� ��ġ(Ű����)�� ���� �������� �ʵ��� ��
	//O_NDELAY: DCD(Data Carrier Detect) ��ȣ�� �������� ����
	//O_NONBLOCK: read���� block���� ����
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

//Reading Data from the Port
//�Է���Ʈ�� ���� �����Ͱ� ������ ���� ũ�⸦ ��ȯ��.
//�Է���Ʈ�� ���� �����Ͱ� ������ �����Ͱ� �Էµɶ� ���� block(sleep) ��.
int serial_read (int fd, char *buf, int mode)
{
	int n, len=0;

	if (mode == 0) 
		//�Ϲ����� ������� read ����.(�����͸� ���������� block��)
		fcntl (fd, F_SETFL, 0);
	else
		//��Ʈ�� �����Ͱ� ������ read�� 0�� ��ȯ�ϰ� block���� �ʵ��� ��.
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
