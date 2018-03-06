//------------------------------------------------------------------------------
// 파 일 명 : ix.c
// 프로젝트 : fwp
// 설    명 :
// 작 성 자 : 유영창, 이영민, 문경원 , 이원재
// 작 성 일 : 2009년 1월 17일
// 수 정 일 :
// 수 정 일 :
// 주    의 :
// 라이센스 : GPL V2
// to do list :
//  1. ix_scan_device()  : sprintf(filename_buf,"/sys/class/input/input%d/name",ix_mng[lp].input_number );
//     과연 이정보가 필요한가 ? 만약 이 정보가 없다면 error로 retrun 해야 하는가?
//	   내가 가지고 있는 보드에는 /sys/class/input/input0 정보가 없다. 그래도 event0가
//	   있기 때문에 입력 값을 얻어오는 데는 문제 없다.

//  3. press버튼은 버튼이 down되는 순간 값을 보내고 , ascii-2로 변환한다. 
//  4. 부가 기능 키 shitf, alt,...등의 키카 눌려 질 때는 code값을 전달하면 안된다. press에서 예를 들어 오른쪽 shtf키는 키값 6과 동일한 코드를 가진다. 
//     따라서 부가 기능 키가 눌려지면, key 값은0으로 해서 전달하자. 
//------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>

#include <sys/poll.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <linux/limits.h>				// PATH_MAX
#include <fcntl.h>                      // O_RDWR , O_NOCTTY 등의 상수 정의
#include <time.h>

#include "ex.h"
#include "ix.h"
#include "ux_lib.h"

#define _IX_VAR_ 1

//--------------------------------------------------------------
// 전역 변수
//--------------------------------------------------------------

#define ABS( x )   (((x) > 0 )? (x):-(x))		// 절대값 


static 	int				ix_error_code			= IXERR_NONE;				// 에러코드
static	ix_mng_t		ix_mng[IX_INPUT_DEVICE_MAX];
static	int				ix_mng_count			= 0;
static	ex_mng_t 		*input_obj[IX_INPUT_DEVICE_MAX];					// 입력 디바이스에 대한 오브젝트


static int				ix_sreen_x_min = 0;
static int				ix_sreen_y_min = 0;

static int				ix_sreen_x_max = IX_SCREEN_X_DEFAULT;
static int				ix_sreen_y_max = IX_SCREEN_Y_DEFAULT;

static int				ix_mouse_x = IX_SCREEN_X_DEFAULT/2;
static int				ix_mouse_y = IX_SCREEN_Y_DEFAULT/2;
static int				ix_key	    =0;											// KEY  입력값

static ix_btn_state_t	ix_button_state =0;											// 버튼 상태값

static unsigned long	ix_mouse_event_flag	 =0;									// 마우스 이벤트 발생 ( up ,down, move ..etc)
static unsigned long	ix_key_event_flag	 =0;									// key    이벤트 발생 ( up ,down ,press )
//static unsigned long	ix_mouse_double_click_flag=0;

static ix_event_func_t  ix_event_func = NULL;			// 함수 포인터 										//

static ix_cal_touch_info_t  	ix_cal_touch_info[IX_CAL_TOUCH_INFO_MAX];
static ix_adjust_touch_info_t  	ix_adjust_touch_info;

static char 			ix_capslock_flag 	=0;										// 키보드 caps lock flag
static char 			ix_numlock_flag 	=0;										// 키보드 caps lock flag

	   int  			ix_mouse_click_interval = IX_MOUSE_DOUBLE_CLICK_TIME_INTAVAL;		// 마우스 더블 클릭 타임 인터벌 

static struct timeval	ix_pre_time;

static char ix_adjust_flag =0;
static char ix_adjust_touch_flag =0;

static char ix_askii[128] = {
	KEY_RESERVED,

	KEY_RESERVED ,KEY_RESERVED ,KEY_RESERVED ,KEY_RESERVED ,KEY_RESERVED,
	KEY_RESERVED ,KEY_RESERVED ,KEY_RESERVED ,KEY_RESERVED ,KEY_RESERVED,

	KEY_RESERVED ,KEY_RESERVED ,KEY_RESERVED ,KEY_RESERVED ,KEY_RESERVED,
	KEY_RESERVED ,KEY_RESERVED ,KEY_RESERVED ,KEY_RESERVED ,KEY_RESERVED,

	KEY_RESERVED ,KEY_RESERVED ,KEY_RESERVED ,KEY_RESERVED ,KEY_RESERVED,
	KEY_RESERVED ,KEY_RESERVED ,KEY_RESERVED ,KEY_RESERVED ,KEY_RESERVED,


	KEY_RESERVED ,KEY_SPACE ,KEY_RESERVED ,KEY_RESERVED ,KEY_RESERVED,
	KEY_RESERVED ,KEY_RESERVED ,KEY_RESERVED ,KEY_RESERVED ,KEY_RESERVED,


	KEY_RESERVED ,KEY_RESERVED ,KEY_RESERVED ,KEY_RESERVED ,KEY_MINUS,
	KEY_DOT      ,KEY_SLASH    ,KEY_0		 ,KEY_1		   ,KEY_2	, 
	KEY_3,	KEY_4,	KEY_5,	KEY_6, KEY_7,

	KEY_8,	KEY_9,	KEY_RESERVED ,KEY_SEMICOLON , KEY_RESERVED,
	KEY_EQUAL, KEY_RESERVED, KEY_RESERVED,	KEY_RESERVED,

	KEY_A ,KEY_B, KEY_C, KEY_D ,KEY_E,
	KEY_F, KEY_G, KEY_H, KEY_I, KEY_J,
	KEY_K, KEY_L, KEY_M, KEY_N ,KEY_O,
	KEY_P, KEY_Q, KEY_R, KEY_S ,KEY_T,
	KEY_U, KEY_V, KEY_W, KEY_X ,KEY_Y,
	KEY_Z, 
	
	KEY_LEFTBRACE, KEY_RESERVED, KEY_RIGHTBRACE ,KEY_RESERVED ,KEY_RESERVED, KEY_RESERVED ,
	KEY_A, KEY_B, KEY_C, KEY_D ,KEY_E,
	KEY_F, KEY_G, KEY_H, KEY_I, KEY_J,
	KEY_K, KEY_L, KEY_M, KEY_N ,KEY_O,
	KEY_P, KEY_Q, KEY_R, KEY_S ,KEY_T,
	KEY_U, KEY_V, KEY_W, KEY_X ,KEY_Y,
	KEY_Z, 
	KEY_RESERVED, KEY_RESERVED, KEY_RESERVED ,KEY_RESERVED 
};
static char ix_key_to_askii[IX_KEY_PRESS_MAX]= {			
	ASKII_RESERVED , ASKII_RESERVED,

	ASKII_1 ,ASKII_2 ,ASKII_3 ,ASKII_4, 
	ASKII_5 ,ASKII_6 ,ASKII_7 ,ASKII_8 ,ASKII_9,ASKII_ZERO,		 // 1,2,3,4,5,6,7,8,9,0

	ASKII_MINUS, ASKII_EQUAL,									 // - =
	ASKII_RESERVED, ASKII_RESERVED,
	
	ASKII_Q,ASKII_W,ASKII_E,ASKII_R,ASKII_T,
	ASKII_Y,ASKII_U,ASKII_I,ASKII_O,ASKII_P,			// Q,W,E,R,T,Y,U,I,O,P	

	ASKII_OPENSQURE, ASKII_CLOSESQUARE,					// [ ]	
	ASKII_RESERVED, ASKII_RESERVED,	

	ASKII_A,ASKII_S,ASKII_D,ASKII_F,ASKII_G,
	ASKII_H,ASKII_J,ASKII_K,ASKII_L,					// A,S,D,F,G,H,J,K,L	
	
	ASKII_SEMICOLON,ASKII_SINGLEQUOTE,					// ; '  		
	ASKII_GRAVE, ASKII_RESERVED,ASKII_BACKSLASH,		// `  ASKII_RESERVED    

	ASKII_Z ,ASKII_X ,ASKII_C ,ASKII_V ,ASKII_B,		// Z,X,C,V,B,N,M
	ASKII_N ,ASKII_M,
	
	ASKII_COMMA,ASKII_DOT,ASKII_FORDWARDSLASH			// , . /
};


static char ix_key_to_askii_with_shift[IX_KEY_PRESS_MAX]= {	
	ASKII_RESERVED , ASKII_RESERVED,
		
	ASKII_AT, ASKII_HASH ,ASKII_DOLOR ,ASKII_PERCENT, 
	ASKII_CARAT , ASKII_AMPERSAND, ASKII_STAR,ASKII_OPENPARAN,ASKII_CLOSEPARAN,ASKII_EXCLAM ,

	ASKII_UNDERBAR, ASKII_PLUS,
	ASKII_RESERVED, ASKII_RESERVED,
	
	ASKII_q,ASKII_w,ASKII_e,ASKII_r,ASKII_t,			// q, w, e, r, t
	ASKII_y,ASKII_u,ASKII_i,ASKII_o,ASKII_p,			// y, u, i, o ,p

	ASKII_CURLYOPEN, ASKII_CURLYCLOSE,					// { }
	ASKII_RESERVED, ASKII_RESERVED,

	ASKII_a,ASKII_s,ASKII_d,ASKII_f,ASKII_g,			// a, s, d, f, g
	ASKII_h,ASKII_j,ASKII_k,ASKII_l,					// h, j, k,l
	
	ASKII_COLON , ASKII_QUOTES,							// : "
	ASKII_TILDE , ASKII_RESERVED , ASKII_PIPE,			// ~ ASKII_RESERVED |

	ASKII_z ,ASKII_x ,ASKII_c ,ASKII_v ,ASKII_b,		// z, x, c, v, b
	ASKII_n ,ASKII_m,									// n, m, 

	ASKII_OPENANGLE, ASKII_CLOSEANGLE, ASKII_QUESTION   // < > ?
};

