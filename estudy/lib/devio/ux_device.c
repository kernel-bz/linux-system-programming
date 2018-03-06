//------------------------------------------------------------------------------
// �� �� �� : ux.c
// ������Ʈ : fwp
// ��    �� :
// �� �� �� : ����â, �̿���, �����
// �� �� �� : 2009�� 1�� 17��
// �� �� �� :
// �� �� �� :
// ��    �� :
// ���̼��� : GPL V2
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
// ���� : �˻��� ��ġ�� mknod ���� ,������ Open�Ѵ�. 
// ���� : http://forum.falinux.com/zbxe/?mid=Kernel_API&document_srl=405682&listStyle=&cpage=
// ���� : Thread �� ����Ǿ����� ���� 
// ��ȯ : ���� 		: ���� ��ũ���� , 0���� ũ�ų� ����.
//		  ���� 		: 0 ���� �۴�. 
// �Ű� : path 		: �⺻ ��ġ 
// 		  major		: ��ġ�� major ��ȣ
// 		  minor 	: ��ġ�� minor ��ȣ
// 		  open_flag	: open �Ҷ� flag �ɼ� 
//--------------------------------------------------------------
int ux_mknod_device_open(char *path, char major, char minor , int open_flag)
{
	char 	file_name[PATH_MAX];			// 4096 
	int		dev_fd =0;
	
	static 	int mknod_count =0;	
	
	sprintf (file_name, "%s-%d-%d", path , getpid(), ++mknod_count ); // �ߺ� ���� 
	
	remove (file_name);												// ���� ���� ������ �ִ°� �����Ѵ�. remove�Լ��� ���� open�ϰ� ������ ���� 
	    						
	// mknod �Ѵ�. 	
	mknod (file_name, (S_IRWXU|S_IRWXG|S_IFCHR), MKDEV(major , minor));
	
	dev_fd = open (file_name, open_flag);							// mknod �� ������ open �� �� fd�� ���´�. 
	if (dev_fd < 0)	return dev_fd;

	printf ("ux_mknod_device_open(): %d, %s\n", dev_fd, file_name);
		
	unlink (file_name);										   		// ���μ����� ����� �� �ڵ��� ���� ���� .. ���� ���� �����Ǵ°� �ƴϴ�.^^;

	return dev_fd;
}


