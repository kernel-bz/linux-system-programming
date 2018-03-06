//------------------------------------------------------------------------------
// 파 일 명 : ex.h
// 프로젝트 : fwp
// 설    명 :
// 작 성 자 : 유영창, 이영민, 문경원
// 작 성 일 : 2007년 5월 25일
// 수 정 일 :
// 수 정 일 :
// 주    의 :
// 라이센스 : GPL V2
//------------------------------------------------------------------------------

#ifndef _EX_HEADER_
#define _EX_HEADER_

#define EX_POLL_LIST_MAX            256
#define EX_WAIT_TIME_DEFAULT        20              // 20 mSec

#define  EXERR_NONE					0                                               //EXERR_NONE
#define  EXERR_WAIT_TIME			1                                               //EXERR_WAIT_TIME
#define  EXERR_POLL_FUNC			2                                               //EXERR_POLL_FUNC
#define  EXERR_POLL_ADD_FD			3                                               //EXERR_POLL_ADD_FD
#define  EXERR_POLL_ADD_MEM			4                                               //EXERR_POLL_ADD_MEM

typedef struct
{
	int 	fd;
	void 	*priv;

	int		(*on_read	)(void * self);
	int		(*on_write	)(void * self);
} ex_mng_t;

typedef struct pollfd pollfd_t;

#ifdef __cplusplus
extern "C"
{
#endif



	int			ex_init					( int wait_time	);
	void		ex_print_error			( char *msg		);

	ex_mng_t *	ex_add_fd				( int fd		);
	int			ex_adjust_poll			( void			);

	int			ex_loop					( void 			);

#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------
//
// 전역 변수 정의
//
//------------------------------------------------------------------------------

#ifndef _EX_VAR_

extern int 	(*ex_on_poll_timeout)(void			);
extern int 	(*ex_on_hooking_loop)(int poll_state);

#endif

#endif  // _EX_HEADER_