//--------------------------------------------------------------
// 함수 
//--------------------------------------------------------------
static int ix_get_key_press_value(int *m_key, ix_btn_state_t button_state);


//--------------------------------------------------------------
// 설명 :
// 매계 :
// 반환 : 성공0, 실패-1
//--------------------------------------------------------------
int ix_init (int mode)
{
	// 전역 변수 초기화 ... 나중에 하자.
	ix_event_func = NULL;

	memset(ix_cal_touch_info, 0, sizeof(ix_cal_touch_info_t)*IX_CAL_TOUCH_INFO_MAX);
	memset(&ix_adjust_touch_info, 0, sizeof(ix_adjust_touch_info_t));
	
	gettimeofday( &ix_pre_time, NULL );

	//터치스크린 보정정보 설정
	ix_adjust_touch_flag = 0;
	if (mode > 0) ix_adjust_touch_init ();
	
	return 0;
}

//--------------------------------------------------------------
// 설명 :
//--------------------------------------------------------------
static char  *ix_error_string( void )
{

	char *error_string[] ={ "에러 없음",                                    			//IXERR_NONE
							"ix_read_event_data_low 함수 read 에러 ",               	//IXERR_READ
							"ix_read_event_data_low 함수 fd 에러 ",						//IXERR_FD
							"ix_scan_device 함수 장치 정보파일 오픈 에러",				//IXERR_DEV_INFO_OPEN
							"ix_auto_scan_open 함수 에서 ix_scan_device 에러",			//IXERR_AUTOSCAN_SCANDEVICE
							"ix_auto_scan_open 함수 에서 mknod 에러",					//IXERR_AUTOSCAN_MKNOD
							"ix_auto_scan_open 함수 에서 디바이스 파일 오픈",			//IXERR_AUTOSCAN_DEVICE_OPEN
							"ix_auto_scan_open 함수 에서 ex 함수 등록 에러",			//IXERR_AUTOSCAN_EX_ADD
							"ix_write_event_low 함수 에서 잘못된 인자 에러",			//IXERR_WRITE_EVENT
							"ix_write_mouse_button_action 함수 에서 잘못된 인자 에러",	//IXERR_WRITE_MOUSE_BUTTON_EVENT
							"ix_write_keyboard_key_action 함수 에서 잘못된 인자 에러",	//IXERR_WRITE_KEYBOARD_EVENT

                        };
	return( error_string[ix_error_code]);

	//printf( "\r[gx error:%d]%s %s\n", ex_error_code, ex_error_string(), msg);
}

//--------------------------------------------------------------
// 설명 : ixlib에 에러코드, 에러문자열 및 사용자 메세지 출력
// 매개 : 사용자 메세지
//--------------------------------------------------------------
void ix_print_error( char *msg )
{
	printf( "\r[ix error:%d]%s %s\n", ix_error_code, ix_error_string(), msg);
	//return gx_error_code;
}


//--------------------------------------------------------------
// 설명 : 이벤트 구조로 데이터를 읽어 오는것
// 인자 : fd: 파일 디스크립션 , event_data: 이벤트 데이터
// 반환 : 정상 :읽어온 데이터 사이즈  , 실패 : -1
//--------------------------------------------------------------
int ix_read_event_data_low(int fd, ix_event_t *event_data )
{
	int	read_size = 0;

	if(fd <0)
	{
		ix_error_code = IXERR_FD; 		// 에러
		return -1;
	}

	read_size = read( fd, event_data, sizeof(ix_event_t));

	if(read_size != sizeof(ix_event_t))
	{
		ix_error_code = IXERR_READ; 	// 읽어온 데이터가 올바르지 않으면 에러다.
		return -1;
	}

	return read_size;

}

//--------------------------------------------------------------
// 설명 : 이벤트 구조로 데이터를 읽어 오는것
//--------------------------------------------------------------
void ix_debug_disp_event_data( ix_event_t *event_data )
{

	char *ix_code_rel_str[] = {
								"REL_X        ",
								"REL_Y        ",
								"REL_Z        ",
								"REL_RX       ",
								"REL_RY       ",
								"REL_RZ       ",
								"REL_HWHEEL   ",
								"REL_DIAL     ",
								"REL_WHEEL    ",
								"REL_MISC     ",
							};

	char *ix_code_abs_str[] = {
								"ABS_X         ",
								"ABS_Y         ",
								"ABS_Z         ",
								"ABS_RX        ",
								"ABS_RY        ",
								"ABS_RZ        ",
								"ABS_THROTTLE  ",
								"ABS_RUDDER    ",
								"ABS_WHEEL     ",
								"ABS_GAS       ",
								"ABS_BRAKE     ",
								"ABS_HAT0X     ",
								"ABS_HAT0Y     ",
								"ABS_HAT1X     ",
								"ABS_HAT1Y     ",
								"ABS_HAT2X     ",
								"ABS_HAT2Y     ",
								"ABS_HAT3X     ",
								"ABS_HAT3Y     ",
								"ABS_PRESSURE  ",
								"ABS_DISTANCE  ",
								"ABS_TILT_X    ",
								"ABS_TILT_Y    ",
								"ABS_TOOL_WIDTH",
							};

	//printf("[Sec]%ld, [uSec]%ld ",event_data->time.tv_sec, event_data->time.tv_usec );

	switch(event_data->type)
	{
    case    EV_SYN :  	printf("EV_SYN  ");		break;
	case    EV_KEY :  	printf("EV_KEY  ");		break;
	case    EV_ABS :    printf("EV_ABS  ");		break;      // 터치 스크린 입력
	case    EV_REL :    printf("EV_ABS  ");		break;

    default : 	printf("[type]:%02x! " ,event_data->type); 			break;
	}

	switch(event_data->type)
	{
 	case    EV_ABS :    // 절대값
 		printf("[code:%d]%s , [value]:%d  \n",
 					event_data->code ,
 					ix_code_abs_str[event_data->code],
 					event_data->value
 			   );
 		break;

	case    EV_REL :   	// 상대값
 		printf("[code:%d]%s , [value]:%d  \n",
 					event_data->code ,
 					ix_code_rel_str[event_data->code],
 					event_data->value
 			   );
		break;

    default : 	printf("[code]:%d ,[value]:%d \n" ,event_data->code ,event_data->value );
   		break;
	}
	
}
//--------------------------------------------------------------
// 설명 : ix_scan_device 으로 얻어온 입력 장치에 대한 정보 화면에 표시한다. 
//--------------------------------------------------------------
void ix_debug_disp_ix_device_info( void )
{
	int lp=0;
	printf("=========================================\n");
	printf("==     input device infomation         ==\n");
	printf("=========================================\n");

	for(lp = 0 ; lp < ix_mng_count ; lp++)
	{
		printf(" [device fd   ]:%d  \n" ,ix_mng[lp].fd );
		printf(" [device name ]:%s  \n" ,ix_mng[lp].name );
		printf(" [input number]:%d  \n" ,ix_mng[lp].input_number );
		printf(" [event number]:%d  \n" ,ix_mng[lp].event_number );
		printf(" [event major ]:%d  \n" ,ix_mng[lp].dev_major );
		printf(" [event minor ]:%d  \n" ,ix_mng[lp].dev_minor );
		printf(" [be touch    ]:%d  \n" ,ix_mng[lp].is_touch  );
		printf("-----------------------------------------------\n");
	}	

	if(!ix_mng_count )
		printf("no input device scan data!!!\n");

}

//--------------------------------------------------------------
// 설명 : 키보드 값을 읽어 온다.
// down : l
//--------------------------------------------------------------
int ix_read_keyboard_low(int fd , int *key, int *key_state )
{
	ix_event_t	 event_data;

	*key 		= KEY_RESERVED ;				// 초기화
	*key_state 	= 0;							// 초기화

	while(1)
	{
		if(ix_read_event_data_low(fd , &event_data ))	return -1;

		//ix_debug_disp_event_data(&event_data );

		if(event_data.type == EV_KEY)
		{
			*key 		= event_data.code;
			*key_state	= event_data.value;
		}
		if(event_data.type == EV_SYN)			break;			// syn가 오면 나간다.
	}

	return 0;
}


//--------------------------------------------------------------
// 설명 : 키보드 값을 읽어 온다.
//        키보드를 누른값
//        키가 눌리면 데이터가 두번 온다.
//[Sec]665, [uSec]902491 EV_KEY  [code]:2 ,[value]:1       //value가 1이면 down
//[Sec]665, [uSec]902604 EV_SYN  [code]:0 ,[value]:0
// 따라서  key 값 2를 syn 될때 보네기 위해
//--------------------------------------------------------------
int ix_read_key_press_low(int fd , int *key )
{
	int 	key_state =0;					// syn하기 이전값 보전
	ix_event_t	 event_data;

	*key 	= KEY_RESERVED ;				// 초기화


	while(1)
	{
		if(ix_read_event_data_low(fd , &event_data ))	return -1;

		if(event_data.type == EV_KEY)
		{
			*key 		= event_data.code;
			key_state	= event_data.value;
		}
		
		if(	 (event_data.type	== EV_SYN			)
		   &&(key_state			== IX_KEY_VAULE_DOWN_LOW))
		{		
			break;			// 키가 눌린 후 syn가 오면 나간다.
		}
	}	
	return 0;
}


