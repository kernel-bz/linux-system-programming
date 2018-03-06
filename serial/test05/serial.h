//
//	file name: serial.h
//	comments: Serial Port Module
//	author: Jung,JaeJoon(rgbi3307@nate.com) at the www.kernel.bz
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
int serial_write (int fd, char *buf, int size);

//wait: seconds
int serial_write_select (int fd, char *buf, int size, int wait);
//wait: seconds
int serial_write_poll (int fd, char *buf, int size, int wait);

//Reading Data from the Port
//입력포트에 읽을 데이터가 있으면 읽은 크기를 반환함.
//입력포트에 읽을 데이터가 없으면 데이터가 입력될때 까지 block(sleep) 됨.
int serial_read (int fd, char *buf, int mode);

//wait: seconds
int serial_read_select (int fd, char *buf, int wait);
//wait: seconds
//flag: timeout 메세지 출력
int serial_read_poll (int fd, char *buf, int wait, int flag);

int serial_close (int fd);

void serial_setting(int fd);

#endif