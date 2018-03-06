//
//	file name: send.c
//	comments: Serial Communication Main
//	author: Jung,JaeJoon(rgbi3307@nate.com) at the www.kernel.bz
//

#include <stdio.h>
#include <string.h>
#include <termios.h>	//terminal control definitions
#include "serial.h"

int input_data (char *buf)
{
	int i = 0, c;

	printf ("\nSend: ");
	memset (buf, 0, BUFSIZE);	
	while ((c = getchar()) != (int)'\n' && i < BUFSIZE-2) {
		buf[i++] = c;
	}
	buf[i] = '\n';
	buf[i+1] = 0;

	return i+1;
}

int main (void)
{
	int fd, len;
	char buf[BUFSIZE];
	struct termios options;

	fd = serial_open (DEV_NODE, 0);
	if (fd < 0) return -1;	

	//get current serial port settings
	tcgetattr(fd, &options); 
	//�ø��� ��Ʈ ����
	serial_setting (fd);

	printf ("sizeof buffer=%d\n", sizeof(buf));

	len = input_data (buf);
	while (len > 0 && buf[0] > 0) {
		//sending...
		if (serial_write (fd, buf, len) < 0) break;

		//receiving...
		if (serial_read (fd, buf, 0) < 0) break;	//block mode
		//if (serial_read (fd, buf, 1, len) < 0) break;	//non-block mode
		
		printf ("Recv: %s\n", buf);

		len = input_data (buf);
	}

	//restore current serial port settings
	tcsetattr(fd, TCSANOW, &options);

	serial_close (fd);

	return 0;
}