//--------------------------------------------------------------
// 설명 : mouse 값을 읽어 온다.
// 인자 :
//		*x	: REL_X 값
//		*y	: REL_Y 값
//		*btn: code 값을 이용하여 마우스 버튼 위치를 넘겨준다.
//			  버튼은 동시에 여러게 누릴 수 있음으로 비트 마스크 하여 동시에 버튼이 눌리는 상태를 전달한다.
//
//  [예시1. 마우스 오른쪽버튼을 누르고, 마우스를 움직일 경우]
//	type = 	EV_KEY, code = BTN_TOUCH,	value = 1    <- 터치 스크린 마우스 왼쪽 버튼이 눌림
//	type = 	EV_ABS, code = ABS_X,		value = -14  <- X 좌표값은 -14이다.
//	type = 	EV_ABS, code = ABS_Y,		value = 99   <- Y 좌표값은 +99이다.
//	type = 	EV_SYN, code = 0,			value = 1    <- 지금까지 데이터가 하나의 데이터이다.
//
//	버튼이 눌린 상태에서 , 마우스를 움직이는 값을 확인 하려면,
//--------------------------------------------------------------
int ix_read_touch_low(int fd , int *x, int *y, int *press )
{

	ix_event_t	 event_data;

	while(1)
	{
		if(ix_read_event_data_low(fd , &event_data ))	return -1;

		//ix_debug_disp_event_data(&event_data );

		if( (event_data.type	== EV_KEY)
		   &&(event_data.code	== BTN_TOUCH))
		{
			*press = event_data.value;
		}

		if( event_data.type	== EV_ABS)
		{
			if(event_data.code == ABS_X)	*x = event_data.value;

			if(event_data.code == ABS_Y)	*y = event_data.value;
		}

		if(event_data.type == EV_SYN)			break;			// syn가 오면 나간다.
	}
	return 0;
}


//--------------------------------------------------------------
// 설명 : mouse 값을 읽어 온다.
// 인자 :
//		*x	: REL_X 값
//		*y	: REL_Y 값
//		*btn: code 값을 이용하여 마우스 버튼 위치를 넘겨준다.
//			  버튼은 동시에 여러게 누릴 수 있음으로 비트 마스크 하여 동시에 버튼이 눌리는 상태를 전달한다.
//
//  [예시1. 마우스 오른쪽버튼을 누르고, 마우스를 움직일 경우]
//	type = 	EV_KEY, code = BTN_LEFT,	value = 1    <- 마우스 왼쪽 버튼이 눌림
//	type = 	EV_REL, code = REL_X,		value = -14  <- X 좌표값은 -14이다.
//	type = 	EV_REL, code = REL_Y,		value = 99   <- Y 좌표값은 +99이다.
//	type = 	EV_SYN, code = 0,			value = 1    <- 지금까지 데이터가 하나의 데이터이다.
//
//	버튼이 눌린 상태에서 , 마우스를 움직이는 값을 확인 하려면,
//--------------------------------------------------------------
int ix_read_mouse_low(int fd , int *x, int *y, int *btn)
{
	ix_event_t	 event_data;

	while(1)
	{
		if(ix_read_event_data_low(fd , &event_data ))	return -1;

		//ix_debug_disp_event_data(&event_data );

		if( event_data.type	== EV_KEY)
		{
			switch(event_data.code)
			{
			case BTN_LEFT	:
					if(event_data.value == 1)	*btn |= IX_MOUSE_BTN_LEFT;		// 마우스 왼쪽 버튼이 눌림
					else						*btn &= ~IX_MOUSE_BTN_LEFT;		// 마우스 왼쪽 버튼이 눌림 해제
				 break;

			case BTN_RIGHT	:
					if(event_data.value == 1)	*btn |= IX_MOUSE_BTN_RIGHT;		// 마우스 오른쪽 버튼이 눌림
					else						*btn &= ~IX_MOUSE_BTN_RIGHT;	// 마우스 오른쪽 버튼이 눌림 해제
				 break;

			case BTN_MIDDLE :
					if(event_data.value == 1)	*btn |= IX_MOUSE_BTN_MIDDLE;	// 마우스 가운데 버튼이 눌림
					else						*btn &= ~IX_MOUSE_BTN_MIDDLE;	// 마우스 가운데 버튼이 눌림 해제
				 break;
			}
		}

		if( event_data.type	== EV_REL)
		{
			if(event_data.code == REL_X)	*x = event_data.value;				// x 값을 저장

			if(event_data.code == REL_Y)	*y = event_data.value;				// y 값을 저장
		}

		if(event_data.type == EV_SYN)			break;			// syn가 오면 나간다.
	}
	return 0;
}



//--------------------------------------------------------------
// 설명 : 모든 디바이스를 찾아서 정리한다.
// 수정 : imesu 2009년 2월 13일
//         cat /proc/bus/input/devices 에서 input에 대한 정보가 없어도
//         event에 대한 정보는 있는 경우가 있다.
//         이경우 S: Sysfs=/class/input/input 에서 디바이드 이름 정보를 얻어 올 수 없는데
//         이 디바이스 정보는 /proc/bus/input/devices 에서 얻어 올 수 있기 때문에 수정하였다.
//         input 에 대한 정보가 없다면  -1을 저장하게 수정하였다.
//--------------------------------------------------------------
int ix_scan_device(void)
{
	FILE *fp;
	char read_buf[1024];
	char filename_buf[PATH_MAX];			// 4096

	char *name_ptr = NULL;
	char *tmp_ptr = NULL;

	int tmp_num1=0;
	int tmp_num2=0;

	int lp=0;

	char *find_str;

	fp = fopen(FILE_NAME_INPUT_INFO_BASE, "r");
	if(fp == NULL)
	{
		ix_error_code = IXERR_DEV_INFO_OPEN;
		return -1;
	}

	ix_mng_count = 0;		// 초기화

	memset(ix_mng , 0, sizeof(ix_mng_t)*IX_INPUT_DEVICE_MAX);  // 초기화


	while(!feof(fp))
	{
		if(ix_mng_count >= IX_INPUT_DEVICE_MAX) 	break;

		memset(read_buf, 0, sizeof(read_buf));

		fgets(read_buf, sizeof(read_buf)-1, fp );			// 한 줄씩 읽어온다.
		//printf("[line data]%s" ,read_buf );
		if(read_buf[0] == 'S')			//	입력 디바이스 번호를 스캔한다.
		{
			sscanf(read_buf, "S: Sysfs=/class/input/input%d" ,&tmp_num1);
			ix_mng[ix_mng_count].input_number = tmp_num1;
		}

		if(read_buf[0] == 'H')			//	이벤트  디바이스 번호를 스캔한다.
		{
			find_str = strstr(read_buf, "event");			// 문자열에 "event" 가 있은 곳을 검색하고 , 있다면 부분 포인터를 넘겨준다.
			if(find_str !=NULL)
			{
				sscanf(find_str, "event%d" ,&tmp_num1 );
				ix_mng[ix_mng_count].event_number = tmp_num1;
			}
			else //imesu 2월 13일 추가
			{
				ix_mng[ix_mng_count].event_number = -1;
			}
		}

		if(read_buf[0] == 'N')							// imesu 2월 13일 추가 , 아래 주석처리한 부분을 대체하는 부분
		{
			name_ptr = strchr(read_buf,'=');
			name_ptr++;  								// = 은 필요 없어서 제외
			tmp_ptr = strchr(read_buf,'\n');
			//printf(" %s \n",name_ptr);

			strncpy(ix_mng[ix_mng_count].name , name_ptr , tmp_ptr - name_ptr);		//
		}

		if(	read_buf[0] ==0x0a)	ix_mng_count++;	          // 0x0a 는 LF 즉 '\r'
	}
	fclose(fp);

	// 입력 장치 major , minor 번호를 얻는다.
	for(lp = 0 ; lp < ix_mng_count ; lp++)
	{
		sprintf(filename_buf,"/sys/class/input/event%d/dev" ,ix_mng[lp].event_number );

		fp = fopen(filename_buf, "r");
		if(fp !=NULL)
		{
			fscanf(fp , "%d:%d" ,&tmp_num1 ,&tmp_num2 );

			ix_mng[lp].dev_major = tmp_num1;				// major 번호 저장
			ix_mng[lp].dev_minor = tmp_num2;				// minor 번호 저장
			fclose(fp);
		}
		
		// 터치 디바이스 인가를 확인한다. 
		sprintf(filename_buf,"/sys/class/input/input%d/capabilities/abs" ,ix_mng[lp].input_number );

		fp = fopen(filename_buf, "r");
		if(fp !=NULL)
		{
			fscanf(fp , "%d" ,&tmp_num1  );
			
//			printf( "FILE %s [%d]\n", filename_buf, tmp_num1 );
			
			if( tmp_num1 >= 3 ) ix_mng[lp].is_touch = 1;			// 터치 디바이스 이다. 
			else                ix_mng[lp].is_touch = 0;			// 터치 디바이스 아니다. 

			fclose(fp);
		}	
				
	}

	// 디버그 메시지
//#if 0
	for(lp = 0 ; lp < ix_mng_count ; lp++)
	{
		printf (" [device name ]:%s  \n" ,ix_mng[lp].name );
		printf (" [input number]:%d  \n" ,ix_mng[lp].input_number );
		printf (" [event number]:%d  \n" ,ix_mng[lp].event_number );
		printf (" [event major ]:%d  \n" ,ix_mng[lp].dev_major );
		printf (" [event minor ]:%d  \n" ,ix_mng[lp].dev_minor );
		printf ("\n");
	}
//#endif
	printf("input device scan complete!!!!\n");

	return 0;
}



