//------------------------------------------------------------------------------
// 파 일 명 : ux.c
// 프로젝트 : fwp
// 설    명 :
// 작 성 자 : 유영창, 이영민, 문경원
// 작 성 일 : 2009년 1월 17일
// 수 정 일 :
// 수 정 일 :
// 주    의 :
// 라이센스 : GPL V2
//------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <linux/input.h>
#include <linux/limits.h>				// PATH_MAX 
#include <linux/kdev_t.h>				// MKDEV

#include <sys/poll.h>
#include <sys/types.h>					// dev_t
#include <sys/stat.h>
#include <fcntl.h>


//--------------------------------------------------------------
// 설명 : 검색된 장치를 mknod 한후 ,파일을 Open한다. 
// 참고 : http://forum.falinux.com/zbxe/?mid=Kernel_API&document_srl=405682&listStyle=&cpage=
// 주의 : Thread 는 고려되어있지 않음 
// 반환 : 성공 		: 파일 디스크립션 , 0보다 크거나 같다.
//		  실패 		: 0 보다 작다. 
// 매개 : path 		: 기본 위치 
// 		  major		: 장치의 major 번호
// 		  minor 	: 장치의 minor 번호
// 		  open_flag	: open 할때 flag 옵션 
//--------------------------------------------------------------
int ux_mknod_device_open(char *path, char major, char minor , int open_flag)
{
	char 	file_name[PATH_MAX];			// 4096 
	int		dev_fd =0;
	
	static 	int mknod_count =0;	
	
	sprintf (file_name, "%s-%d-%d", path , getpid(), ++mknod_count ); // 중복 방지 
	
	remove (file_name);												// 당장 삭제 기존에 있는건 삭제한다. remove함수는 누가 open하고 있으면 실패 
	    						
	// mknod 한다. 	
	mknod (file_name, (S_IRWXU|S_IRWXG|S_IFCHR), MKDEV(major , minor));
	
	dev_fd = open (file_name, open_flag);							// mknod 된 파일을 open 한 후 fd를 얻어온다. 
	if (dev_fd < 0)	return dev_fd;

	printf ("ux_mknod_device_open(): %d, %s\n", dev_fd, file_name);
		
	unlink (file_name);										   		// 프로세스가 종료될 때 자동의 파일 삭제 .. 지금 당장 삭제되는게 아니다.^^;

	return dev_fd;
}


