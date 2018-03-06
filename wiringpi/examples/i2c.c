/*
 * i2c.c:
 *	I2C test
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
//#include <fcntl.h>
//#include <sys/ioctl.h>
//#include <linux/spi/spidev.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>

int main (void)
{
	int fd;
	const int i2c_addr = 0x50;

	//if ((i2c < 0x01) || (i2c > 0x77))
	fd = wiringPiI2CSetup (i2c_addr);
	if (fd < 0) {
		printf("I2C(%02X) setup error!\n", i2c_addr);
		exit(-1);
	} else {
		printf("I2C(%02X) setup succeeded.(fd=%d)\n", i2c_addr, fd);
	}

	while (1) {
		wiringPiI2CWrite (fd, 0x07FF);
	}

	return 0 ;
}