//--------------------------------------------------------------
// 설명 : 사용자 영역에 이벤트를 전송한다.
//--------------------------------------------------------------
static int ix_copy_to_user()
{
	ie_event_t 		ie_data;							// input event data;
	int 			mx, my;
	int 			mkey;								// 키보드 입력값

	ix_get_mouse_position(&mx , &my);					// 마우스 좌표값을 넘겨준다.

	ix_get_key_value(&mkey);							// keyboard 입력 값을 넘겨준다.

	if( ix_mouse_event_flag & IX_MOUSE_FLAG_BTN_DOWN )
	{
		//printf("mouse down\n");
		ix_mouse_event_flag 	&= ~IX_MOUSE_FLAG_BTN_DOWN;
		ie_data.data.mouse.x 	= mx;							// x 좌표값
		ie_data.data.mouse.y 	= my;							// y 좌표값

		//printf("mx:%d, my:%d ",mx, my);	TRACE;
		ie_data.data.mouse.state= ix_button_state;				// 상태값
		ie_data.type 		 	= IE_MOUSE_DOWN;
		if(ix_event_func != NULL)  ix_event_func(&ie_data);
	}

	if( ix_mouse_event_flag & IX_MOUSE_FLAG_MOVE )
	{
		//printf("mouse move\n");
		ix_mouse_event_flag 	&= ~IX_MOUSE_FLAG_MOVE;
		ie_data.data.mouse.x	 = mx;
		ie_data.data.mouse.y	 = my;
		ie_data.data.mouse.state = ix_button_state;				// 상태값
		ie_data.type 			 = IE_MOUSE_MOVE;
		if(ix_event_func != NULL)  ix_event_func(&ie_data);
	}

	if( ix_mouse_event_flag & IX_MOUSE_FLAG_BTN_UP )
	{
		//printf("mouse up\n");
		ix_mouse_event_flag 	&= ~IX_MOUSE_FLAG_BTN_UP;
		ie_data.data.mouse.x	 = mx;
		ie_data.data.mouse.y	 = my;
		ie_data.data.mouse.state = ix_button_state;				// 상태값
		ie_data.type 			 = IE_MOUSE_UP;
		if(ix_event_func != NULL)  ix_event_func(&ie_data);
	}

	if( ix_key_event_flag & IX_KEY_VAULE_DOWN )					// key 가 눌렸다.
	{
		//printf("IX_KEY_VAULE_DOWN  \n");
		ix_key_event_flag		&= 	~IX_KEY_VAULE_DOWN;
		ie_data.data.key.key	 = 	mkey;
		ie_data.type 			 = 	IE_KEY_DOWN;
		ie_data.data.key.state	 =	ix_button_state;				// 상태값
		if(ix_event_func != NULL)	ix_event_func(&ie_data);
	}

	if( ix_key_event_flag & IX_KEY_VAULE_UP )						// key 가 눌림이 해제 되었다.
	{
		//printf("IX_KEY_VAULE_UP  \n");
		ix_key_event_flag    	&= 	~IX_KEY_VAULE_UP;
		ie_data.data.key.key	= 	mkey;
		ie_data.type 			= 	IE_KEY_UP;
		ie_data.data.key.state  = 	ix_button_state;				// 상태값
		if(ix_event_func != NULL)	ix_event_func(&ie_data);
	}

	if( ix_key_event_flag & IX_KEY_VAULE_PRESS )					// key 값을 사용자가 직관적으로 알수 있는 아스키 값으로 변환 
	{ 
		ix_get_key_press_value(&mkey, ix_button_state);
	
		//printf("IX_KEY_VAULE_PRESS %d \n", mkey);
		ix_key_event_flag   	&= 	~IX_KEY_VAULE_PRESS;
		ie_data.data.key.key	= 	mkey;
		ie_data.type 			= 	IE_KEY_PRESS;
		ie_data.data.key.state  =	ix_button_state;				// 상태값
		if(ix_event_func != NULL)	ix_event_func(&ie_data);
	}	
	return 0;
}

//--------------------------------------------------------------
// 설명 : 마우스 더블 클릭 간격 설정 
// 인자 : msec : 마우스 입력이 들어오는 시간 간격, 단위는  msec이다. 
//--------------------------------------------------------------
int  ix_set_mouse_double_click_intaval(int msec)
{
	ix_mouse_click_interval = msec;
	
	return ix_mouse_click_interval;
}


//--------------------------------------------------------------
// 설명 : 마우스 더블 클릭을 확인한다.
// 반환 : 1  :  더블클릭 성공 ,
//        0 :  더블클릭 실패 
//--------------------------------------------------------------
static int  ix_check_mouse_double_click( struct timeval cur_time)
{
/*
	int intaval=0;
	
	// 시간 확인 
	intaval = (cur_time.tv_usec - ix_pre_time.tv_usec);

	printf("cur :%ld, old :%ld, intaval :%d \n", cur_time.tv_usec,ix_pre_time.tv_usec, intaval );
	if(intaval < ix_mouse_click_interval)
	{	// 초기화 
		ix_mouse_double_click_flag++;
		if(ix_mouse_double_click_flag ==2 )
		{
			printf(">>>>>>> Double \n");
			ix_mouse_double_click_flag =0;
			ix_pre_time  = cur_time;	
			return 1;
		}
	}
	else 
	{
		//ix_mouse_double_click_flag=1;
		//ix_pre_time  = cur_time;	
		return 0;
	}
*/
	return 0;
	
}



