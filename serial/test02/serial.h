//
//	file name: serial.h
//	comments: Serial Port Module
//	author: Jung,JaeJoon(rgbi3307@nate.com), www.kernel.bz
//

#ifndef _SERIAL_H
#define _SERIAL_H

#define BUFSIZE		256
//#define BAUDRATE	B9600
//#define BAUDRATE	B38400
#define BAUDRATE	B115200

#define DEV_NODE	"/dev/ttyAMA0"

//Open serial port.
//Returns the file descriptor on success or -1 on error.
int serial_open (char *device, int mode);

//Writing Data to the Port
//return size of writing
int serial_write (int fd, char *buf, int len);

//Reading Data from the Port
//�Է���Ʈ�� ���� �����Ͱ� ������ ���� ũ�⸦ ��ȯ��.
//�Է���Ʈ�� ���� �����Ͱ� ������ �����Ͱ� �Էµɶ� ���� block(sleep) ��.
int serial_read (int fd, char *buf, int mode);

int serial_close (int fd);

void serial_setting (int fd);

#endif