//------------------------------------------------------------------------------
// �� �� �� : ux_debug.c
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

#include <sys/poll.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
//#include <ex.h>						// exlib �� �ݵ�� �־�� �Ѵ�. 
#include <linux/input.h>
#include <linux/limits.h>				// PATH_MAX 
#include <sys/types.h>					// dev_t
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/kdev_t.h>				// MKDEV


