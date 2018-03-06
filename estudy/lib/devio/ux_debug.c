//------------------------------------------------------------------------------
// 파 일 명 : ux_debug.c
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

#include <sys/poll.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
//#include <ex.h>						// exlib 가 반드시 있어야 한다. 
#include <linux/input.h>
#include <linux/limits.h>				// PATH_MAX 
#include <sys/types.h>					// dev_t
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/kdev_t.h>				// MKDEV


