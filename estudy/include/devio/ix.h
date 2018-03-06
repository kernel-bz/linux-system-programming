//------------------------------------------------------------------------------
// 파 일 명 : ix.h
// 프로젝트 : fwp
// 설    명 :
// 작 성 자 : 유영창, 이영민, 문경원 , 이원재,
// 작 성 일 : 2009년 1월 17일
// 수 정 일 :
// 수 정 일 :
// 주    의 :
// 라이센스 : GPL V2
//------------------------------------------------------------------------------

#ifndef _IX_HEADER_
#define _IX_HEADER_


#define	FILE_NAME_INPUT_INFO_BASE		"/proc/bus/input/devices"					// 디바이스 정보위치
#define	IX_TMP_PATH_DEFAULT				"/tmp"										// mknod를 하기위한 임시 폴더

#define FLAG_SET						1
#define FLAG_RELEASE					0

//#define FILE_TOUCH_CAL_INFO				"/etc/ixlib/ts_calibration_info.db"			// 터치 스크린 칼리브 레이션 데이터 
#define FILE_TOUCH_CAL_INFO				"./ts_calibration_info.db"			// 터치 스크린 칼리브 레이션 데이터 

#define IX_KEY_PRESS_MAX		54			// KEY PRESS 로 처리여 전달한 값의 최대값 
// 아스키 테이블 표 
#define	 ASKII_RESERVED 		0x0			// 
#define	 ASKII_TAP				0x09                
#define	 ASKII_RETURN			0x0d                
#define  ASKII_DELETE			0x10                
#define  ASKII_ESCAPE			0x1b                
#define  ASKII_SPACE			0x20                
#define  ASKII_EXCLAM			0x21 			// !
#define  ASKII_QUOTES			0x22			// "
#define  ASKII_HASH				0x23			// #
#define  ASKII_DOLOR			0x24			// $
#define  ASKII_PERCENT			0x25			// %
#define  ASKII_AMPERSAND		0x26			// &
#define  ASKII_SINGLEQUOTE 		0x27			// '
#define  ASKII_OPENPARAN		0x28			// (
#define  ASKII_CLOSEPARAN		0x29			// )
#define  ASKII_STAR				0x2A			// *
#define  ASKII_PLUS				0x2B			// + 
#define  ASKII_COMMA			0x2C			// ,
#define  ASKII_MINUS			0x2D			// - 
#define  ASKII_DOT				0x2E			// . 
#define  ASKII_FORDWARDSLASH	0x2F			// / 
#define  ASKII_ZERO				0x30                
#define  ASKII_1				0x31                
#define  ASKII_2				0x32                
#define  ASKII_3				0x33                
#define  ASKII_4				0x34                
#define  ASKII_5				0x35                
#define  ASKII_6				0x36                
#define  ASKII_7				0x37                
#define  ASKII_8				0x38                
#define  ASKII_9				0x39                
#define  ASKII_COLON			0x3a			// :
#define  ASKII_SEMICOLON		0x3b			// ; 
#define  ASKII_OPENANGLE		0x3c			// <
#define  ASKII_EQUAL			0x3d			// =
#define  ASKII_CLOSEANGLE		0x3e			// >
#define  ASKII_QUESTION			0x3f			// ?
#define  ASKII_AT				0x40  			// @              
#define  ASKII_A				0x41                
#define  ASKII_B				0x42                
#define  ASKII_C				0x43                
#define  ASKII_D				0x44                
#define  ASKII_E				0x45                
#define  ASKII_F				0x46                
#define  ASKII_G				0x47                
#define  ASKII_H				0x48                
#define  ASKII_I				0x49                
#define  ASKII_J				0x4a                
#define  ASKII_K				0x4b                
#define  ASKII_L				0x4c                
#define  ASKII_M				0x4d                
#define  ASKII_N				0x4e                
#define  ASKII_O				0x4f                
#define  ASKII_P				0x50                
#define  ASKII_Q				0x51                
#define  ASKII_R				0x52                
#define  ASKII_S				0x53                
#define  ASKII_T				0x54                
#define  ASKII_U				0x55                
#define  ASKII_V				0x56                
#define  ASKII_W				0x57                
#define  ASKII_X				0x58                
#define  ASKII_Y				0x59                
#define  ASKII_Z				0x5a                
#define  ASKII_OPENSQURE		0x5b			// [
#define  ASKII_BACKSLASH		0x5c			// "\"  
#define  ASKII_CLOSESQUARE		0x5d			// ]
#define  ASKII_CARAT			0x5e			// ^ 
#define  ASKII_UNDERBAR			0x5f			// _ 
#define  ASKII_GRAVE			0x60			// `
#define  ASKII_a				0x61                
#define  ASKII_b				0x62                
#define  ASKII_c				0x63                
#define  ASKII_d				0x64                
#define  ASKII_e				0x65                
#define  ASKII_f				0x66                
#define  ASKII_g				0x67                
#define  ASKII_h				0x68                
#define  ASKII_i				0x69                
#define  ASKII_j				0x6a                
#define  ASKII_k				0x6b                
#define  ASKII_l				0x6c                
#define  ASKII_m				0x6d                
#define  ASKII_n				0x6e                
#define  ASKII_o				0x6f                
#define  ASKII_p				0x70                
#define  ASKII_q				0x71                
#define  ASKII_r				0x72                
#define  ASKII_s				0x73                
#define  ASKII_t				0x74                
#define  ASKII_u				0x75                
#define  ASKII_v				0x76                
#define  ASKII_w				0x77                
#define  ASKII_x				0x78                
#define  ASKII_y				0x79                
#define  ASKII_z				0x7a                
#define  ASKII_CURLYOPEN		0x7b			// {
#define  ASKII_PIPE				0x7c			// |
#define  ASKII_CURLYCLOSE		0x7d			// }
#define  ASKII_TILDE			0x7e			// ~  