//--------------------------------------------------------------
// 설명 : input event tye key 입력 처리
//
//--------------------------------------------------------------
static int ix_input_event_type_key(ix_event_t event_data)
{
	//printf("[code] 0x%x\n",event_data.code );
	switch(event_data.code)
	{
	case BTN_TOUCH  :					// touh 입력은 마우스의 left와 동일한 입력이다.
	case BTN_LEFT	:
			if(event_data.value)	ix_button_state |= IX_MOUSE_BTN_LEFT;			// 마우스 왼쪽 버튼이 눌림
			else					ix_button_state &= ~IX_MOUSE_BTN_LEFT;			// 마우스 왼쪽 버튼이 눌림 해제
		 	break;

	case BTN_RIGHT	:
			if(event_data.value)	ix_button_state |= IX_MOUSE_BTN_RIGHT;			// 마우스 오른쪽 버튼이 눌림
			else					ix_button_state &= ~IX_MOUSE_BTN_RIGHT;			// 마우스 오른쪽 버튼이 눌림 해제
		 	break;

	case BTN_MIDDLE :
			if(event_data.value)	ix_button_state |= IX_MOUSE_BTN_MIDDLE;			// 마우스 가운데 버튼이 눌림
			else					ix_button_state &= ~IX_MOUSE_BTN_MIDDLE;		// 마우스 가운데 버튼이 눌림 해제
			break;

	case KEY_LEFTSHIFT :
			if(event_data.value)	ix_button_state |= IX_KEYBOARD_BTN_LEFT_SHIFT;		// 키보드 왼쪽 shitf 버튼이 눌림
			else					ix_button_state &= ~IX_KEYBOARD_BTN_LEFT_SHIFT;	// 키보드 왼쪽 shitf 버튼이 눌림 해제
		 	break;

	case KEY_RIGHTSHIFT   :
			if(event_data.value)	ix_button_state |= IX_KEYBOARD_BTN_RIGHT_SHIFT;	// 키보드 오른쪽 shitf 버튼이 눌림
			else					ix_button_state &= ~IX_KEYBOARD_BTN_RIGHT_SHIFT;	// 키보드 오른쪽 shitf 버튼이 눌림 해제
		 	break;

	case KEY_LEFTALT    :
			if(event_data.value)	ix_button_state |= IX_KEYBOARD_BTN_LEFT_ALT;		// 키보드 왼쪽쪽 ALT 버튼이 눌림
			else					ix_button_state &= ~IX_KEYBOARD_BTN_LEFT_ALT;		// 키보드 오른쪽 ALT 버튼이 눌림 해제
		 	break;

	case KEY_RIGHTALT    :
			if(event_data.value)	ix_button_state |= IX_KEYBOARD_BTN_RIGHT_ALT;		// 키보드 오른쪽 ALT 버튼이 눌림
			else					ix_button_state &= ~IX_KEYBOARD_BTN_RIGHT_ALT;		// 키보드 오른쪽 ALT 버튼이 눌림 해제
		 	break;

	case KEY_LEFTCTRL     :
			if(event_data.value)	ix_button_state |= IX_KEYBOARD_BTN_LEFT_CTRL;		// 키보드 왼쪽쪽 CTRL 버튼이 눌림
			else					ix_button_state &= ~IX_KEYBOARD_BTN_LEFT_CTRL;		// 키보드 오른쪽 CTRL 버튼이 눌림 해제
			break;

	case KEY_RIGHTCTRL     :
			if(event_data.value)	ix_button_state |= IX_KEYBOARD_BTN_RIGHT_CTRL;		// 키보드 오른쪽 CTRL 버튼이 눌림
			else					ix_button_state &= ~IX_KEYBOARD_BTN_RIGHT_CTRL;	// 키보드 오른쪽 CTRL 버튼이 눌림 해제
			break;

	case KEY_CAPSLOCK     :	// imesu 090215 
			if(event_data.value)	ix_button_state |= IX_KEYBOARD_CAPSLOCK;		// 키보드 오른쪽 CTRL 버튼이 눌림
			else					ix_button_state &= ~IX_KEYBOARD_CAPSLOCK;		// 키보드 오른쪽 CTRL 버튼이 눌림 해제			
			break;


	case KEY_NUMLOCK     :	// imesu 090215 
			if(event_data.value)	ix_button_state |= IX_KEYBOARD_NUMLOCK;			// 키보드 Num Lock 버튼이 눌림
			else					ix_button_state &= ~IX_KEYBOARD_NUMLOCK;		// 키보드 Num Lock 버튼이 눌림 해제
			break;

	case KEY_UP     :		// imesu 090215 
			if(event_data.value)	ix_button_state |= IX_KEYBOARD_UP;				// 키보드 윗쪽 방향키 눌림 
			else					ix_button_state &= ~IX_KEYBOARD_UP;				// 키보드 윗쪽 방향키 눌림 해제
			break;

	case KEY_DOWN   :		// imesu 090215 
			if(event_data.value)	ix_button_state |= IX_KEYBOARD_DOWN;			// 키보드 아래쪽 방향키 눌림 
			else					ix_button_state &= ~IX_KEYBOARD_DOWN;			// 키보드 아래쪽 방향키 눌림 해제
			break;
			
	case KEY_LEFT    :		// imesu 090215  방향키 
			if(event_data.value)	ix_button_state |= IX_KEYBOARD_LEFT;			// 키보드 왼쪽 방향키 버튼 눌림 
			else					ix_button_state &= ~IX_KEYBOARD_LEFT;			// 키보드 왼쪽 방향키 버튼 툴림 해제
			break;

	case KEY_RIGHT    :		// imesu 090215 
			if(event_data.value)	ix_button_state |= IX_KEYBOARD_RIGHT;			// 키보드 오른쪽 방향키 버튼 눌림
			else					ix_button_state &= ~IX_KEYBOARD_RIGHT;			// 키보드 오른쪽 방향키 버튼 눌림 해제 
			break;

	default : // 주의 : 버튼 말고 나머지는 키보드로 처리했다. 나중에 수정할 부분
		  //if(event_data.value == 1)	ix_button_state= event_data.value;				// key가 눌려있을때 값을 무조건 넣는다.
		  	break;
	}

	// 더블 클릭 확인 
	switch(event_data.code)
	{
	case BTN_TOUCH  :					// touh 입력은 마우스의 left와 동일한 입력이다.
	case BTN_LEFT	: 			
			if(ix_check_mouse_double_click(event_data.time))
			{
				if(event_data.value)	ix_mouse_event_flag |=IX_MOUSE_FLAG_DOUBLE_CLICK;	//마우스가 눌렸다.
				else					ix_mouse_event_flag &=~IX_MOUSE_FLAG_DOUBLE_CLICK;		// 마우스가 눌림 해제				
			}
			break;

	default :
		
			break;
	}
	
	// FLAG설정 
	switch(event_data.code)
	{
	case BTN_TOUCH  :					// touh 입력은 마우스의 left와 동일한 입력이다.
	case BTN_LEFT	:
	case BTN_RIGHT	:
	case BTN_MIDDLE :
			if(event_data.value)	ix_mouse_event_flag |=IX_MOUSE_FLAG_BTN_DOWN;	//마우스가 눌렸다.
			else					ix_mouse_event_flag |=IX_MOUSE_FLAG_BTN_UP;		// 마우스가 눌림 해제
			break;

	case KEY_CAPSLOCK     :				// imesu 090215 버튼을 누르면 DOWN -> UP이 된다. 즉 2번ix_input_event_type_key()가 호출된다. 그래서 DOWN 될때 FLAG 설정하게 한다. 
			if(event_data.value)
			{
				ix_key_event_flag |= IX_KEY_VAULE_DOWN;								// 키보드 Num Lock 버튼이 눌림
				ix_key_event_flag |= IX_KEY_VAULE_PRESS;							// key가 눌렸다. low code 값을 ascii값으로 변환 ex) KEY_1 -> 0x30				

				if(!ix_capslock_flag)		ix_capslock_flag = FLAG_SET;			// CapsLock 기능이 설정 되었다. 
				else 						ix_capslock_flag = FLAG_RELEASE;		// CapsLock 기능이 해제 되었다.
			}
			else
			{
				ix_key_event_flag |= IX_KEY_VAULE_UP;										
			}

			break;


	case KEY_NUMLOCK     :	// imesu 090215 
			if(event_data.value)
			{
				ix_key_event_flag 		|= IX_KEY_VAULE_DOWN;						// 키보드 Num Lock 버튼이 눌림
				ix_key_event_flag 		|= IX_KEY_VAULE_PRESS;						// key가 눌렸다. low code 값을 ascii값으로 변환 ex) KEY_1 -> 0x30				

				if(!ix_numlock_flag) 	ix_numlock_flag = FLAG_SET;					// CapsLock 기능이 설정 되었다. 
				else 					ix_numlock_flag = FLAG_RELEASE;				// CapsLock 기능이 해제 되었다.
			}
			else
			{
				ix_key_event_flag |= IX_KEY_VAULE_UP;					
			}			
			break;	


	default : // 주의 : 버튼 말고 나머지는 키보드로 처리했다. 나중에 수정할 부분
			ix_key = event_data.code;								// key 값을 넣는다. 			
			
		  	if(event_data.value )
		  	{
		  		ix_key_event_flag |= IX_KEY_VAULE_DOWN;				// key가 눌렸다. low code 값을 전달
		  		ix_key_event_flag |= IX_KEY_VAULE_PRESS;			// key가 눌렸다. low code 값을 ascii값으로 변환 ex) KEY_1 -> 0x30
		  	}
		  	else
		  	{
		  		ix_key_event_flag |= IX_KEY_VAULE_UP;				// key가 눌렸다.
			}			
			break;
	}
	return 0;
}


//--------------------------------------------------------------
// 설명 :REL 입력 값을 처리한다. 
//--------------------------------------------------------------
static int ix_input_event_type_rel(ix_event_t event_data)
{
	if(event_data.code == REL_X)
	{
		ix_mouse_x += event_data.value;							// x 값을 저장

		if(ix_mouse_x < ix_sreen_x_min )	ix_mouse_x = ix_sreen_x_min;
		if(ix_mouse_x >= ix_sreen_x_max )	ix_mouse_x = ix_sreen_x_max-1;
	}
	if(event_data.code == REL_Y)
	{
		ix_mouse_y += event_data.value;							// y 값을 저장

		if(ix_mouse_y < ix_sreen_y_min )	ix_mouse_y = ix_sreen_y_min;
		if(ix_mouse_y >= ix_sreen_y_max )	ix_mouse_y = ix_sreen_y_max-1;
	}

	ix_mouse_event_flag |=IX_MOUSE_FLAG_MOVE;

	return 0;
}

//--------------------------------------------------------------
// 설명 : ABS 값 처리
//--------------------------------------------------------------
static int ix_input_event_type_abs(ix_event_t event_data)
{	

	int x, y;
	if(event_data.code == ABS_X)
	{
		ix_mouse_x = event_data.value;							// x 값을 저장		

		if(ix_adjust_touch_flag)
		{
			x = (ix_adjust_touch_info.xa * ix_mouse_x + ix_adjust_touch_info.xb) / IX_CAL_TOUCH_INFO_OFSSET;

			if(x <= 0)
			{
				x =0;
			}
			else if(x >= ix_sreen_x_max)
			{
				x = ix_sreen_x_max; 
			}

			ix_mouse_x = x;
		}
		
	}
	if(event_data.code == ABS_Y)
	{
		ix_mouse_y = event_data.value;							// y 값을 저장
		
		if(ix_adjust_touch_flag)
		{
			y = (ix_adjust_touch_info.ya * ix_mouse_y + ix_adjust_touch_info.yb) / IX_CAL_TOUCH_INFO_OFSSET;

			if(y <= 0)
			{
				y =0;
			}
			else if(y >= ix_sreen_y_max)
			{
				y = ix_sreen_y_max; 
			}

			ix_mouse_y = y;
		}			
	}

	ix_mouse_event_flag |= IX_MOUSE_FLAG_MOVE;

	printf("mx:%d, my:%d ",ix_mouse_x, ix_mouse_y);	TRACE;
	
}

//--------------------------------------------------------------
// 설명 : 이벤트 입력을 처리한다.
//		*on_self 입력들어온 입력을 처리한다.
//       터치입력은 마우스와 동일한 입력다.
//--------------------------------------------------------------
static int ix_event_read(void *on_self )
{
	ex_mng_t		*mng      = NULL;

	ix_event_t 		event_data;

	mng = (ex_mng_t *)on_self;

	ix_read_event_data_low(mng->fd , &event_data);

	//ix_debug_disp_event_data(&event_data);						// 이벤트 데이터 를 확인한다.

	switch(event_data.type)
	{
	case EV_KEY :	ix_input_event_type_key(event_data);	break;

	case EV_REL :	ix_input_event_type_rel(event_data);	break;

	case EV_ABS :	ix_input_event_type_abs(event_data);	break;

	case EV_SYN :	ix_copy_to_user();		break;				// 사용자 영역에 이벤트를 전송한다.

	default : break;
	}

	return 0;

}


//--------------------------------------------------------------
// 설명 : 검색된 장치를 mknod 한다.
// 			1)입력 장치를 자동 검색
// 			2) mknod
// 			3) 디바이스 파일 오픈..
// 			4) ex 함수 등록
// 매개 : path : 기본 디렉토리 위치
// 반환 :
// 주의 :
//--------------------------------------------------------------
int	ix_auto_scan_open( char *path )
{
	char 	device_path[PATH_MAX];							// 4096
	int		lp=0;

	// 1)입력 장치를 자동 검색
	if(ix_scan_device() <0)
	{
		ix_error_code = IXERR_AUTOSCAN_SCANDEVICE;
		return -1;
	}
	
	// 2) mknod
	// 3) 디바이스 파일 오픈..
	for( lp= 0 ; lp < ix_mng_count;lp++)
	{
		sprintf (device_path, "%s/event%d", path, ix_mng[lp].event_number);
		//printf ("ix_auto_scan_open(): device_path=%s\n", device_path);

		ix_mng[lp].fd = ux_mknod_device_open(device_path ,ix_mng[lp].dev_major ,ix_mng[lp].dev_minor, O_RDWR );

		if(ix_mng[lp].fd < 0)	continue;

		// 4) ex 함수 등록
		input_obj[lp] 			= ex_add_fd( ix_mng[lp].fd );

		if(input_obj[lp] == NULL) continue;

		input_obj[lp]->on_read  = ix_event_read;			// read
	}

	ex_adjust_poll();

	return 0;
}

