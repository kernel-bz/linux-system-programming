//------------------------------------------------------------------------------
// 파 일 명 : ex.c
// 프로젝트 : fwp
// 설    명 :
// 작 성 자 : 유영창, 이영민, 문경원
// 작 성 일 : 2007년 5월 25일
// 수 정 일 :
// 수 정 일 :
// 주    의 :
// 라이센스 : GPL V2
//------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>

#include <sys/poll.h>

#define _EX_VAR_ 1
#include "ex.h"
//--------------------------------------------------------------
// 전역 변수 
//--------------------------------------------------------------
static 	ex_mng_t		ex_mng_list[EX_POLL_LIST_MAX];				// 관리 이벤트 리스트
static 	int 			ex_mng_count	= 0;						// 폴 구조체 내의 파일 디스크립터 갯수

static 	pollfd_t		ex_poll_list[EX_POLL_LIST_MAX];				// 이벤트 대기 처리 폴 구조체
static  int				ex_poll_index[EX_POLL_LIST_MAX];            // 이벤트와 관리체의 연결 인텍스를 관리하기 위한 변수
static 	int 			ex_poll_count	= 0;						// 폴 구조체 내의 파일 디스크립터 갯수
static 	int 			ex_adjust_poll_req	= 0;					// ex_adjust_poll_implement()을 호출여부 확인 flag

//--------------------------------------------------------------
// 함수
//--------------------------------------------------------------
static 	int 			ex_wait_time			= EX_WAIT_TIME_DEFAULT;		// 아무런 사건이 없을때 함수를 탈출할 타임 아웃
static 	int				ex_error_code			= EXERR_NONE;				// 에러코드

		int 			(*ex_on_poll_timeout)	(void)				= NULL;
		int				(*ex_on_hooking_loop)	(int poll_state)	= NULL;
		
static	int				ex_adjust_poll_implement(void);						// poll 처리가 가능하도록 최적화 한다. 
//--------------------------------------------------------------
// 설명 : event 라이브러리를 초기화한다.
// 매계 : wait_time poll 대기 시간 설정, 단위[ms], 0이상의 값
// 반환 : 성공0, 실패-1
//--------------------------------------------------------------
int ex_init( int wait_time )
{
	int lp;
	if(wait_time<0)
	{
		ex_error_code = EXERR_WAIT_TIME;
		return -1;
	}

	ex_wait_time = wait_time;

	memset(ex_mng_list,0,sizeof(ex_mng_list));
	for( lp = 0; lp < EX_POLL_LIST_MAX; lp++ )
	{
		ex_mng_list[lp].fd = -1;										// 이벤트 대기 처리 폴 구조체
	}
	ex_mng_count = 0;										// 폴 구조체 내의 파일 디스크립터 갯수

	memset(ex_poll_list,0,sizeof(ex_poll_list));
	for( lp = 0; lp < EX_POLL_LIST_MAX; lp++ )
	{
		ex_poll_list[lp].fd = -1;										// 이벤트 대기 처리 폴 구조체
	}
	ex_poll_count = 0;										// 폴 구조체 내의 파일 디스크립터 갯수

	return 0;
}

//--------------------------------------------------------------
// 설명 : ex_error_code에 대한 에러 설명 문자열을 반환
//--------------------------------------------------------------
static char  *ex_error_string( void)
{
	char *error_string[] ={ "에러 없음",                                    		//EXERR_NONE
							"ex_init함수의 대기시간 값 오류",               		//EXERR_WAIT_TIME
							"poll 함수 오류"	     								//EXERR_POLL_FUNC
							"ex_add_fd함수에 등록할 fd 오류 ",						//EXERR_POLL_ADD_FD
							"ex_add_fd함수에 등록할 메모리 부족 "					//EXERR_POLL_ADD_MEM
                        };
   return( error_string[ex_error_code]);
}

//--------------------------------------------------------------
// 설명 : exlib에 에러코드, 에러문자열 및 사용자 메세지 출력
// 매개 : 사용자 메세지
//--------------------------------------------------------------
void ex_print_error( char *msg )
{
	printf( "\r[gx error:%d]%s %s\n", ex_error_code, ex_error_string(), msg);
	//return gx_error_code;
}

