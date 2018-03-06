//------------------------------------------------------------------------------
// �� �� �� : ix.c
// ������Ʈ : fwp
// ��    �� :
// �� �� �� : ����â, �̿���, ����� , �̿���
// �� �� �� : 2009�� 1�� 17��
// �� �� �� :
// �� �� �� :
// ��    �� :
// ���̼��� : GPL V2
// to do list :
//  1. ix_scan_device()  : sprintf(filename_buf,"/sys/class/input/input%d/name",ix_mng[lp].input_number );
//     ���� �������� �ʿ��Ѱ� ? ���� �� ������ ���ٸ� error�� retrun �ؾ� �ϴ°�?
//	   ���� ������ �ִ� ���忡�� /sys/class/input/input0 ������ ����. �׷��� event0��
//	   �ֱ� ������ �Է� ���� ������ ���� ���� ����.

//  3. press��ư�� ��ư�� down�Ǵ� ���� ���� ������ , ascii-2�� ��ȯ�Ѵ�. 
//  4. �ΰ� ��� Ű shitf, alt,...���� Űī ���� �� ���� code���� �����ϸ� �ȵȴ�. press���� ���� ��� ������ shtfŰ�� Ű�� 6�� ������ �ڵ带 ������. 
//     ���� �ΰ� ��� Ű�� ��������, key ����0���� �ؼ� ��������. 
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
#include <fcntl.h>                      // O_RDWR , O_NOCTTY ���� ��� ����
#include <time.h>

#include "ex.h"
#include "ix.h"
#include "ux_lib.h"

#define _IX_VAR_ 1

//--------------------------------------------------------------
// ���� ����
//--------------------------------------------------------------

#define ABS( x )   (((x) > 0 )? (x):-(x))		// ���밪 


static 	int				ix_error_code			= IXERR_NONE;				// �����ڵ�
static	ix_mng_t		ix_mng[IX_INPUT_DEVICE_MAX];
static	int				ix_mng_count			= 0;
static	ex_mng_t 		*input_obj[IX_INPUT_DEVICE_MAX];					// �Է� ����̽��� ���� ������Ʈ


static int				ix_sreen_x_min = 0;
static int				ix_sreen_y_min = 0;

static int				ix_sreen_x_max = IX_SCREEN_X_DEFAULT;
static int				ix_sreen_y_max = IX_SCREEN_Y_DEFAULT;

static int				ix_mouse_x = IX_SCREEN_X_DEFAULT/2;
static int				ix_mouse_y = IX_SCREEN_Y_DEFAULT/2;
static int				ix_key	    =0;											// KEY  �Է°�

static ix_btn_state_t	ix_button_state =0;											// ��ư ���°�

static unsigned long	ix_mouse_event_flag	 =0;									// ���콺 �̺�Ʈ �߻� ( up ,down, move ..etc)
static unsigned long	ix_key_event_flag	 =0;									// key    �̺�Ʈ �߻� ( up ,down ,press )
//static unsigned long	ix_mouse_double_click_flag=0;

static ix_event_func_t  ix_event_func = NULL;			// �Լ� ������ 										//

static ix_cal_touch_info_t  	ix_cal_touch_info[IX_CAL_TOUCH_INFO_MAX];
static ix_adjust_touch_info_t  	ix_adjust_touch_info;

static char 			ix_capslock_flag 	=0;										// Ű���� caps lock flag
static char 			ix_numlock_flag 	=0;										// Ű���� caps lock flag

	   int  			ix_mouse_click_interval = IX_MOUSE_DOUBLE_CLICK_TIME_INTAVAL;		// ���콺 ���� Ŭ�� Ÿ�� ���͹� 

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
// �Լ� 
//--------------------------------------------------------------
static int ix_get_key_press_value(int *m_key, ix_btn_state_t button_state);


//--------------------------------------------------------------
// ���� :
// �Ű� :
// ��ȯ : ����0, ����-1
//--------------------------------------------------------------
int ix_init (int mode)
{
	// ���� ���� �ʱ�ȭ ... ���߿� ����.
	ix_event_func = NULL;

	memset(ix_cal_touch_info, 0, sizeof(ix_cal_touch_info_t)*IX_CAL_TOUCH_INFO_MAX);
	memset(&ix_adjust_touch_info, 0, sizeof(ix_adjust_touch_info_t));
	
	gettimeofday( &ix_pre_time, NULL );

	//��ġ��ũ�� �������� ����
	ix_adjust_touch_flag = 0;
	if (mode > 0) ix_adjust_touch_init ();
	
	return 0;
}

//--------------------------------------------------------------
// ���� :
//--------------------------------------------------------------
static char  *ix_error_string( void )
{

	char *error_string[] ={ "���� ����",                                    			//IXERR_NONE
							"ix_read_event_data_low �Լ� read ���� ",               	//IXERR_READ
							"ix_read_event_data_low �Լ� fd ���� ",						//IXERR_FD
							"ix_scan_device �Լ� ��ġ �������� ���� ����",				//IXERR_DEV_INFO_OPEN
							"ix_auto_scan_open �Լ� ���� ix_scan_device ����",			//IXERR_AUTOSCAN_SCANDEVICE
							"ix_auto_scan_open �Լ� ���� mknod ����",					//IXERR_AUTOSCAN_MKNOD
							"ix_auto_scan_open �Լ� ���� ����̽� ���� ����",			//IXERR_AUTOSCAN_DEVICE_OPEN
							"ix_auto_scan_open �Լ� ���� ex �Լ� ��� ����",			//IXERR_AUTOSCAN_EX_ADD
							"ix_write_event_low �Լ� ���� �߸��� ���� ����",			//IXERR_WRITE_EVENT
							"ix_write_mouse_button_action �Լ� ���� �߸��� ���� ����",	//IXERR_WRITE_MOUSE_BUTTON_EVENT
							"ix_write_keyboard_key_action �Լ� ���� �߸��� ���� ����",	//IXERR_WRITE_KEYBOARD_EVENT

                        };
	return( error_string[ix_error_code]);

	//printf( "\r[gx error:%d]%s %s\n", ex_error_code, ex_error_string(), msg);
}

//--------------------------------------------------------------
// ���� : ixlib�� �����ڵ�, �������ڿ� �� ����� �޼��� ���
// �Ű� : ����� �޼���
//--------------------------------------------------------------
void ix_print_error( char *msg )
{
	printf( "\r[ix error:%d]%s %s\n", ix_error_code, ix_error_string(), msg);
	//return gx_error_code;
}