//--------------------------------------------------------------
// 설명 : 터치 파일 디스크립터를 얻는다.  
//        자동으로 mknod 한다.
// 			1) 입력 장치중 터치 검색
// 			2) mknod
// 			3) 디바이스 파일 오픈..
// 			4) ex 함수 등록
// 매개 : path : 기본 디렉토리 위치
// 반환 :
// 주의 :
//--------------------------------------------------------------
int ix_get_touch_fd( char *path )
{
	char 	device_path[PATH_MAX];							// 4096
	int lp;
	
	for( lp= 0 ; lp < ix_mng_count;lp++)
	{
		if( !ix_mng[lp].is_touch ) continue;
			
		//printf( "INDEX %d\n" , lp );	
			
		sprintf(device_path, "%s/event%d", path, ix_mng[lp].event_number);
		ix_mng[lp].fd = ux_mknod_device_open(device_path ,ix_mng[lp].dev_major ,ix_mng[lp].dev_minor, O_RDWR );

		if(ix_mng[lp].fd >= 0 )	return ix_mng[lp].fd;
	}	
	
	return -1;
}

//--------------------------------------------------------------
// 설명 : 스크린 사이즈를 설정한다.
//        마우스 위치를 설정한다.
//--------------------------------------------------------------
int ix_set_screen_size(int x, int y)
{
	ix_sreen_x_max = x;
	ix_sreen_y_max = y;

	ix_mouse_x	= (ix_sreen_x_max - ix_sreen_x_min )/2 + ix_sreen_x_min ;
	ix_mouse_y	= (ix_sreen_y_max - ix_sreen_y_min )/2 + ix_sreen_y_min ;

	return 0;
}

//--------------------------------------------------------------
// 설명 : 마우스 위치를 얻어온다.
//--------------------------------------------------------------
int ix_get_mouse_position(int *x, int *y)
{
	*x = ix_mouse_x;
	*y = ix_mouse_y;

	return 0;
}

//--------------------------------------------------------------
// 설명 : 이벤트 데이터를 처리할 함수를 등록한다.
//--------------------------------------------------------------
int ix_add_event(ix_event_func_t func)
{
	ix_event_func = (ix_event_func_t) func;

	return 0;
}

//--------------------------------------------------------------
// 설명 : key 입력값을 얻어온다.
//--------------------------------------------------------------
int ix_get_key_value(int *key)
{
	*key = ix_key;

	return 0;
}


//--------------------------------------------------------------
// 설명 : 이벤트 장치에 사용자가 직접 이벤트 정보를 넣어서 이벤트를 발생시킨다.
// 주의 : 현재 장치를 구별할 방법에 없어서 모든 장치 이벤트 핸들러에 쓴다. 
//      : 장치를 구별할 방업에 생기면 이부분을 ix_write_event_low()함수로 대치해서 사용하자 
// imesu 2008-02-20
//--------------------------------------------------------------
static int ix_write_event_bulk_low(ix_event_t *event_data)
{
	int flag_write_event = -1;									// ix_write_event_low 함수의 에러를 체크함
	int lp =0;

	if(event_data == NULL)	return -1;
		
	for(lp = 0; lp < ix_mng_count; lp++)
	{
		if(!(ix_write_event_low(ix_mng[lp].fd, event_data )))
		{
			flag_write_event = 0;	//한번이라도 event write가 성공하면 에러를 무발생!
		}
	}
	if(flag_write_event <0)
	{
		return -1;	//ix_write_event_low함수 모두 실패
	}
	return 0;

}

//--------------------------------------------------------------
// 설명 : 이벤트 장치에 사용자가 직접 이벤트 정보를 넣어서 이벤트를 발생시킨다.
//--------------------------------------------------------------
int ix_write_event_low(int fd, ix_event_t *event_data )
{
	if( fd < 0 || event_data == NULL )
	{
		ix_error_code = IXERR_WRITE_EVENT; 		// 에러
		return -1;
	}

	write( fd, event_data, sizeof(ix_event_t));
	return 0;
}


//--------------------------------------------------------------
// 설명 : 마우스 특정버튼의 Up down 엑션을 발생시킨다.
//--------------------------------------------------------------
int ix_write_mouse_button_action(int mouse_button, int action)
{
	ix_event_t event_data;
	
	//step1: event 구조체의 공통된 값 설정
    gettimeofday( &event_data.time, NULL );
	event_data.type = EV_KEY;

	//step2: event 구조체의 버튼 설정값 설정
	switch(mouse_button)
	{
	case IX_MOUSE_BTN_LEFT:		event_data.code = BTN_LEFT;							break;
	case IX_MOUSE_BTN_RIGHT:	event_data.code = BTN_RIGHT;						break;
	case IX_MOUSE_BTN_MIDDLE:	event_data.code = BTN_MIDDLE;						break;
	default:					ix_error_code   = IXERR_WRITE_MOUSE_BUTTON_EVENT; 	return -1;	// 에러
	}

	//step3: event 구조체의 버튼 상태값 설정
	switch(action)
	{
	case IE_MOUSE_DOWN:	event_data.value = IX_MOUSE_VAULE_DOWN_LOW;					break;
	case IE_MOUSE_UP:	event_data.value = IX_MOUSE_VAULE_UP_LOW;					break;
	default:			ix_error_code    = IXERR_WRITE_MOUSE_BUTTON_EVENT; 		return -1;	// 에러
	}
	
	// 이벤트를 모든 핸들러에 쓴다. ... 나중에 장치 구별이 되면, 해당 핸들러에만 쓰게 수정하자.!! 
	if(ix_write_event_bulk_low(&event_data) <0)		return -1;

	//step4: SYC 발생
    //gettimeofday( &event_data.time, NULL );

	event_data.type  = EV_SYN;
	event_data.code  = SYN_REPORT;
	event_data.value = 0;


	// 이벤트를 모든 핸들러에 쓴다. ... 나중에 장치 구별이 되면, 해당 핸들러에만 쓰게 수정하자.!! 
	if(ix_write_event_bulk_low(&event_data) <0)		return -1;

	return 0;

}

//--------------------------------------------------------------
// 설명 : 마우스 위치 변경 이벤트를 발생시킨다.
// 반환 : 0 :성공, -1; 실패 
//--------------------------------------------------------------
int ix_write_mouse_rel_move(int rel_x, int rel_y)
{
	ix_event_t event_data;

	//step1: event 구조체의 공통된 값 설정
    gettimeofday( &event_data.time, NULL );
	event_data.type = EV_REL;
	
	//step2: event 구조체의 X 변경값 설정
	event_data.code = REL_X;
	event_data.value = rel_x;

	// 이벤트를 모든 핸들러에 쓴다. ... 나중에 장치 구별이 되면, 해당 핸들러에만 쓰게 수정하자.!! 
	if(ix_write_event_bulk_low(&event_data) <0)		return -1;
	//step3: event 구조체의 Y 변경값 설정
	event_data.code = REL_Y;
	event_data.value = rel_y;
	
	// 이벤트를 모든 핸들러에 쓴다. ... 나중에 장치 구별이 되면, 해당 핸들러에만 쓰게 수정하자.!! 
	if(ix_write_event_bulk_low(&event_data) <0)		return -1;
	
	//step4: SYC 발생

    //gettimeofday( &event_data.time, NULL );
	event_data.type  = EV_SYN;
	event_data.code  = SYN_REPORT;
	event_data.value = 0;

	// 이벤트를 모든 핸들러에 쓴다. ... 나중에 장치 구별이 되면, 해당 핸들러에만 쓰게 수정하자.!! 
	if(ix_write_event_bulk_low(&event_data) <0)		return -1;

	return 0;
}

//--------------------------------------------------------------
// 설명 : 키보드에 키 이벤트를 발생시킨다.
// 매개	:
//			keyboard_key: keyboard의 키 code를 의미함. 참고: Linux 커널소소/include/linux/input.h의 'Keys and buttons' 항목
//
//			action: 해당 키의 발생시키고 싶은 상태
//
//			IE_KEY_DOWN
//			IE_KEY_UP
//--------------------------------------------------------------
int ix_write_keyboard_key_action(int keyboard_key, int action)
{
	ix_event_t event_data;

	//step1: event 구조체의 공통된 값 설정
    gettimeofday( &event_data.time, NULL );
	event_data.type = EV_KEY;

	//step2: event 구조체의 KEY 값 설정
	event_data.code = keyboard_key;
	
	//step3: event 구조체의 액션 값 설정
	switch(action)
	{
	case IE_KEY_DOWN:	event_data.value = 1;							break;
	case IE_KEY_UP:		event_data.value = 0;							break;
	default:			ix_error_code    = IXERR_WRITE_KEYBOARD_EVENT; 	return -1;	// 에러
	}

	// 이벤트를 모든 핸들러에 쓴다. ... 나중에 장치 구별이 되면, 해당 핸들러에만 쓰게 수정하자.!! 
	if(ix_write_event_bulk_low(&event_data) <0)		return -1;

	//step4: SYC 발생
    //gettimeofday( &event_data.time, NULL );
	event_data.type  = EV_SYN;
	event_data.code  = SYN_REPORT;
	event_data.value = 0;

	// 이벤트를 모든 핸들러에 쓴다. ... 나중에 장치 구별이 되면, 해당 핸들러에만 쓰게 수정하자.!! 
	if(ix_write_event_bulk_low(&event_data) <0)		return -1;
	
	return 0;
}

