//
//	file name: set_baud_rate.c
//	comments: stty -F /dev/ttyS0 9600
//

#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <sys/fcntl.h>
 
int main( int argc, char *argv[] )
{
	struct termios termOptions;
	char        port[1024];
	int         ttyFid;
 
	if( argc == 2 )
	{
		// Copy the port from the argument
		strcpy( port, argv[1] );
		printf( "Read port name: %s\n", port );
	}
	else
	{
		// No argument or incorrect number of arguments read:
		printf( "Usage: setSer /dev/ttySX\n" );
		return -1;
	}
 
	// Open the tty:
	ttyFid = open( port, O_RDWR );
	if (ttyFid == -1)
	{
		printf( "Error unable to open port: %s\n", port );
		return -1;
	}
 
	// Get the current options:
	tcgetattr( ttyFid, &termOptions );
	
	// Set the input/output speed to 921.6kbps
	//cfsetispeed( &termOptions, B921600 );
	//cfsetospeed( &termOptions, B921600 );
	cfsetispeed( &termOptions, B115200 );
	cfsetospeed( &termOptions, B115200 );
 
	// Now set the term options (set immediately)
	tcsetattr( ttyFid, TCSANOW, &termOptions );
 
	// All done
	printf( "Done, port speed set on: %s\n", port );
}