#define IX_ALPABAT_MAX					26					// 알파벳 최대 개수 A~Z
#define IX_NUMBER_MAX					10					// 숫자  최대 개수  0~9 

#define IX_INPUT_DEVICE_MAX				16

#define IXERR_NONE						0                   //IXERR_NONE
#define IXERR_READ						1                   //IX READ ERROR
#define IXERR_FD						2                   //IX FD ERROR
#define IXERR_DEV_INFO_OPEN				3  	                //IXERR_DEV_INFO_OPEN
#define IXERR_AUTOSCAN_SCANDEVICE   	4					//ix_auto_scan_open 함수 에서 ix_scan_device 에러
#define IXERR_AUTOSCAN_MKNOD			5					//ix_auto_scan_open 함수 에서 mknod 에러",
#define IXERR_AUTOSCAN_DEVICE_OPEN  	6                   //ix_auto_scan_open 함수 에서 디바이스 파일 오픈"
#define IXERR_AUTOSCAN_EX_ADD        	7                   //ix_auto_scan_open 함수 에서 ex 함수 등록 에러",
#define IXERR_WRITE_EVENT 		       	8                   //ix_write_event_low 함수 에서 잘못된 인자 에러",
#define IXERR_WRITE_MOUSE_BUTTON_EVENT 	9                   //ix_write_mouse_button_action 함수 에서 잘못된 인자 에러",
#define IXERR_WRITE_KEYBOARD_EVENT		10					//ix_write_keyboard_key_action 함수 에서 잘못된 인자 에러",


#define	IX_KEY_VAULE_UP_LOW				0					// 키버튼 누름 해제  LOW 값 
#define	IX_KEY_VAULE_DOWN_LOW			1					// 키버튼 누름
#define	IX_KEY_VAULE_PRESS_LOW			2					// 키버트 DOWN->UP 된 후, 아키코 코드값으로 CODE를 변환하다. 

#define	IX_MOUSE_VAULE_UP_LOW			0					//  마우스 오른쪽 버튼이 눌림
#define	IX_MOUSE_VAULE_DOWN_LOW			1					//  마우스 오른쪽 버튼이 눌림 해제

//---------------------------------------------------------------------------------
// 비트 마스크 
//---------------------------------------------------------------------------------
#define IX_MOUSE_BTN_LEFT				0x00000001			// 마우스 왼쪽 버튼
#define IX_MOUSE_BTN_RIGHT				0x00000002			// 마우스 오른쪽 버튼
#define IX_MOUSE_BTN_MIDDLE				0x00000004			// 마우스 스크롤 버튼

#define IX_MOUSE_FLAG_BTN_DOWN			0x00000001			// 마우스 버튼 눌림
#define IX_MOUSE_FLAG_BTN_UP			0x00000002			// 마우스 버튼 떨어짐
#define IX_MOUSE_FLAG_MOVE				0x00000004			// 마우스 움직임
#define IX_MOUSE_FLAG_DOUBLE_CLICK		0x00000008			// 마우스 움직임