//--------------------------------------------------------------
// 설명 : 입력 값을 사용자가 바로 사용할 수 있는 아스키 코드로 변환하여 반환한다. 
//        예를 들어 KEY_1은 0x30으로 변환하여 리턴한다. 
//        Shift , ALT, CTRL키도 고려한다. 
// http://lxr.falinux.com/source/include/linux/input.h 에 있는 키 값을 실제 키보드 배열대로 나열되어 揚獵? 
//--------------------------------------------------------------
static int ix_get_key_press_value(int *m_key, ix_btn_state_t button_state)
{
	int shift_flag	= 0;
	int askii		= 0;

	//printf("[*m_key]:0x%x ", *m_key);  TRACE;
	if(*m_key > IX_KEY_PRESS_MAX) return -1;
	
	if( (button_state & IX_KEYBOARD_BTN_LEFT_SHIFT)||(button_state & IX_KEYBOARD_BTN_RIGHT_SHIFT) )
	{	
		shift_flag = FLAG_SET;											// shift 가 눌려져 있으면 flag를 설정한다. 
	}

	if(KEY_1 <= *m_key && *m_key <=KEY_SLASH )		
	{
		askii = ix_key_to_askii[*m_key]; 								// KEY_XXX 입력 값을 아스키 코드 값으로 변환 

		if(ASKII_A <= askii && askii <= ASKII_Z)
		{			
			if( shift_flag == FLAG_SET || ix_capslock_flag == FLAG_SET )
			{
				askii =ix_key_to_askii_with_shift[*m_key]; 
			}
		}
		else 
		{
			if( shift_flag == FLAG_SET)	
			{
				askii =ix_key_to_askii_with_shift[*m_key]; 
			}
		}
	}	
	*m_key = askii;		
	return 0;	
}

//--------------------------------------------------------------
// 설명 : 터치 스크린 보정된 위치값을 얻어 온다. 
// imesu 2009-02-20
//--------------------------------------------------------------
int ix_touch_calcurator(int *x, int *y)
{
	int tx =0;	
	int ty =0;

	int sx = 0; 
	int sy = 0;
	
	ix_get_mouse_position(&tx,&ty);							// 보정전의 터치스크린 값을 얻어 온다. 

	printf("ix_adjust_touch_info.xa:%d ",ix_adjust_touch_info.xa);	TRACE;
	printf("ix_adjust_touch_info.xb:%d ",ix_adjust_touch_info.xb);	TRACE;
	printf("ix_adjust_touch_info.ya:%d ",ix_adjust_touch_info.ya);	TRACE;
	printf("ix_adjust_touch_info.yb:%d ",ix_adjust_touch_info.yb);	TRACE;

	if(ix_adjust_touch_info.xa ==0)	return -1;				// 분모가 0이면 에러다. 
		
	sx = (tx-ix_adjust_touch_info.xb)/ix_adjust_touch_info.xa;	
	if(sx <= 0)
	{
		sx =0;
	}
	else if(sx >= ix_sreen_x_max)
	{
		sx = ix_sreen_x_max; 
	}

	if(ix_adjust_touch_info.ya ==0)	return -1;				// 분모가 0이면 에러다. 
	
	sy = (ty-ix_adjust_touch_info.yb)/ix_adjust_touch_info.ya;			
	if(sy <= 0)
	{
		sy =0;
	}
	else if(sy >= ix_sreen_x_max)
	{
		sy = ix_sreen_x_max; 
	}
	*x = sx;
	*y = sy;

	return 0;
}

//--------------------------------------------------------------
// 설명 : 가상 키보드에서 받은 입력을 , 실제 입력 값으로 변환 ( 아스키 -> KEY_XX)
// imesu 2009-02-20
// 인자 : 
//         key :  아스키 코드값을 입력 받는다. 
// 사용법 : ix_write_key_press('A');  ix_write_key_press(3)
// 주의 : 인자로 문자 1개 만 보낸다. 
// TO DO : 소문자 처리 
//--------------------------------------------------------------
int ix_write_key_press(int ascii)
{
	int key_xxx =0;
	ix_event_t event_data;

	if(128 < ascii )	return -1;

	// 소문자가 들어 오면 소문자 처리 
	if(ASKII_a <= ascii && ascii<=ASKII_z)
	{
		ix_button_state |= IX_KEYBOARD_BTN_LEFT_SHIFT;
	}
	else
	{
		ix_button_state &= ~IX_KEYBOARD_BTN_LEFT_SHIFT;	// 키보드 왼쪽 shitf 버튼이 눌림 해?
		TRACE;
	}	
	
	key_xxx = ix_askii[ascii];

	//printf("key_xxx :%d , ascii:0x%x " ,key_xxx, ascii);	TRACE;	

	//step1: event 구조체의 공통된 값 설정
    gettimeofday( &event_data.time, NULL );
	event_data.type = EV_KEY;

	//step2: event 구조체의 KEY 값 설정
	event_data.code = key_xxx;
	
	//step3: event 구조체의 액션 값 설정
	event_data.value = 1;

	// 이벤트를 모든 핸들러에 쓴다. ... 나중에 장치 구별이 되면, 해당 핸들러에만 쓰게 수정하자.!! 
	if(ix_write_event_bulk_low(&event_data) <0)		return -1;

	//step4: SYC 발생
    //gettimeofday( &event_data.time, NULL );
	event_data.type  = EV_SYN;
	event_data.code  = SYN_REPORT;
	event_data.value = 0;

	// 이벤트를 모든 핸들러에 쓴다. ... 나중에 장치 구별이 되면, 해당 핸들러에만 쓰게 수정하자.!! 
	if(ix_write_event_bulk_low(&event_data) <0)		return -1;

	if(ix_write_keyboard_key_action(key_xxx, IE_KEY_UP) <0 ) return -1;
	
	return 0;	
}


//--------------------------------------------------------------
// 설명 : 가상 키보드에서 받은 입력열을 , 실제 입력 값으로 변환 ( 아스키 -> KEY_XX)
// imesu 2009-02-20
// 인자 : 
//         str :  문자열을 입력 받는다. 
//         size : 문자열의 사이즈 
// 사용법 : ix_write_key_press('ABC', 3);
//--------------------------------------------------------------
int ix_write_key_string_press(char *str, int size)
{
	return 0;

}


//--------------------------------------------------------------
// 설명 : 터치 스크린 칼리브 레이션 정보를 로딩한다. 
//--------------------------------------------------------------
int ix_loading_ts_cailbration_info( void )
{
	FILE 	*fd;
	char 	linebuf[128]={0,};
	char	cmd[128]	={0,};
	int 	lp			=0;
	int		tag_index	=0;

	char	*tag[] ={	"[screen-x]",	"[screen-y]",				
						"[touch-x]",	"[touch-y]"
					};

	if(access(FILE_TOUCH_CAL_INFO ,F_OK))
	{
		printf("%s not found!!\n",FILE_TOUCH_CAL_INFO);
		return -1;
	}
	
	fd = fopen(FILE_TOUCH_CAL_INFO ,"r");	
	if(fd <0)
	{
		printf("file open error!!\n");
	}

	while(!feof(fd))
	{
		fgets(linebuf, sizeof(linebuf)-1 , fd);

		if(linebuf[0] == '#')  continue;			// 주석은 무시한다. 

		sscanf(linebuf ,"%s", cmd);

		for( tag_index=0 ; tag_index < 4 ; tag_index++ )
		{
			if(!strcmp(cmd , tag[tag_index]))
			{
				for(lp=0 ; lp <4 ; lp++)
				{
					memset(linebuf, 0, sizeof(linebuf));	
					memset(cmd    , 0, sizeof(cmd));
					
					fgets(linebuf, sizeof(linebuf)-1 , fd);		// 한줄씩 읽어 온다.
					if(linebuf[0] == '#')  continue;			// 주석은 무시한다. 
					
					if(atoi(&linebuf[0]) == (lp+1))
					{			
						sprintf(cmd,"%d=",lp+1);									
					
						if(!memcmp(linebuf , cmd, 2 ))				
						{
							switch(tag_index)
							{
							case 0	: ix_cal_touch_info[lp].sx = atoi(&linebuf[2]); break;
							case 1	: ix_cal_touch_info[lp].sy = atoi(&linebuf[2]); break;
							case 2	: ix_cal_touch_info[lp].tx = atoi(&linebuf[2]); break;
							case 3	: ix_cal_touch_info[lp].ty = atoi(&linebuf[2]); break;
							default : break;
							}	
						}
					}
				}
			}
			else
			{
				continue;
			}
		}
	}

//#if 1
	for(lp=0 ; lp <4 ; lp++)
	{
		printf("======================================\n");
		printf("\r screen -x[%d]:%d , y[%d]:%d ",lp,ix_cal_touch_info[lp].sx ,lp ,ix_cal_touch_info[lp].sy);TRACE;
		printf("\r touch  -x[%d]:%d , y[%d]:%d ",lp ,ix_cal_touch_info[lp].tx ,lp ,ix_cal_touch_info[lp].ty);TRACE;
		printf("\n");
	}
//#endif
	
	fclose(fd);
	return 0;
}