//--------------------------------------------------------------
// 설명 :
//--------------------------------------------------------------
int ex_loop( void )
{
	int 			poll_state;
	int				mng_index;
	int 			lp;
	struct pollfd	*ex_poll_item;
	ex_mng_t		*ex_mng_item;



	if( ex_adjust_poll_req )	
	{
		ex_adjust_poll_implement();			// poll 처리가 가능하도록 최적화 한다. 
		ex_adjust_poll_req = 0;				// flag 초기화 
	}		

	// poll()을 호출하여 event 발생 여부 확인(int값 반환)
	poll_state = poll( ex_poll_list,   		// event 등록 변수(struct pollfd *)
                        ex_poll_count,    	// 체크할 pollfd 개수(int)
                        ex_wait_time   		// time out 시간(int)
                       );

	if( poll_state < 0 )
	{
		ex_error_code = EXERR_POLL_FUNC;
		return -1;
	}

	if( ex_on_hooking_loop != NULL )
	{
		if(ex_on_hooking_loop(poll_state))	return 0;
	}

	if( poll_state == 0)
	{
		if( ex_on_poll_timeout != NULL) ex_on_poll_timeout();
	}

	for( lp = 0; lp < ex_poll_count; lp++ )
	{
		ex_poll_item	= &ex_poll_list[lp];
		mng_index		= ex_poll_index[lp];
		ex_mng_item		= &ex_mng_list[mng_index];

		if( ex_poll_item->revents & POLLIN 	&& ex_mng_item->on_read  != NULL ) ex_mng_item->on_read ( ex_mng_item );
		if( ex_poll_item->revents & POLLOUT && ex_mng_item->on_write != NULL ) ex_mng_item->on_write( ex_mng_item );
		//if( ex_poll_item.revents & POLLERR 	&& ex_event_t->on_poll_err != NULL ) on_poll_err( );
		//if( ex_poll_item.revents & POLLHUP 	&& ex_event_t->on_poll_hup != NULL ) on_poll_hup( );
	}
	return 0;
}


//--------------------------------------------------------------
// 설명 : fd를 관리대상에 포함시키고, fd를 포함하는 ex_mng_t 구조체를 반환한다.
// 반환 : NULL이면 실패, 그 외 ex_mng_t 구조체 주소
// 주의 : 동일 fd는 등록불가
// 주의 : 함수호출 후 ex_adjust_poll함수 호출이 필요.
//--------------------------------------------------------------
ex_mng_t * ex_add_fd( int fd )
{
	int lp;

	if(fd <0)
	{
		ex_error_code = EXERR_POLL_ADD_FD;
		return NULL;
	}

	for(lp=0;lp<ex_mng_count;lp++)
	{
		if(ex_mng_list[lp].fd == fd)
		{
			return &ex_mng_list[lp];
		}
	}

	if(ex_mng_count >=EX_POLL_LIST_MAX)
	{
		ex_error_code = EXERR_POLL_ADD_MEM;
		return NULL;
	}

	ex_mng_list[ex_mng_count].fd = fd;
	return &ex_mng_list[ex_mng_count++];
}

//--------------------------------------------------------------
// 설명 : fd를 관리대상에 삭제시키고
// 주의 : ex_del_fd()을 실행한 후 ex_adjust_poll()을 반드시 해주어라.
//--------------------------------------------------------------
void	ex_del_fd(int fd)
{
	int find_lp, del_lp;

	if(ex_mng_count <1)		return;

	for(find_lp=0; find_lp < ex_mng_count ;find_lp++)
	{
		if(ex_mng_list[find_lp].fd ==fd)
		{
			// 삭제한다.
			for(del_lp =find_lp ; del_lp < ex_mng_count-1 ;del_lp++ )
			{
				ex_mng_list[del_lp]= ex_mng_list[del_lp+1];
			}
			ex_mng_count--;		

			// 맨 마지막 것을 clear한다.
			ex_mng_list[del_lp].fd 			=-1;
			ex_mng_list[del_lp].priv 		= NULL;
			ex_mng_list[del_lp].on_read		= NULL;
			ex_mng_list[del_lp].on_write	= NULL;
		}
	}

}

//--------------------------------------------------------------
// 설명 : poll 처리가 가능하도록 최적화 하게 플레그를 세운다. 
//       :ex_loop() POLL 하기 전에 필요하다.
//--------------------------------------------------------------
int ex_adjust_poll( void)
{	
	ex_adjust_poll_req = 1;	
	return 0;	
}

//--------------------------------------------------------------
// 설명 : poll 처리가 가능하도록 최적화 한다. 
//       :ex_loop() POLL 하기 전에 필요하다. 
//--------------------------------------------------------------
static int ex_adjust_poll_implement( void)
{
	int lp;
	short events;
	
	ex_poll_count = 0;								// 초기화 

	for(lp=0;lp<ex_mng_count;lp++)
	{
		if(ex_mng_list[lp].fd < 0) continue;

		events = 0;

		if(ex_mng_list[lp].on_read  !=NULL) events |= POLLIN;			//  read 함수가 등록되 있으면 POLLIN를 등록
		if(ex_mng_list[lp].on_write !=NULL) events |= POLLOUT;			//  on_write 함수가 등록되 있으면 POLLIN를 등록	

		if(events == 0) continue;

		ex_poll_list[ex_poll_count].fd      = ex_mng_list[lp].fd ;
		ex_poll_list[ex_poll_count].events  = events;
		ex_poll_list[ex_poll_count].revents = 0;

		ex_poll_index[ex_poll_count] 		= lp;

		ex_poll_count++;
	}

	return 0;
}