#define	IX_KEY_VAULE_UP					0x00000001			// 키버튼 누름 해제   , state에 make에 사용된다. 
#define	IX_KEY_VAULE_DOWN				0x00000002			// 키버튼 누름
#define	IX_KEY_VAULE_PRESS				0x00000004			// 키버트 DOWN->UP 된 후, 아키코 코드값으로 CODE를 변환하다. 
//#define	IX_KEY_VAULE_REPEAT			0x00000008					// 키버튼 누름 유지  (제거 대상)

#define IX_KEYBOARD_BTN_LEFT_SHIFT		0x80000000			// 키보드 왼쪽 SHIFT
#define IX_KEYBOARD_BTN_RIGHT_SHIFT		0x40000000			// 키보드 오른쪽 SHIFT

#define IX_KEYBOARD_BTN_LEFT_ALT		0x20000000			// 키보드 왼쪽 ALT
#define IX_KEYBOARD_BTN_RIGHT_ALT		0x10000000			// 키보드 오른쪽 ALT

#define IX_KEYBOARD_BTN_LEFT_CTRL		0x08000000			// 키보드 왼쪽 CTRL
#define IX_KEYBOARD_BTN_RIGHT_CTRL		0x04000000			// 키보드 오른쪽 CTRL

#define IX_KEYBOARD_CAPSLOCK			0x02000000			// 키보드 Caps Lock 버튼이 눌림     // imesu 090215
#define IX_KEYBOARD_NUMLOCK				0x01000000			// 키보드 Num Lock 버튼이 눌림     // imesu 090215

#define IX_KEYBOARD_UP					0x00800000			// 키보드 윗쪽  	방향키
#define IX_KEYBOARD_DOWN				0x00400000			// 키보드 아래쪽 	방향키
#define IX_KEYBOARD_LEFT				0x00200000			// 키보드 왼쪽 		방향키 버튼이 눌림     // imesu 090215
#define IX_KEYBOARD_RIGHT				0x00100000			// 키보드 오른쪽 	방향키 버튼이 눌림     // imesu 090215



#define IX_SCREEN_X_DEFAULT				640					// 스크린 X 사이즈
#define IX_SCREEN_Y_DEFAULT				480					// 스크린 Y 사이즈

#define IX_CAL_TOUCH_INFO_MAX			4					// 좌표 입력 최대 버퍼 갯수

#if 0//wj@@test-touch
#define IX_CAL_TOUCH_INFO_OFSSET		1				// TOUCH SCREEN 보정값 
#else
#define IX_CAL_TOUCH_INFO_OFSSET		0x00010000          //(1<< 16)// TOUCH SCREEN 보정값 
#endif



#define IX_MOUSE_DOUBLE_CLICK_TIME_INTAVAL	   500			//500msec = 0.5 초 	
//---------------------------------------------------------------------------------
// 사용자 전송 이벤트 메시지
//---------------------------------------------------------------------------------
#define IE_MOUSE_DOWN					1
#define IE_MOUSE_UP 					2
#define IE_MOUSE_MOVE   				4
#define IE_KEY_PRESS 					5
#define IE_KEY_DOWN 					6
#define IE_KEY_UP 						7

typedef struct input_event ix_event_t;					// kernel 에 있는 이벤트 구조체 /include/linux/input.h

typedef struct
{
	int     fd;					// 디스크립션
	char 	name[128];			// 장치 이름
	int 	input_number;		// 입력장치 번호
	int 	event_number;		// event 장치 번호

	int 	dev_major;			// 장치 주번호
	int 	dev_minor;			// 장치 부번호

	int		is_touch;			// 터치 디바이스 이면 1 0 이면 아님 

} ix_mng_t;

typedef  unsigned long ix_btn_state_t;	// 버튼의 상태 자료형

typedef struct {
	int  tx;					// touch screen x좌표값
	int  ty;					// touch screen y좌표값

	int  sx;					// lcd screen x좌표값
	int  sy;					// lcd screen y좌표값
}ix_cal_touch_info_t;

typedef struct {
	int  xa;					// 기울기
	int  xb;					//

	int  ya;					// 기울기
	int  yb;					//
}ix_adjust_touch_info_t;