//--------------------------------------------------------------
// ���� : �̺�Ʈ ������ �����͸� �о� ���°�
// ���� : fd: ���� ��ũ���� , event_data: �̺�Ʈ ������
// ��ȯ : ���� :�о�� ������ ������  , ���� : -1
//--------------------------------------------------------------
int ix_read_event_data_low(int fd, ix_event_t *event_data )
{
	int	read_size = 0;

	if(fd <0)
	{
		ix_error_code = IXERR_FD; 		// ����
		return -1;
	}

	read_size = read( fd, event_data, sizeof(ix_event_t));

	if(read_size != sizeof(ix_event_t))
	{
		ix_error_code = IXERR_READ; 	// �о�� �����Ͱ� �ùٸ��� ������ ������.
		return -1;
	}

	return read_size;

}

//--------------------------------------------------------------
// ���� : �̺�Ʈ ������ �����͸� �о� ���°�
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
	case    EV_ABS :    printf("EV_ABS  ");		break;      // ��ġ ��ũ�� �Է�
	case    EV_REL :    printf("EV_ABS  ");		break;

    default : 	printf("[type]:%02x! " ,event_data->type); 			break;
	}

	switch(event_data->type)
	{
 	case    EV_ABS :    // ���밪
 		printf("[code:%d]%s , [value]:%d  \n",
 					event_data->code ,
 					ix_code_abs_str[event_data->code],
 					event_data->value
 			   );
 		break;

	case    EV_REL :   	// ��밪
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
// ���� : ix_scan_device ���� ���� �Է� ��ġ�� ���� ���� ȭ�鿡 ǥ���Ѵ�. 
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
// ���� : Ű���� ���� �о� �´�.
// down : l
//--------------------------------------------------------------
int ix_read_keyboard_low(int fd , int *key, int *key_state )
{
	ix_event_t	 event_data;

	*key 		= KEY_RESERVED ;				// �ʱ�ȭ
	*key_state 	= 0;							// �ʱ�ȭ

	while(1)
	{
		if(ix_read_event_data_low(fd , &event_data ))	return -1;

		//ix_debug_disp_event_data(&event_data );

		if(event_data.type == EV_KEY)
		{
			*key 		= event_data.code;
			*key_state	= event_data.value;
		}
		if(event_data.type == EV_SYN)			break;			// syn�� ���� ������.
	}

	return 0;
}


//--------------------------------------------------------------
// ���� : Ű���� ���� �о� �´�.
//        Ű���带 ������
//        Ű�� ������ �����Ͱ� �ι� �´�.
//[Sec]665, [uSec]902491 EV_KEY  [code]:2 ,[value]:1       //value�� 1�̸� down
//[Sec]665, [uSec]902604 EV_SYN  [code]:0 ,[value]:0
// ����  key �� 2�� syn �ɶ� ���ױ� ����
//--------------------------------------------------------------
int ix_read_key_press_low(int fd , int *key )
{
	int 	key_state =0;					// syn�ϱ� ������ ����
	ix_event_t	 event_data;

	*key 	= KEY_RESERVED ;				// �ʱ�ȭ


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
			break;			// Ű�� ���� �� syn�� ���� ������.
		}
	}	
	return 0;
}


//--------------------------------------------------------------
// ���� : mouse ���� �о� �´�.
// ���� :
//		*x	: REL_X ��
//		*y	: REL_Y ��
//		*btn: code ���� �̿��Ͽ� ���콺 ��ư ��ġ�� �Ѱ��ش�.
//			  ��ư�� ���ÿ� ������ ���� �� �������� ��Ʈ ����ũ �Ͽ� ���ÿ� ��ư�� ������ ���¸� �����Ѵ�.
//
//  [����1. ���콺 �����ʹ�ư�� ������, ���콺�� ������ ���]
//	type = 	EV_KEY, code = BTN_TOUCH,	value = 1    <- ��ġ ��ũ�� ���콺 ���� ��ư�� ����
//	type = 	EV_ABS, code = ABS_X,		value = -14  <- X ��ǥ���� -14�̴�.
//	type = 	EV_ABS, code = ABS_Y,		value = 99   <- Y ��ǥ���� +99�̴�.
//	type = 	EV_SYN, code = 0,			value = 1    <- ���ݱ��� �����Ͱ� �ϳ��� �������̴�.
//
//	��ư�� ���� ���¿��� , ���콺�� �����̴� ���� Ȯ�� �Ϸ���,
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

		if(event_data.type == EV_SYN)			break;			// syn�� ���� ������.
	}
	return 0;
}


//--------------------------------------------------------------
// ���� : mouse ���� �о� �´�.
// ���� :
//		*x	: REL_X ��
//		*y	: REL_Y ��
//		*btn: code ���� �̿��Ͽ� ���콺 ��ư ��ġ�� �Ѱ��ش�.
//			  ��ư�� ���ÿ� ������ ���� �� �������� ��Ʈ ����ũ �Ͽ� ���ÿ� ��ư�� ������ ���¸� �����Ѵ�.
//
//  [����1. ���콺 �����ʹ�ư�� ������, ���콺�� ������ ���]
//	type = 	EV_KEY, code = BTN_LEFT,	value = 1    <- ���콺 ���� ��ư�� ����
//	type = 	EV_REL, code = REL_X,		value = -14  <- X ��ǥ���� -14�̴�.
//	type = 	EV_REL, code = REL_Y,		value = 99   <- Y ��ǥ���� +99�̴�.
//	type = 	EV_SYN, code = 0,			value = 1    <- ���ݱ��� �����Ͱ� �ϳ��� �������̴�.
//
//	��ư�� ���� ���¿��� , ���콺�� �����̴� ���� Ȯ�� �Ϸ���,
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
					if(event_data.value == 1)	*btn |= IX_MOUSE_BTN_LEFT;		// ���콺 ���� ��ư�� ����
					else						*btn &= ~IX_MOUSE_BTN_LEFT;		// ���콺 ���� ��ư�� ���� ����
				 break;

			case BTN_RIGHT	:
					if(event_data.value == 1)	*btn |= IX_MOUSE_BTN_RIGHT;		// ���콺 ������ ��ư�� ����
					else						*btn &= ~IX_MOUSE_BTN_RIGHT;	// ���콺 ������ ��ư�� ���� ����
				 break;

			case BTN_MIDDLE :
					if(event_data.value == 1)	*btn |= IX_MOUSE_BTN_MIDDLE;	// ���콺 ��� ��ư�� ����
					else						*btn &= ~IX_MOUSE_BTN_MIDDLE;	// ���콺 ��� ��ư�� ���� ����
				 break;
			}
		}

		if( event_data.type	== EV_REL)
		{
			if(event_data.code == REL_X)	*x = event_data.value;				// x ���� ����

			if(event_data.code == REL_Y)	*y = event_data.value;				// y ���� ����
		}

		if(event_data.type == EV_SYN)			break;			// syn�� ���� ������.
	}
	return 0;
}