//--------------------------------------------------------------
// 설명 : 터치 스크린 칼리브 레이션 정보를 설정한다.
// 1) section을 찾는다. 
// keyname  는 '1='
//--------------------------------------------------------------
int ix_write_ts_cailbration_info( char *section, char *keyname ,int value )
{

	FILE 	*fd;
	char 	linebuf[128]={0,};
	char	cmd[128]	={0,};
	int 	lp			=0;
	int size=0;

	if(access(FILE_TOUCH_CAL_INFO ,F_OK))
	{
		printf("%s not found!!\n",FILE_TOUCH_CAL_INFO);
		return -1;
	}
	
	fd = fopen(FILE_TOUCH_CAL_INFO ,"r+");	
	if(fd <0)
	{
		printf("file open error!!\n");
	}
	
	while(!feof(fd))
	{
		fgets(linebuf, sizeof(linebuf)-1 , fd);

		if(linebuf[0] == '#')  continue;			// 주석은 무시한다. 

		sscanf(linebuf ,"%s", cmd);
		
		if(!strcmp(cmd , section))					// 색션을 비교한다. 
		{	
			printf("[cmd]:%s [section]:%s \n",cmd ,section);
		
			for(lp=0 ; lp <4 ; lp++)
			{
				memset(linebuf, 0, sizeof(linebuf));
				memset(cmd    , 0, sizeof(cmd));
				
				fgets(linebuf, sizeof(linebuf)-1 , fd);		// 한줄씩 읽어 온다. 마지막에 \n 이 삽입됨 
				size = sizeof(linebuf);
				printf("[size]:%d \n",size);
				fseek(fd, -size, SEEK_CUR	);
				
				if(linebuf[0] == '#')  continue;			// 주석은 무시한다. 
				
				if(atoi(&linebuf[0]) == (lp+1))				// key를 찾는다. 
				{
					// keyname 을 확인한다. 
					if(!strncmp(linebuf , keyname, 2 ))				
					{		
						
						printf("linebuf:%d , keyname:%s   \n",atoi(&linebuf[0]) ,keyname );
						printf("lp의 key %s 에 값을 쓴다. \n", keyname);
						fprintf(fd, "\r%s%d",keyname,value );	// 쓰기 , ?? 왜 한칸 아래에 써지지?																// fgets 때문에 이동한다. 
					}
				}
			}
		}
		else
		{
			continue;
		}

	}

#if 0
	for(lp=0 ; lp <4 ; lp++)
	{
		printf("======================================\n");
		printf("\r screen -x[%d]:%d , y[%d]:%d ",lp,ix_cal_touch_info[lp].sx ,lp ,ix_cal_touch_info[lp].sy);TRACE;
		printf("\r touch  -x[%d]:%d , y[%d]:%d ",lp ,ix_cal_touch_info[lp].tx ,lp ,ix_cal_touch_info[lp].ty);TRACE;
		printf("\n");
	}
#endif

	fclose(fd);
	return 0;

}


//--------------------------------------------------------------
// 설명 : touch screen 값을 보정하기 위한 값을 넣는다.
// 반환 : -1 이면 에러 발생 , 0이면 정상
//--------------------------------------------------------------
int ix_set_cal_touch_info(int index ,ix_cal_touch_info_t one_point)
{

	if(index < 0)						return -1;
	//if(index >=IX_CAL_TOUCH_INFO_MAX) 	return -1;

	ix_cal_touch_info[index] = one_point;
	
	//printf("\r x[%d]:%d , y[%d]:%d ",index ,ix_cal_touch_info[index].sx ,index ,ix_cal_touch_info[index].sy); TRACE;
	//printf("\r x[%d]:%d , y[%d]:%d ",index ,ix_cal_touch_info[index].tx ,index ,ix_cal_touch_info[index].ty);TRACE;
	//printf("\n");

	ix_adjust_flag =0;
	return 0;
}

//--------------------------------------------------------------
// 설명 : touch screen 값을 보정을 위한 기준 값을 설정한다. 
//--------------------------------------------------------------
int ix_adjust_touch( void )
{	
	int tx1 =0; //touch min
	int tx2 =0;	//touch max
	
	int sx1 =0; //screen min
	int sx2 =0;	//screen max
	
	int ty1 =0; //touch min
	int ty2 =0; //touch max
	
	int sy1 =0; //screen min
	int sy2 =0;	//screen max
	
	tx1 =(ix_cal_touch_info[0].tx + ix_cal_touch_info[2].tx )/2 ;
	tx2 =(ix_cal_touch_info[1].tx + ix_cal_touch_info[3].tx )/2 ;	
	sx1 =(ix_cal_touch_info[0].sx + ix_cal_touch_info[2].sx )/2 ;
	sx2 =(ix_cal_touch_info[1].sx + ix_cal_touch_info[3].sx )/2 ;
	
	ty1 =(ix_cal_touch_info[0].ty + ix_cal_touch_info[1].ty )/2 ;
	ty2 =(ix_cal_touch_info[2].ty + ix_cal_touch_info[3].ty )/2 ;	
	sy1 =(ix_cal_touch_info[0].sy + ix_cal_touch_info[1].sy )/2 ;
	sy2 =(ix_cal_touch_info[2].sy + ix_cal_touch_info[3].sy )/2 ;
		
				
#if 0//wj@@test-touch
	ix_adjust_touch_info.xa = ((tx2 -tx1)*IX_CAL_TOUCH_INFO_OFSSET) /(sx2 - sx1);
	ix_adjust_touch_info.xb = tx1 -(( ix_adjust_touch_info.xa * sx1)/IX_CAL_TOUCH_INFO_OFSSET) ;
	
	ix_adjust_touch_info.ya = ((ty2 -ty1)*IX_CAL_TOUCH_INFO_OFSSET )/(sy2 - sy1);
	ix_adjust_touch_info.yb = ty1 -(( ix_adjust_touch_info.ya * sy1)/IX_CAL_TOUCH_INFO_OFSSET) ;
#else
    ix_adjust_touch_info.xa = ((sx1 - sx2) * IX_CAL_TOUCH_INFO_OFSSET) /(tx1 - tx2);
    ix_adjust_touch_info.xb = (sx1 * IX_CAL_TOUCH_INFO_OFSSET) - (ix_adjust_touch_info.xa * tx1) ;

    ix_adjust_touch_info.ya = ((sy1 - sy2) * IX_CAL_TOUCH_INFO_OFSSET )/(ty1 - ty2);
    ix_adjust_touch_info.yb = (sy1 * IX_CAL_TOUCH_INFO_OFSSET) - (ix_adjust_touch_info.ya * ty1) ;


    ix_adjust_touch_flag = FLAG_SET;

#endif	

	return 0;	
}


//--------------------------------------------------------------
// 설명 : 
//--------------------------------------------------------------
int ix_get_touch(int x_tch, int y_tch, int *x_scr_ptr, int *y_scr_ptr)
{
    int   x, y;

#if 1//reserved
    if ( !ix_adjust_touch_flag )
    {
        *x_scr_ptr = x_tch;
        *y_scr_ptr = y_tch;

        return 1;
    }
#endif

    x = (ix_adjust_touch_info.xa * x_tch + ix_adjust_touch_info.xb) / IX_CAL_TOUCH_INFO_OFSSET;
    y = (ix_adjust_touch_info.ya * y_tch + ix_adjust_touch_info.yb) / IX_CAL_TOUCH_INFO_OFSSET;

    if( x < 0 )                 x = 0;
    if( y < 0 )                 y = 0;

    if(x >= ix_sreen_x_max)     x = ix_sreen_x_max; 
    if(y >= ix_sreen_y_max)     y = ix_sreen_y_max; 

    *x_scr_ptr = (int)x;
    *y_scr_ptr = (int)y;
    return 1;
}

//--------------------------------------------------------------
// 설명 : ix_adjust_touch_info 값을 설정 한다
//--------------------------------------------------------------
int ix_set_touch( ix_adjust_touch_info_t *info )
{	

    ix_adjust_touch_info.xa = info->xa;
    ix_adjust_touch_info.xb = info->xb;
    ix_adjust_touch_info.ya = info->ya;
    ix_adjust_touch_info.yb = info->yb;

    ix_adjust_touch_flag = FLAG_SET;

	return 1;	
}

//--------------------------------------------------------------
// 설명 : ix_adjust_touch_info 값을 가져온다
//--------------------------------------------------------------
int ix_get_touch_info( ix_adjust_touch_info_t *info )
{	
    *info = ix_adjust_touch_info;
	return 1;	
}


//--------------------------------------------------------------
// 설명 : ix_adjust_touch_info 값을 프린트 한다
//--------------------------------------------------------------
int ix_print_touch( void )
{
    printf ("ix_adjust_touch_info.xa = %d \n", ix_adjust_touch_info.xa);
    printf ("ix_adjust_touch_info.xb = %d \n", ix_adjust_touch_info.xb);
    printf ("ix_adjust_touch_info.ya = %d \n", ix_adjust_touch_info.ya);
    printf ("ix_adjust_touch_info.yb = %d \n", ix_adjust_touch_info.yb);
	printf ("ix_adjust_touch_flag = %d \n\n", ix_adjust_touch_flag);

	return 1;	
}

//--------------------------------------------------------------
// 설명 : ix_adjust_touch_info 값을 설정하기 위한 초기화 
//--------------------------------------------------------------
int ix_adjust_touch_init( void )
{
	//터치스크린 보정정보 읽기
	ix_loading_ts_cailbration_info ();

	//보정값 설정 (ix_adjust_touch_flag=1)
	ix_adjust_touch ();

	//보정정보 출력
	ix_print_touch ();

	return 0;
}