typedef struct {
	int				x;			//x 좌표
	int				y;			//y 좌표
	ix_btn_state_t  state;		//상태

}ie_mouse_t;

typedef struct {
	int				key;		//입력 표시
	ix_btn_state_t  state;		//상태
}ie_key_t;

typedef struct {
	int		type;				// 입력값 종류
	union{
		ie_mouse_t		mouse;	// 마우스 입력
		ie_key_t		key;	// keyboard 입력
	}data;
}ie_event_t;


typedef      int (*ix_event_func_t) (ie_event_t *event_data);

#ifdef __cplusplus
extern "C"
{
#endif


	void 		ix_print_error				( char *msg );								// 에러 메시지 출력
	void		ix_debug_disp_event_data	( ix_event_t *event_data );					// 디버깅용 이벤트 데이터 값을 표시한다.
	void		ix_debug_disp_ix_device_info( void );									// ix_scan_device 으로 얻어온 입력 장치에 대한 정보 화면에 표시한다. 


	int 		ix_init						( int mode );
	int			ix_auto_scan_open			( char *path );								// 1)입력 장치를 자동 검색
																						// 2) mknod
																						// 3) 디바이스 파일 오픈..
																						// 4) ex 함수 등록
																						
	int 		ix_get_touch_fd				( char *path );								// 터치 스크린 파일 디스크립터를 구한다. 

	int 		ix_add_event				( ix_event_func_t func );					//이벤트 데이터를 처리할 함수를 등록한다.
	int 		ix_scan_device				( void );									// 입력 장치 정보를 검색한다.

	int 		ix_read_event_data_low		( int fd,  ix_event_t *event_data );		//이벤트 구조로 데이터를 읽어 오는것
	int 		ix_read_keyboard_low		( int fd,  int *key, int *key_state );		//키보드 값을 읽어 온다.
	int 		ix_read_key_press_low		( int fd,  int *key );						//키보드가 눌렸을때 값
	int 		ix_read_touch_low			( int fd,  int *x, int *y, int *press );	// touch 값을 읽어 온다.
	int 		ix_read_mouse_low			( int fd,  int *x, int *y, int *btn);		//마우스 입력값을 얻어 온다.
	int 		ix_set_screen_size			( int x ,  int y);

	int			ix_write_event_low			( int fd, ix_event_t *event_data );			//이벤트 장치에 사용자가 직접 이벤트 정보를 넣어서 이벤트를 발생시킨다.
	int			ix_write_mouse_button_action( int mouse_button, int action );			//마우스 특정버튼의 Up down 엑션 이벤트를 발생시킨다.
	int			ix_write_mouse_rel_move		( int rel_x, int rel_y );					//마우스 변위 위치 변경 이벤트를 발생시킨다.
	int			ix_write_mouse_abs_move		( int abs_x, int abs_y );					//마우스 절대 위치 변경 이벤트를 발생시킨다.
	int			ix_write_keyboard_key_action( int keyboard_key, int action);			//키보드에 키 이벤트를 발생시킨다.
	int			ix_write_key_press		( int askii );								//키보드에 key press 값을 넣는다. 

	int			ix_get_mouse_position		( int *x,  int *y );						// 마우스 현재 위치를 얻는다.
	int			ix_set_mouse_double_click_intaval( int msec );							// 마우스 더블 클릭 간격 설정 
	int			ix_get_key_value			( int *key );								// keyboard 입력값을 얻는다.


	int			ix_adjust_touch				( void );									// touch screen을 보정한다.
	//int 		ix_get_touch_position		( int *x, int *y );							// 터치 스크린 보정된 위치값을 얻어 온다. 
    int         ix_get_touch                ( int x_tch, int y_tch, int *x_scr_ptr, int *y_scr_ptr );
    int         ix_get_touch_info           ( ix_adjust_touch_info_t *info );
    int         ix_set_touch                ( ix_adjust_touch_info_t *info );
    int         ix_print_touch              ( void );
    int         ix_adjust_touch_init        ( void );

	int			ix_loading_ts_cailbration_info( void );									//터치 스크린 칼리브 레이션 정보를 로딩한다. 

	int			ix_write_ts_cailbration_info( char *section, char *keyname ,int value );	// 터치스크린 입력 값을 쓴다. 
#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------
// 전역 변수 정의
//------------------------------------------------------------------------------






#ifndef _IX_VAR_

#endif

#endif  // _IX_HEADER_