//--------------------------------------------------------------
// ���� : ��� ����̽��� ã�Ƽ� �����Ѵ�.
// ���� : imesu 2009�� 2�� 13��
//         cat /proc/bus/input/devices ���� input�� ���� ������ ���
//         event�� ���� ������ �ִ� ��찡 �ִ�.
//         �̰�� S: Sysfs=/class/input/input ���� ����̵� �̸� ������ ��� �� �� ���µ�
//         �� ����̽� ������ /proc/bus/input/devices ���� ��� �� �� �ֱ� ������ �����Ͽ���.
//         input �� ���� ������ ���ٸ�  -1�� �����ϰ� �����Ͽ���.
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

	ix_mng_count = 0;		// �ʱ�ȭ

	memset(ix_mng , 0, sizeof(ix_mng_t)*IX_INPUT_DEVICE_MAX);  // �ʱ�ȭ


	while(!feof(fp))
	{
		if(ix_mng_count >= IX_INPUT_DEVICE_MAX) 	break;

		memset(read_buf, 0, sizeof(read_buf));

		fgets(read_buf, sizeof(read_buf)-1, fp );			// �� �پ� �о�´�.
		//printf("[line data]%s" ,read_buf );
		if(read_buf[0] == 'S')			//	�Է� ����̽� ��ȣ�� ��ĵ�Ѵ�.
		{
			sscanf(read_buf, "S: Sysfs=/class/input/input%d" ,&tmp_num1);
			ix_mng[ix_mng_count].input_number = tmp_num1;
		}

		if(read_buf[0] == 'H')			//	�̺�Ʈ  ����̽� ��ȣ�� ��ĵ�Ѵ�.
		{
			find_str = strstr(read_buf, "event");			// ���ڿ��� "event" �� ���� ���� �˻��ϰ� , �ִٸ� �κ� �����͸� �Ѱ��ش�.
			if(find_str !=NULL)
			{
				sscanf(find_str, "event%d" ,&tmp_num1 );
				ix_mng[ix_mng_count].event_number = tmp_num1;
			}
			else //imesu 2�� 13�� �߰�
			{
				ix_mng[ix_mng_count].event_number = -1;
			}
		}

		if(read_buf[0] == 'N')							// imesu 2�� 13�� �߰� , �Ʒ� �ּ�ó���� �κ��� ��ü�ϴ� �κ�
		{
			name_ptr = strchr(read_buf,'=');
			name_ptr++;  								// = �� �ʿ� ��� ����
			tmp_ptr = strchr(read_buf,'\n');
			//printf(" %s \n",name_ptr);

			strncpy(ix_mng[ix_mng_count].name , name_ptr , tmp_ptr - name_ptr);		//
		}

		if(	read_buf[0] ==0x0a)	ix_mng_count++;	          // 0x0a �� LF �� '\r'
	}
	fclose(fp);

	// �Է� ��ġ major , minor ��ȣ�� ��´�.
	for(lp = 0 ; lp < ix_mng_count ; lp++)
	{
		sprintf(filename_buf,"/sys/class/input/event%d/dev" ,ix_mng[lp].event_number );

		fp = fopen(filename_buf, "r");
		if(fp !=NULL)
		{
			fscanf(fp , "%d:%d" ,&tmp_num1 ,&tmp_num2 );

			ix_mng[lp].dev_major = tmp_num1;				// major ��ȣ ����
			ix_mng[lp].dev_minor = tmp_num2;				// minor ��ȣ ����
			fclose(fp);
		}
		
		// ��ġ ����̽� �ΰ��� Ȯ���Ѵ�. 
		sprintf(filename_buf,"/sys/class/input/input%d/capabilities/abs" ,ix_mng[lp].input_number );

		fp = fopen(filename_buf, "r");
		if(fp !=NULL)
		{
			fscanf(fp , "%d" ,&tmp_num1  );
			
//			printf( "FILE %s [%d]\n", filename_buf, tmp_num1 );
			
			if( tmp_num1 >= 3 ) ix_mng[lp].is_touch = 1;			// ��ġ ����̽� �̴�. 
			else                ix_mng[lp].is_touch = 0;			// ��ġ ����̽� �ƴϴ�. 

			fclose(fp);
		}	
				
	}

	// ����� �޽���
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
// ���� : ����� ������ �̺�Ʈ�� �����Ѵ�.
//--------------------------------------------------------------
static int ix_copy_to_user()
{
	ie_event_t 		ie_data;							// input event data;
	int 			mx, my;
	int 			mkey;								// Ű���� �Է°�

	ix_get_mouse_position(&mx , &my);					// ���콺 ��ǥ���� �Ѱ��ش�.

	ix_get_key_value(&mkey);							// keyboard �Է� ���� �Ѱ��ش�.

	if( ix_mouse_event_flag & IX_MOUSE_FLAG_BTN_DOWN )
	{
		//printf("mouse down\n");
		ix_mouse_event_flag 	&= ~IX_MOUSE_FLAG_BTN_DOWN;
		ie_data.data.mouse.x 	= mx;							// x ��ǥ��
		ie_data.data.mouse.y 	= my;							// y ��ǥ��

		//printf("mx:%d, my:%d ",mx, my);	TRACE;
		ie_data.data.mouse.state= ix_button_state;				// ���°�
		ie_data.type 		 	= IE_MOUSE_DOWN;
		if(ix_event_func != NULL)  ix_event_func(&ie_data);
	}

	if( ix_mouse_event_flag & IX_MOUSE_FLAG_MOVE )
	{
		//printf("mouse move\n");
		ix_mouse_event_flag 	&= ~IX_MOUSE_FLAG_MOVE;
		ie_data.data.mouse.x	 = mx;
		ie_data.data.mouse.y	 = my;
		ie_data.data.mouse.state = ix_button_state;				// ���°�
		ie_data.type 			 = IE_MOUSE_MOVE;
		if(ix_event_func != NULL)  ix_event_func(&ie_data);
	}

	if( ix_mouse_event_flag & IX_MOUSE_FLAG_BTN_UP )
	{
		//printf("mouse up\n");
		ix_mouse_event_flag 	&= ~IX_MOUSE_FLAG_BTN_UP;
		ie_data.data.mouse.x	 = mx;
		ie_data.data.mouse.y	 = my;
		ie_data.data.mouse.state = ix_button_state;				// ���°�
		ie_data.type 			 = IE_MOUSE_UP;
		if(ix_event_func != NULL)  ix_event_func(&ie_data);
	}

	if( ix_key_event_flag & IX_KEY_VAULE_DOWN )					// key �� ���ȴ�.
	{
		//printf("IX_KEY_VAULE_DOWN  \n");
		ix_key_event_flag		&= 	~IX_KEY_VAULE_DOWN;
		ie_data.data.key.key	 = 	mkey;
		ie_data.type 			 = 	IE_KEY_DOWN;
		ie_data.data.key.state	 =	ix_button_state;				// ���°�
		if(ix_event_func != NULL)	ix_event_func(&ie_data);
	}

	if( ix_key_event_flag & IX_KEY_VAULE_UP )						// key �� ������ ���� �Ǿ���.
	{
		//printf("IX_KEY_VAULE_UP  \n");
		ix_key_event_flag    	&= 	~IX_KEY_VAULE_UP;
		ie_data.data.key.key	= 	mkey;
		ie_data.type 			= 	IE_KEY_UP;
		ie_data.data.key.state  = 	ix_button_state;				// ���°�
		if(ix_event_func != NULL)	ix_event_func(&ie_data);
	}

	if( ix_key_event_flag & IX_KEY_VAULE_PRESS )					// key ���� ����ڰ� ���������� �˼� �ִ� �ƽ�Ű ������ ��ȯ 
	{ 
		ix_get_key_press_value(&mkey, ix_button_state);
	
		//printf("IX_KEY_VAULE_PRESS %d \n", mkey);
		ix_key_event_flag   	&= 	~IX_KEY_VAULE_PRESS;
		ie_data.data.key.key	= 	mkey;
		ie_data.type 			= 	IE_KEY_PRESS;
		ie_data.data.key.state  =	ix_button_state;				// ���°�
		if(ix_event_func != NULL)	ix_event_func(&ie_data);
	}	
	return 0;
}

