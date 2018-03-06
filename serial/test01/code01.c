//
//	file name: code01.c
//	comments: Serial Communication
//	author: Jung,JaeJoon(rgbi3307@nate.com), www.kernel.bz
//

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

//Open serial port 1.
//Returns the file descriptor on success or -1 on error.
int serial_open (void)
{
	int fd;

	//O_NOCTTY: 터미널 장치(키보드)로 부터 간섭받지 않도록 함
	//O_NDELAY: DCD(Data Carrier Detect) 신호를 점검하지 않음
	fd = open ("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1)
		//perror("open_port: Unable to open /dev/ttyS0");
		printf ("open_port: Unable to open /dev/ttyS0\n");
	else
		fcntl (fd, F_SETFL, 0);

	return fd;
}


//Writing Data to the Port
int serial_write (int fd)
{
	int n;

	n = write (fd, "ATZ\r", 4);
	if (n < 0) {
		printf ("write() of 4 bytes failed!\n");
		return -1;
	}
	return n;
}

//Reading Data from the Port
//입력포트에 읽을 데이터가 있으면 읽은 크기를 반환함.
//입력포트에 읽을 데이터가 없으면 데이터가 입력될때 까지 block(sleep) 됨.
int serial_read (int fd)
{
	//포트에 데이터가 없으면 read가 0을 반환하고 block되지 않도록 함.
	fcntl (fd, F_SETFL, FNDELAY);

	//일반적인 방식으로 read 동작.
	fcntl (fd, F_SETFL, 0);

}

int serial_close (int fd)
{
	return close (fd);
}


//==================== Configuring the Serial Port ================================================

/*
termios Structure Members

c_cflag  Control options 
c_lflag  Line options 
c_iflag  Input options 
c_oflag  Output options 
c_cc     Control characters 
c_ispeed Input baud (new interface) 
c_ospeed Output baud (new interface) 
*/


/*
//c_cflag: Control options 
-------------------------------------------------------------------------------
Constants for the c_cflag Member:

CBAUD	Bit mask for baud rate 
B0		0 baud (drop DTR) 
B50		50 baud 
B75		75 baud 
B110	110 baud 
B134	134.5 baud 
B150	150 baud 
B200	200 baud 
B300	300 baud 
B600	600 baud 
B1200	1200 baud 
B1800	1800 baud 
B2400	2400 baud 
B4800	4800 baud 
B9600	9600 baud 
B19200	19200 baud 
B38400	38400 baud 
B57600	57,600 baud 
B76800	76,800 baud 
B115200 115,200 baud 
EXTA	External rate clock 
EXTB	External rate clock 
CSIZE	Bit mask for data bits 
CS5		5 data bits 
CS6		6 data bits 
CS7		7 data bits 
CS8		8 data bits 
CSTOPB	2 stop bits (1 otherwise) 
CREAD	Enable receiver 
PARENB	Enable parity bit 
PARODD	Use odd parity instead of even 
HUPCL	Hangup (drop DTR) on last close 
CLOCAL	Local line - do not change "owner" of port 
LOBLK	Block job control output 
CNEW_RTSCTS 
CRTSCTS Enable hardware flow control (not supported on all platforms) 
*/

int serial_set_control (int fd)
{
	struct termios options;

	//Get the current options for the port.
	tcgetattr(fd, &options);

	//Set the baud rates to 19200.
	//대부분의 시스템에서 입력과 출력 속도는 동일하게..
    cfsetispeed(&options, B19200);
    cfsetospeed(&options, B19200);

	//Enable the receiver and set local mode.
    options.c_cflag |= (CLOCAL | CREAD);

	//Setting the Character Size
	options.c_cflag &= ~CSIZE; /* Mask the character size bits */
    options.c_cflag |= CS8;    /* Select 8 data bits */

	//Setting Parity Checking

	//No parity (8N1): 
	options.c_cflag &= ~PARENB
	options.c_cflag &= ~CSTOPB
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

	/*
	//Even parity (7E1): 
	options.c_cflag |= PARENB
	options.c_cflag &= ~PARODD
	options.c_cflag &= ~CSTOPB
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS7;

	//Odd parity (7O1): 
	options.c_cflag |= PARENB
	options.c_cflag |= PARODD
	options.c_cflag &= ~CSTOPB
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS7;

	//Space parity is setup the same as no parity (7S1): 
	options.c_cflag &= ~PARENB
	options.c_cflag &= ~CSTOPB
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	*/	

	//Setting Hardware Flow Control using the CTS (Clear To Send) and RTS (Request To Send) signal lines

	//enable hardware flow control
    //options.c_cflag |= CNEW_RTSCTS;    /* Also called CRTSCTS */

	//disable hardware flow control
    //options.c_cflag &= ~CNEW_RTSCTS;

	//Set the new options for the port.
	//TCSANOW:		Make changes now without waiting for data to complete 
	//TCSADRAIN:	Wait until everything has been transmitted 
	//TCSAFLUSH:	Flush input and output buffers and make the change 
    tcsetattr(fd, TCSANOW, &options);
}


/*
c_lflag: Line options
-------------------------------------------------------------------------------
The local modes member c_lflag controls how input characters are managed by the serial driver. 
In general you will configure the c_lflag member for canonical or raw input.

Constants for the c_lflag Member:

ISIG	Enable SIGINTR, SIGSUSP, SIGDSUSP, and SIGQUIT signals 
ICANON	Enable canonical input (else raw) 
XCASE	Map uppercase \lowercase (obsolete) 
ECHO	Enable echoing of input characters 
ECHOE	Echo erase character as BS-SP-BS 
ECHOK	Echo NL after kill character 
ECHONL	Echo NL 
NOFLSH	Disable flushing of input buffers after interrupt or quit characters 
IEXTEN	Enable extended functions 
ECHOCTL Echo control characters as ^char and delete as ~? 
ECHOPRT Echo erased character as character erased 
ECHOKE	BS-SP-BS entire line on line kill 
FLUSHO	Output being flushed 
PENDIN	Retype pending input at next read or input char 
TOSTOP	Send SIGTTOU for background output 
*/

int serial_set_line (int fd)
{

	//Choosing Canonical Input
	//Canonical input is line-oriented. 
	//Input characters are put into a buffer which can be edited interactively by the user 
	//until a CR (carriage return) or LF (line feed) character is received.
	//When selecting this mode you normally select the ICANON, ECHO, and ECHOE options:

    options.c_lflag |= (ICANON | ECHO | ECHOE);

	//Choosing Raw Input
	//Raw input is unprocessed. 
	//Input characters are passed through exactly as they are received, when they are received. 
	//Generally you'll deselect the ICANON, ECHO, ECHOE, and ISIG options when using raw input:

    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	//A Note About Input Echo
	//Never enable input echo (ECHO, ECHOE) when sending commands to a MODEM or other computer that is echoing characters, 
	//as you will generate a feedback loop between the two serial interfaces!
}

/*
c_iflag: Input Options
-------------------------------------------------------------------------------
The input modes member c_iflag controls any input processing that is done to characters received on the port. 
Like the c_cflag field, the final value stored in c_iflag is the bitwise OR of the desired options. 

Constants for the c_iflag Member:

INPCK	Enable parity check 
IGNPAR	Ignore parity errors 
PARMRK	Mark parity errors 
ISTRIP	Strip parity bits 
IXON	Enable software flow control (outgoing) 
IXOFF	Enable software flow control (incoming) 
IXANY	Allow any character to start flow again 
IGNBRK	Ignore break condition 
BRKINT	Send a SIGINT when a break condition is detected 
INLCR	Map NL to CR 
IGNCR	Ignore CR 
ICRNL	Map CR to NL 
IUCLC	Map uppercase to lowercase 
IMAXBEL Echo BEL on input line too long 
*/

int serial_set_input (int fd)
{
	//Setting Input Parity Options
	//You should enable input parity checking when you have enabled parity in the c_cflag member (PARENB). 
	//The revelant constants for input parity checking are INPCK, IGNPAR, PARMRK , and ISTRIP. 
	//Generally you will select INPCK and ISTRIP to enable checking and stripping of the parity bit:

    options.c_iflag |= (INPCK | ISTRIP);

	//IGNPAR is a somewhat dangerous option that tells the serial driver to ignore parity errors 
	//and pass the incoming data through as if no errors had occurred. 
	//This can be useful for testing the quality of a communications link, but in general is not used for practical reasons.

	//PARMRK causes parity errors to be 'marked' in the input stream using special characters. 
	//If IGNPAR is enabled, a NUL character (000 octal) is sent to your program before every character with a parity error. 
	//Otherwise, a DEL (177 octal) and NUL character is sent along with the bad character.

	//Setting Software Flow Control
	//Software flow control is enabled using the IXON, IXOFF, and IXANY constants:

    options.c_iflag |= (IXON | IXOFF | IXANY);

	//To disable software flow control simply mask those bits:

    options.c_iflag &= ~(IXON | IXOFF | IXANY);

	//The XON (start data) and XOFF (stop data) characters are defined in the c_cc array described below. 
}

/*
c_oflag: Output Options
-------------------------------------------------------------------------------
The c_oflag member contains output filtering options. 
Like the input modes, you can select processed or raw data output. 

Constants for the c_oflag Member:

OPOST	Postprocess output (not set = raw output) 
OLCUC	Map lowercase to uppercase 
ONLCR	Map NL to CR-NL 
OCRNL	Map CR to NL 
NOCR	No CR output at column 0 
ONLRET	NL performs CR function 
OFILL	Use fill characters for delay 
OFDEL	Fill character is DEL 
NLDLY	Mask for delay time needed between lines 
NL0		No delay for NLs 
NL1		Delay further output after newline for 100 milliseconds 
CRDLY	Mask for delay time needed to return carriage to left column 
CR0		No delay for CRs 
CR1		Delay after CRs depending on current column position 
CR2		Delay 100 milliseconds after sending CRs 
CR3		Delay 150 milliseconds after sending CRs 
TABDLY	Mask for delay time needed after TABs 
TAB0	No delay for TABs 
TAB1	Delay after TABs depending on current column position 
TAB2	Delay 100 milliseconds after sending TABs 
TAB3	Expand TAB characters to spaces 
BSDLY	Mask for delay time needed after BSs 
BS0		No delay for BSs 
BS1		Delay 50 milliseconds after sending BSs 
VTDLY	Mask for delay time needed after VTs 
VT0		No delay for VTs 
VT1		Delay 2 seconds after sending VTs 
FFDLY	Mask for delay time needed after FFs 
FF0		No delay for FFs 
FF1		Delay 2 seconds after sending FFs 
*/

int serial_set_output (int fd)
{
	//Choosing Processed Output
	//Processed output is selected by setting the OPOST option in the c_oflag member:

    options.c_oflag |= OPOST;

	//Of all the different options, you will only probably use the ONLCR option which maps newlines into CR-LF pairs. 
	//The rest of the output options are primarily historic and date back to the time 
	//when line printers and terminals could not keep up with the serial data stream!

	//Choosing Raw Output
	//Raw output is selected by resetting the OPOST option in the c_oflag member:

    options.c_oflag &= ~OPOST;

	//When the OPOST option is disabled, all other option bits in c_oflag are ignored.
}

/*
c_cc: Control Characters
-------------------------------------------------------------------------------
The c_cc character array contains control character definitions as well as timeout parameters. 
Constants are defined for every element of this array. 

Control Characters in the c_cc Member:

VINTR	Interrupt			CTRL-C 
VQUIT	Quit				CTRL-Z 
VERASE	Erase				Backspace (BS) 
VKILL	Kill-line			CTRL-U 
VEOF	End-of-file			CTRL-D 
VEOL	End-of-line			Carriage return (CR) 
VEOL2	Second end-of-line	Line feed (LF) 
VMIN	Minimum number of characters to read - 
VSTART	Start flow			CTRL-Q (XON) 
VSTOP	Stop flow			CTRL-S (XOFF) 
VTIME	Time to wait for data (tenths of seconds) - 
*/


//==================== Selecting Input from a Serial Port =========================================
/*
While simple applications can poll or wait on data coming from the serial port, 
most applications are not simple and need to handle input from multiple sources.

UNIX provides this capability through the select(2) system call. 
This system call allows your program to check for input, output, or error conditions on one or more file descriptors. 
The file descriptors can point to serial ports, regular files, other devices, pipes, or sockets. 
You can poll to check for pending input, wait for input indefinitely, 
or timeout after a specific amount of time, making the select system call extremely flexible.

Most GUI Toolkits provide an interface to select; we will discuss the X Intrinsics ("Xt") library later in this chapter.

The SELECT System Call
The select system call accepts 5 arguments:

    int select(int max_fd, fd_set *input, fd_set *output, fd_set *error, struct timeval *timeout);

The max_fd argument specifies the highest numbered file descriptor in the input, output, and error sets. 
The input, output, and error arguments specify sets of file descriptors for pending input, output, or error conditions; 
specify NULL to disable monitoring for the corresponding condition. These sets are initialized using three macros:

    FD_ZERO(&fd_set);
    FD_SET(fd, &fd_set);
    FD_CLR(fd, &fd_set);

The FD_ZERO macro clears the set entirely. 
The FD_SET and FD_CLR macros add and remove a file descriptor from the set, respectively.

The timeout argument specifies a timeout value which consists of seconds (timeout.tv_sec) and microseconds (timeout.tv_usec ). 
To poll one or more file descriptors, set the seconds and microseconds to zero. 
To wait indefinitely specify NULL for the timeout pointer.

The select system call returns the number of file descriptors that have a pending condition, or -1 if there was an error.

Using the SELECT System Call
Suppose we are reading data from a serial port and a socket. 
We want to check for input from either file descriptor, but want to notify the user if no data is seen within 10 seconds. 
To do this we'll need to use the select system call, as blow.

Using SELECT to process input from more than one source.
*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>

int            n;
int            socket;
int            fd;
int            max_fd;
fd_set         input;
struct timeval timeout;

//Initialize the input set
FD_ZERO(&input);
FD_SET(fd, &input);
FD_SET(sock, &input);

max_fd = (sock > fd) ? sock : fd + 1;

//Initialize the timeout structure
timeout.tv_sec  = 10;
timeout.tv_usec = 0;

//Do the select
n = select(max_fd, &input, NULL, NULL, &timeout);

//See if there was an error
if (n < 0)
	perror("select failed");
else if (n == 0)
	puts("TIMEOUT");
else
{
	//We have input
	if (FD_ISSET(fd, &input))
		process_fd();
	if (FD_ISSET(sock, &input))
		process_socket();
}