//--------------------------------------------------------------
// ���� : ���콺 ���� Ŭ�� ���� ���� 
// ���� : msec : ���콺 �Է��� ������ �ð� ����, ������  msec�̴�. 
//--------------------------------------------------------------
int  ix_set_mouse_double_click_intaval(int msec)
{
	ix_mouse_click_interval = msec;
	
	return ix_mouse_click_interval;
}


//--------------------------------------------------------------
// ���� : ���콺 ���� Ŭ���� Ȯ���Ѵ�.
// ��ȯ : 1  :  ����Ŭ�� ���� ,
//        0 :  ����Ŭ�� ���� 
//--------------------------------------------------------------
static int  ix_check_mouse_double_click( struct timeval cur_time)
{
/*
	int intaval=0;
	
	// �ð� Ȯ�� 
	intaval = (cur_time.tv_usec - ix_pre_time.tv_usec);

	printf("cur :%ld, old :%ld, intaval :%d \n", cur_time.tv_usec,ix_pre_time.tv_usec, intaval );
	if(intaval < ix_mouse_click_interval)
	{	// �ʱ�ȭ 
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
// ���� : input event tye key �Է� ó��
//
//--------------------------------------------------------------
static int ix_input_event_type_key(ix_event_t event_data)
{
	//printf("[code] 0x%x\n",event_data.code );
	switch(event_data.code)
	{
	case BTN_TOUCH  :					// touh �Է��� ���콺�� left�� ������ �Է��̴�.
	case BTN_LEFT	:
			if(event_data.value)	ix_button_state |= IX_MOUSE_BTN_LEFT;			// ���콺 ���� ��ư�� ����
			else					ix_button_state &= ~IX_MOUSE_BTN_LEFT;			// ���콺 ���� ��ư�� ���� ����
		 	break;

	case BTN_RIGHT	:
			if(event_data.value)	ix_button_state |= IX_MOUSE_BTN_RIGHT;			// ���콺 ������ ��ư�� ����
			else					ix_button_state &= ~IX_MOUSE_BTN_RIGHT;			// ���콺 ������ ��ư�� ���� ����
		 	break;

	case BTN_MIDDLE :
			if(event_data.value)	ix_button_state |= IX_MOUSE_BTN_MIDDLE;			// ���콺 ��� ��ư�� ����
			else					ix_button_state &= ~IX_MOUSE_BTN_MIDDLE;		// ���콺 ��� ��ư�� ���� ����
			break;

	case KEY_LEFTSHIFT :
			if(event_data.value)	ix_button_state |= IX_KEYBOARD_BTN_LEFT_SHIFT;		// Ű���� ���� shitf ��ư�� ����
			else					ix_button_state &= ~IX_KEYBOARD_BTN_LEFT_SHIFT;	// Ű���� ���� shitf ��ư�� ���� ����
		 	break;

	case KEY_RIGHTSHIFT   :
			if(event_data.value)	ix_button_state |= IX_KEYBOARD_BTN_RIGHT_SHIFT;	// Ű���� ������ shitf ��ư�� ����
			else					ix_button_state &= ~IX_KEYBOARD_BTN_RIGHT_SHIFT;	// Ű���� ������ shitf ��ư�� ���� ����
		 	break;

	case KEY_LEFTALT    :
			if(event_data.value)	ix_button_state |= IX_KEYBOARD_BTN_LEFT_ALT;		// Ű���� ������ ALT ��ư�� ����
			else					ix_button_state &= ~IX_KEYBOARD_BTN_LEFT_ALT;		// Ű���� ������ ALT ��ư�� ���� ����
		 	break;

	case KEY_RIGHTALT    :
			if(event_data.value)	ix_button_state |= IX_KEYBOARD_BTN_RIGHT_ALT;		// Ű���� ������ ALT ��ư�� ����
			else					ix_button_state &= ~IX_KEYBOARD_BTN_RIGHT_ALT;		// Ű���� ������ ALT ��ư�� ���� ����
		 	break;

	case KEY_LEFTCTRL     :
			if(event_data.value)	ix_button_state |= IX_KEYBOARD_BTN_LEFT_CTRL;		// Ű���� ������ CTRL ��ư�� ����
			else					ix_button_state &= ~IX_KEYBOARD_BTN_LEFT_CTRL;		// Ű���� ������ CTRL ��ư�� ���� ����
			break;

	case KEY_RIGHTCTRL     :
			if(event_data.value)	ix_button_state |= IX_KEYBOARD_BTN_RIGHT_CTRL;		// Ű���� ������ CTRL ��ư�� ����
			else					ix_button_state &= ~IX_KEYBOARD_BTN_RIGHT_CTRL;	// Ű���� ������ CTRL ��ư�� ���� ����
			break;

	case KEY_CAPSLOCK     :	// imesu 090215 
			if(event_data.value)	ix_button_state |= IX_KEYBOARD_CAPSLOCK;		// Ű���� ������ CTRL ��ư�� ����
			else					ix_button_state &= ~IX_KEYBOARD_CAPSLOCK;		// Ű���� ������ CTRL ��ư�� ���� ����			
			break;


	case KEY_NUMLOCK     :	// imesu 090215 
			if(event_data.value)	ix_button_state |= IX_KEYBOARD_NUMLOCK;			// Ű���� Num Lock ��ư�� ����
			else					ix_button_state &= ~IX_KEYBOARD_NUMLOCK;		// Ű���� Num Lock ��ư�� ���� ����
			break;

	case KEY_UP     :		// imesu 090215 
			if(event_data.value)	ix_button_state |= IX_KEYBOARD_UP;				// Ű���� ���� ����Ű ���� 
			else					ix_button_state &= ~IX_KEYBOARD_UP;				// Ű���� ���� ����Ű ���� ����
			break;

	case KEY_DOWN   :		// imesu 090215 
			if(event_data.value)	ix_button_state |= IX_KEYBOARD_DOWN;			// Ű���� �Ʒ��� ����Ű ���� 
			else					ix_button_state &= ~IX_KEYBOARD_DOWN;			// Ű���� �Ʒ��� ����Ű ���� ����
			break;
			
	case KEY_LEFT    :		// imesu 090215  ����Ű 
			if(event_data.value)	ix_button_state |= IX_KEYBOARD_LEFT;			// Ű���� ���� ����Ű ��ư ���� 
			else					ix_button_state &= ~IX_KEYBOARD_LEFT;			// Ű���� ���� ����Ű ��ư ���� ����
			break;

	case KEY_RIGHT    :		// imesu 090215 
			if(event_data.value)	ix_button_state |= IX_KEYBOARD_RIGHT;			// Ű���� ������ ����Ű ��ư ����
			else					ix_button_state &= ~IX_KEYBOARD_RIGHT;			// Ű���� ������ ����Ű ��ư ���� ���� 
			break;

	default : // ���� : ��ư ���� �������� Ű����� ó���ߴ�. ���߿� ������ �κ�
		  //if(event_data.value == 1)	ix_button_state= event_data.value;				// key�� ���������� ���� ������ �ִ´�.
		  	break;
	}

	// ���� Ŭ�� Ȯ�� 
	switch(event_data.code)
	{
	case BTN_TOUCH  :					// touh �Է��� ���콺�� left�� ������ �Է��̴�.
	case BTN_LEFT	: 			
			if(ix_check_mouse_double_click(event_data.time))
			{
				if(event_data.value)	ix_mouse_event_flag |=IX_MOUSE_FLAG_DOUBLE_CLICK;	//���콺�� ���ȴ�.
				else					ix_mouse_event_flag &=~IX_MOUSE_FLAG_DOUBLE_CLICK;		// ���콺�� ���� ����				
			}
			break;

	default :
		
			break;
	}
	
	// FLAG���� 
	switch(event_data.code)
	{
	case BTN_TOUCH  :					// touh �Է��� ���콺�� left�� ������ �Է��̴�.
	case BTN_LEFT	:
	case BTN_RIGHT	:
	case BTN_MIDDLE :
			if(event_data.value)	ix_mouse_event_flag |=IX_MOUSE_FLAG_BTN_DOWN;	//���콺�� ���ȴ�.
			else					ix_mouse_event_flag |=IX_MOUSE_FLAG_BTN_UP;		// ���콺�� ���� ����
			break;

	case KEY_CAPSLOCK     :				// imesu 090215 ��ư�� ������ DOWN -> UP�� �ȴ�. �� 2��ix_input_event_type_key()�� ȣ��ȴ�. �׷��� DOWN �ɶ� FLAG �����ϰ� �Ѵ�. 
			if(event_data.value)
			{
				ix_key_event_flag |= IX_KEY_VAULE_DOWN;								// Ű���� Num Lock ��ư�� ����
				ix_key_event_flag |= IX_KEY_VAULE_PRESS;							// key�� ���ȴ�. low code ���� ascii������ ��ȯ ex) KEY_1 -> 0x30				

				if(!ix_capslock_flag)		ix_capslock_flag = FLAG_SET;			// CapsLock ����� ���� �Ǿ���. 
				else 						ix_capslock_flag = FLAG_RELEASE;		// CapsLock ����� ���� �Ǿ���.
			}
			else
			{
				ix_key_event_flag |= IX_KEY_VAULE_UP;										
			}

			break;


	case KEY_NUMLOCK     :	// imesu 090215 
			if(event_data.value)
			{
				ix_key_event_flag 		|= IX_KEY_VAULE_DOWN;						// Ű���� Num Lock ��ư�� ����
				ix_key_event_flag 		|= IX_KEY_VAULE_PRESS;						// key�� ���ȴ�. low code ���� ascii������ ��ȯ ex) KEY_1 -> 0x30				

				if(!ix_numlock_flag) 	ix_numlock_flag = FLAG_SET;					// CapsLock ����� ���� �Ǿ���. 
				else 					ix_numlock_flag = FLAG_RELEASE;				// CapsLock ����� ���� �Ǿ���.
			}
			else
			{
				ix_key_event_flag |= IX_KEY_VAULE_UP;					
			}			
			break;	


	default : // ���� : ��ư ���� �������� Ű����� ó���ߴ�. ���߿� ������ �κ�
			ix_key = event_data.code;								// key ���� �ִ´�. 			
			
		  	if(event_data.value )
		  	{
		  		ix_key_event_flag |= IX_KEY_VAULE_DOWN;				// key�� ���ȴ�. low code ���� ����
		  		ix_key_event_flag |= IX_KEY_VAULE_PRESS;			// key�� ���ȴ�. low code ���� ascii������ ��ȯ ex) KEY_1 -> 0x30
		  	}
		  	else
		  	{
		  		ix_key_event_flag |= IX_KEY_VAULE_UP;				// key�� ���ȴ�.
			}			
			break;
	}
	return 0;
}


//--------------------------------------------------------------
// ���� :REL �Է� ���� ó���Ѵ�. 
//--------------------------------------------------------------
static int ix_input_event_type_rel(ix_event_t event_data)
{
	if(event_data.code == REL_X)
	{
		ix_mouse_x += event_data.value;							// x ���� ����

		if(ix_mouse_x < ix_sreen_x_min )	ix_mouse_x = ix_sreen_x_min;
		if(ix_mouse_x >= ix_sreen_x_max )	ix_mouse_x = ix_sreen_x_max-1;
	}
	if(event_data.code == REL_Y)
	{
		ix_mouse_y += event_data.value;							// y ���� ����

		if(ix_mouse_y < ix_sreen_y_min )	ix_mouse_y = ix_sreen_y_min;
		if(ix_mouse_y >= ix_sreen_y_max )	ix_mouse_y = ix_sreen_y_max-1;
	}

	ix_mouse_event_flag |=IX_MOUSE_FLAG_MOVE;

	return 0;
}

//--------------------------------------------------------------
// ���� : ABS �� ó��
//--------------------------------------------------------------
static int ix_input_event_type_abs(ix_event_t event_data)
{	

	int x, y;
	if(event_data.code == ABS_X)
	{
		ix_mouse_x = event_data.value;							// x ���� ����		

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
		ix_mouse_y = event_data.value;							// y ���� ����
		
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
// ���� : �̺�Ʈ �Է��� ó���Ѵ�.
//		*on_self �Էµ��� �Է��� ó���Ѵ�.
//       ��ġ�Է��� ���콺�� ������ �Է´�.
//--------------------------------------------------------------
static int ix_event_read(void *on_self )
{
	ex_mng_t		*mng      = NULL;

	ix_event_t 		event_data;

	mng = (ex_mng_t *)on_self;

	ix_read_event_data_low(mng->fd , &event_data);

	//ix_debug_disp_event_data(&event_data);						// �̺�Ʈ ������ �� Ȯ���Ѵ�.

	switch(event_data.type)
	{
	case EV_KEY :	ix_input_event_type_key(event_data);	break;

	case EV_REL :	ix_input_event_type_rel(event_data);	break;

	case EV_ABS :	ix_input_event_type_abs(event_data);	break;

	case EV_SYN :	ix_copy_to_user();		break;				// ����� ������ �̺�Ʈ�� �����Ѵ�.

	default : break;
	}

	return 0;

}


//--------------------------------------------------------------
// ���� : �˻��� ��ġ�� mknod �Ѵ�.
// 			1)�Է� ��ġ�� �ڵ� �˻�
// 			2) mknod
// 			3) ����̽� ���� ����..
// 			4) ex �Լ� ���
// �Ű� : path : �⺻ ���丮 ��ġ
// ��ȯ :
// ���� :
//--------------------------------------------------------------
int	ix_auto_scan_open( char *path )
{
	char 	device_path[PATH_MAX];							// 4096
	int		lp=0;

	// 1)�Է� ��ġ�� �ڵ� �˻�
	if(ix_scan_device() <0)
	{
		ix_error_code = IXERR_AUTOSCAN_SCANDEVICE;
		return -1;
	}
	
	// 2) mknod
	// 3) ����̽� ���� ����..
	for( lp= 0 ; lp < ix_mng_count;lp++)
	{
		sprintf (device_path, "%s/event%d", path, ix_mng[lp].event_number);
		//printf ("ix_auto_scan_open(): device_path=%s\n", device_path);

		ix_mng[lp].fd = ux_mknod_device_open(device_path ,ix_mng[lp].dev_major ,ix_mng[lp].dev_minor, O_RDWR );

		if(ix_mng[lp].fd < 0)	continue;

		// 4) ex �Լ� ���
		input_obj[lp] 			= ex_add_fd( ix_mng[lp].fd );

		if(input_obj[lp] == NULL) continue;

		input_obj[lp]->on_read  = ix_event_read;			// read
	}

	ex_adjust_poll();

	return 0;
}

//--------------------------------------------------------------
// ���� : ��ġ ���� ��ũ���͸� ��´�.  
//        �ڵ����� mknod �Ѵ�.
// 			1) �Է� ��ġ�� ��ġ �˻�
// 			2) mknod
// 			3) ����̽� ���� ����..
// 			4) ex �Լ� ���
// �Ű� : path : �⺻ ���丮 ��ġ
// ��ȯ :
// ���� :
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
// ���� : ��ũ�� ����� �����Ѵ�.
//        ���콺 ��ġ�� �����Ѵ�.
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
// ���� : ���콺 ��ġ�� ���´�.
//--------------------------------------------------------------
int ix_get_mouse_position(int *x, int *y)
{
	*x = ix_mouse_x;
	*y = ix_mouse_y;

	return 0;
}

//--------------------------------------------------------------
// ���� : �̺�Ʈ �����͸� ó���� �Լ��� ����Ѵ�.
//--------------------------------------------------------------
int ix_add_event(ix_event_func_t func)
{
	ix_event_func = (ix_event_func_t) func;

	return 0;
}

//--------------------------------------------------------------
// ���� : key �Է°��� ���´�.
//--------------------------------------------------------------
int ix_get_key_value(int *key)
{
	*key = ix_key;

	return 0;
}


//--------------------------------------------------------------
// ���� : �̺�Ʈ ��ġ�� ����ڰ� ���� �̺�Ʈ ������ �־ �̺�Ʈ�� �߻���Ų��.
// ���� : ���� ��ġ�� ������ ����� ��� ��� ��ġ �̺�Ʈ �ڵ鷯�� ����. 
//      : ��ġ�� ������ ����� ����� �̺κ��� ix_write_event_low()�Լ��� ��ġ�ؼ� ������� 
// imesu 2008-02-20
//--------------------------------------------------------------
static int ix_write_event_bulk_low(ix_event_t *event_data)
{
	int flag_write_event = -1;									// ix_write_event_low �Լ��� ������ üũ��
	int lp =0;

	if(event_data == NULL)	return -1;
		
	for(lp = 0; lp < ix_mng_count; lp++)
	{
		if(!(ix_write_event_low(ix_mng[lp].fd, event_data )))
		{
			flag_write_event = 0;	//�ѹ��̶� event write�� �����ϸ� ������ ���߻�!
		}
	}
	if(flag_write_event <0)
	{
		return -1;	//ix_write_event_low�Լ� ��� ����
	}
	return 0;

}

//--------------------------------------------------------------
// ���� : �̺�Ʈ ��ġ�� ����ڰ� ���� �̺�Ʈ ������ �־ �̺�Ʈ�� �߻���Ų��.
//--------------------------------------------------------------
int ix_write_event_low(int fd, ix_event_t *event_data )
{
	if( fd < 0 || event_data == NULL )
	{
		ix_error_code = IXERR_WRITE_EVENT; 		// ����
		return -1;
	}

	write( fd, event_data, sizeof(ix_event_t));
	return 0;
}


//--------------------------------------------------------------
// ���� : ���콺 Ư����ư�� Up down ������ �߻���Ų��.
//--------------------------------------------------------------
int ix_write_mouse_button_action(int mouse_button, int action)
{
	ix_event_t event_data;
	
	//step1: event ����ü�� ����� �� ����
    gettimeofday( &event_data.time, NULL );
	event_data.type = EV_KEY;

	//step2: event ����ü�� ��ư ������ ����
	switch(mouse_button)
	{
	case IX_MOUSE_BTN_LEFT:		event_data.code = BTN_LEFT;							break;
	case IX_MOUSE_BTN_RIGHT:	event_data.code = BTN_RIGHT;						break;
	case IX_MOUSE_BTN_MIDDLE:	event_data.code = BTN_MIDDLE;						break;
	default:					ix_error_code   = IXERR_WRITE_MOUSE_BUTTON_EVENT; 	return -1;	// ����
	}

	//step3: event ����ü�� ��ư ���°� ����
	switch(action)
	{
	case IE_MOUSE_DOWN:	event_data.value = IX_MOUSE_VAULE_DOWN_LOW;					break;
	case IE_MOUSE_UP:	event_data.value = IX_MOUSE_VAULE_UP_LOW;					break;
	default:			ix_error_code    = IXERR_WRITE_MOUSE_BUTTON_EVENT; 		return -1;	// ����
	}
	
	// �̺�Ʈ�� ��� �ڵ鷯�� ����. ... ���߿� ��ġ ������ �Ǹ�, �ش� �ڵ鷯���� ���� ��������.!! 
	if(ix_write_event_bulk_low(&event_data) <0)		return -1;

	//step4: SYC �߻�
    //gettimeofday( &event_data.time, NULL );

	event_data.type  = EV_SYN;
	event_data.code  = SYN_REPORT;
	event_data.value = 0;


	// �̺�Ʈ�� ��� �ڵ鷯�� ����. ... ���߿� ��ġ ������ �Ǹ�, �ش� �ڵ鷯���� ���� ��������.!! 
	if(ix_write_event_bulk_low(&event_data) <0)		return -1;

	return 0;

}

//--------------------------------------------------------------
// ���� : ���콺 ��ġ ���� �̺�Ʈ�� �߻���Ų��.
// ��ȯ : 0 :����, -1; ���� 
//--------------------------------------------------------------
int ix_write_mouse_rel_move(int rel_x, int rel_y)
{
	ix_event_t event_data;

	//step1: event ����ü�� ����� �� ����
    gettimeofday( &event_data.time, NULL );
	event_data.type = EV_REL;
	
	//step2: event ����ü�� X ���氪 ����
	event_data.code = REL_X;
	event_data.value = rel_x;

	// �̺�Ʈ�� ��� �ڵ鷯�� ����. ... ���߿� ��ġ ������ �Ǹ�, �ش� �ڵ鷯���� ���� ��������.!! 
	if(ix_write_event_bulk_low(&event_data) <0)		return -1;
	//step3: event ����ü�� Y ���氪 ����
	event_data.code = REL_Y;
	event_data.value = rel_y;
	
	// �̺�Ʈ�� ��� �ڵ鷯�� ����. ... ���߿� ��ġ ������ �Ǹ�, �ش� �ڵ鷯���� ���� ��������.!! 
	if(ix_write_event_bulk_low(&event_data) <0)		return -1;
	
	//step4: SYC �߻�

    //gettimeofday( &event_data.time, NULL );
	event_data.type  = EV_SYN;
	event_data.code  = SYN_REPORT;
	event_data.value = 0;

	// �̺�Ʈ�� ��� �ڵ鷯�� ����. ... ���߿� ��ġ ������ �Ǹ�, �ش� �ڵ鷯���� ���� ��������.!! 
	if(ix_write_event_bulk_low(&event_data) <0)		return -1;

	return 0;
}

//--------------------------------------------------------------
// ���� : Ű���忡 Ű �̺�Ʈ�� �߻���Ų��.
// �Ű�	:
//			keyboard_key: keyboard�� Ű code�� �ǹ���. ����: Linux Ŀ�μҼ�/include/linux/input.h�� 'Keys and buttons' �׸�
//
//			action: �ش� Ű�� �߻���Ű�� ���� ����
//
//			IE_KEY_DOWN
//			IE_KEY_UP
//--------------------------------------------------------------
int ix_write_keyboard_key_action(int keyboard_key, int action)
{
	ix_event_t event_data;

	//step1: event ����ü�� ����� �� ����
    gettimeofday( &event_data.time, NULL );
	event_data.type = EV_KEY;

	//step2: event ����ü�� KEY �� ����
	event_data.code = keyboard_key;
	
	//step3: event ����ü�� �׼� �� ����
	switch(action)
	{
	case IE_KEY_DOWN:	event_data.value = 1;							break;
	case IE_KEY_UP:		event_data.value = 0;							break;
	default:			ix_error_code    = IXERR_WRITE_KEYBOARD_EVENT; 	return -1;	// ����
	}

	// �̺�Ʈ�� ��� �ڵ鷯�� ����. ... ���߿� ��ġ ������ �Ǹ�, �ش� �ڵ鷯���� ���� ��������.!! 
	if(ix_write_event_bulk_low(&event_data) <0)		return -1;

	//step4: SYC �߻�
    //gettimeofday( &event_data.time, NULL );
	event_data.type  = EV_SYN;
	event_data.code  = SYN_REPORT;
	event_data.value = 0;

	// �̺�Ʈ�� ��� �ڵ鷯�� ����. ... ���߿� ��ġ ������ �Ǹ�, �ش� �ڵ鷯���� ���� ��������.!! 
	if(ix_write_event_bulk_low(&event_data) <0)		return -1;
	
	return 0;
}

//--------------------------------------------------------------
// ���� : �Է� ���� ����ڰ� �ٷ� ����� �� �ִ� �ƽ�Ű �ڵ�� ��ȯ�Ͽ� ��ȯ�Ѵ�. 
//        ���� ��� KEY_1�� 0x30���� ��ȯ�Ͽ� �����Ѵ�. 
//        Shift , ALT, CTRLŰ�� ����Ѵ�. 
// http://lxr.falinux.com/source/include/linux/input.h �� �ִ� Ű ���� ���� Ű���� �迭��� �����Ǿ� ��ִ? 
//--------------------------------------------------------------
static int ix_get_key_press_value(int *m_key, ix_btn_state_t button_state)
{
	int shift_flag	= 0;
	int askii		= 0;

	//printf("[*m_key]:0x%x ", *m_key);  TRACE;
	if(*m_key > IX_KEY_PRESS_MAX) return -1;
	
	if( (button_state & IX_KEYBOARD_BTN_LEFT_SHIFT)||(button_state & IX_KEYBOARD_BTN_RIGHT_SHIFT) )
	{	
		shift_flag = FLAG_SET;											// shift �� ������ ������ flag�� �����Ѵ�. 
	}

	if(KEY_1 <= *m_key && *m_key <=KEY_SLASH )		
	{
		askii = ix_key_to_askii[*m_key]; 								// KEY_XXX �Է� ���� �ƽ�Ű �ڵ� ������ ��ȯ 

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
// ���� : ��ġ ��ũ�� ������ ��ġ���� ��� �´�. 
// imesu 2009-02-20
//--------------------------------------------------------------
int ix_touch_calcurator(int *x, int *y)
{
	int tx =0;	
	int ty =0;

	int sx = 0; 
	int sy = 0;
	
	ix_get_mouse_position(&tx,&ty);							// �������� ��ġ��ũ�� ���� ��� �´�. 

	printf("ix_adjust_touch_info.xa:%d ",ix_adjust_touch_info.xa);	TRACE;
	printf("ix_adjust_touch_info.xb:%d ",ix_adjust_touch_info.xb);	TRACE;
	printf("ix_adjust_touch_info.ya:%d ",ix_adjust_touch_info.ya);	TRACE;
	printf("ix_adjust_touch_info.yb:%d ",ix_adjust_touch_info.yb);	TRACE;

	if(ix_adjust_touch_info.xa ==0)	return -1;				// �и� 0�̸� ������. 
		
	sx = (tx-ix_adjust_touch_info.xb)/ix_adjust_touch_info.xa;	
	if(sx <= 0)
	{
		sx =0;
	}
	else if(sx >= ix_sreen_x_max)
	{
		sx = ix_sreen_x_max; 
	}

	if(ix_adjust_touch_info.ya ==0)	return -1;				// �и� 0�̸� ������. 
	
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
// ���� : ���� Ű���忡�� ���� �Է��� , ���� �Է� ������ ��ȯ ( �ƽ�Ű -> KEY_XX)
// imesu 2009-02-20
// ���� : 
//         key :  �ƽ�Ű �ڵ尪�� �Է� �޴´�. 
// ���� : ix_write_key_press('A');  ix_write_key_press(3)
// ���� : ���ڷ� ���� 1�� �� ������. 
// TO DO : �ҹ��� ó�� 
//--------------------------------------------------------------
int ix_write_key_press(int ascii)
{
	int key_xxx =0;
	ix_event_t event_data;

	if(128 < ascii )	return -1;

	// �ҹ��ڰ� ��� ���� �ҹ��� ó�� 
	if(ASKII_a <= ascii && ascii<=ASKII_z)
	{
		ix_button_state |= IX_KEYBOARD_BTN_LEFT_SHIFT;
	}
	else
	{
		ix_button_state &= ~IX_KEYBOARD_BTN_LEFT_SHIFT;	// Ű���� ���� shitf ��ư�� ���� ��?
		TRACE;
	}	
	
	key_xxx = ix_askii[ascii];

	//printf("key_xxx :%d , ascii:0x%x " ,key_xxx, ascii);	TRACE;	

	//step1: event ����ü�� ����� �� ����
    gettimeofday( &event_data.time, NULL );
	event_data.type = EV_KEY;

	//step2: event ����ü�� KEY �� ����
	event_data.code = key_xxx;
	
	//step3: event ����ü�� �׼� �� ����
	event_data.value = 1;

	// �̺�Ʈ�� ��� �ڵ鷯�� ����. ... ���߿� ��ġ ������ �Ǹ�, �ش� �ڵ鷯���� ���� ��������.!! 
	if(ix_write_event_bulk_low(&event_data) <0)		return -1;

	//step4: SYC �߻�
    //gettimeofday( &event_data.time, NULL );
	event_data.type  = EV_SYN;
	event_data.code  = SYN_REPORT;
	event_data.value = 0;

	// �̺�Ʈ�� ��� �ڵ鷯�� ����. ... ���߿� ��ġ ������ �Ǹ�, �ش� �ڵ鷯���� ���� ��������.!! 
	if(ix_write_event_bulk_low(&event_data) <0)		return -1;

	if(ix_write_keyboard_key_action(key_xxx, IE_KEY_UP) <0 ) return -1;
	
	return 0;	
}


//--------------------------------------------------------------
// ���� : ���� Ű���忡�� ���� �Է¿��� , ���� �Է� ������ ��ȯ ( �ƽ�Ű -> KEY_XX)
// imesu 2009-02-20
// ���� : 
//         str :  ���ڿ��� �Է� �޴´�. 
//         size : ���ڿ��� ������ 
// ���� : ix_write_key_press('ABC', 3);
//--------------------------------------------------------------
int ix_write_key_string_press(char *str, int size)
{
	return 0;

}


//--------------------------------------------------------------
// ���� : ��ġ ��ũ�� Į���� ���̼� ������ �ε��Ѵ�. 
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

		if(linebuf[0] == '#')  continue;			// �ּ��� �����Ѵ�. 

		sscanf(linebuf ,"%s", cmd);

		for( tag_index=0 ; tag_index < 4 ; tag_index++ )
		{
			if(!strcmp(cmd , tag[tag_index]))
			{
				for(lp=0 ; lp <4 ; lp++)
				{
					memset(linebuf, 0, sizeof(linebuf));	
					memset(cmd    , 0, sizeof(cmd));
					
					fgets(linebuf, sizeof(linebuf)-1 , fd);		// ���پ� �о� �´�.
					if(linebuf[0] == '#')  continue;			// �ּ��� �����Ѵ�. 
					
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
// ���� : ��ġ ��ũ�� Į���� ���̼� ������ �����Ѵ�.
// 1) section�� ã�´�. 
// keyname  �� '1='
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

		if(linebuf[0] == '#')  continue;			// �ּ��� �����Ѵ�. 

		sscanf(linebuf ,"%s", cmd);
		
		if(!strcmp(cmd , section))					// ������ ���Ѵ�. 
		{	
			printf("[cmd]:%s [section]:%s \n",cmd ,section);
		
			for(lp=0 ; lp <4 ; lp++)
			{
				memset(linebuf, 0, sizeof(linebuf));
				memset(cmd    , 0, sizeof(cmd));
				
				fgets(linebuf, sizeof(linebuf)-1 , fd);		// ���پ� �о� �´�. �������� \n �� ���Ե� 
				size = sizeof(linebuf);
				printf("[size]:%d \n",size);
				fseek(fd, -size, SEEK_CUR	);
				
				if(linebuf[0] == '#')  continue;			// �ּ��� �����Ѵ�. 
				
				if(atoi(&linebuf[0]) == (lp+1))				// key�� ã�´�. 
				{
					// keyname �� Ȯ���Ѵ�. 
					if(!strncmp(linebuf , keyname, 2 ))				
					{		
						
						printf("linebuf:%d , keyname:%s   \n",atoi(&linebuf[0]) ,keyname );
						printf("lp�� key %s �� ���� ����. \n", keyname);
						fprintf(fd, "\r%s%d",keyname,value );	// ���� , ?? �� ��ĭ �Ʒ��� ������?																// fgets ������ �̵��Ѵ�. 
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
// ���� : touch screen ���� �����ϱ� ���� ���� �ִ´�.
// ��ȯ : -1 �̸� ���� �߻� , 0�̸� ����
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
// ���� : touch screen ���� ������ ���� ���� ���� �����Ѵ�. 
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
// ���� : 
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
// ���� : ix_adjust_touch_info ���� ���� �Ѵ�
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
// ���� : ix_adjust_touch_info ���� �����´�
//--------------------------------------------------------------
int ix_get_touch_info( ix_adjust_touch_info_t *info )
{	
    *info = ix_adjust_touch_info;
	return 1;	
}


//--------------------------------------------------------------
// ���� : ix_adjust_touch_info ���� ����Ʈ �Ѵ�
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
// ���� : ix_adjust_touch_info ���� �����ϱ� ���� �ʱ�ȭ 
//--------------------------------------------------------------
int ix_adjust_touch_init( void )
{
	//��ġ��ũ�� �������� �б�
	ix_loading_ts_cailbration_info ();

	//������ ���� (ix_adjust_touch_flag=1)
	ix_adjust_touch ();

	//�������� ���
	ix_print_touch ();

	return 0;
}

