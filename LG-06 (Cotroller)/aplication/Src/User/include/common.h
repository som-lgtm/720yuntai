#ifndef __COMMON_H
#define __COMMON_H
#include "stm32f030x8.h"

#define LOCUS_PAGE		0
#define SET_A_POINT 	1
#define SET_B_POINT 	2
#define MAIN_ID			3
#define SINGLE_PANORAMA		4 //单层全景
#define SINGLE_PANORAMA_11	5 //单层全景模式第二页
#define DELAY_SET		6
#define DELAY_DIS			10
#define VIDEO_MODE			11
#define CONFIG_ID		9
#define PREINSTALL_MODE	7 //专业模式界面1
#define PREINSTALL_SET		8 // 专业模式界面2，参数的添加
#define PREINSTALL_SESHOT 12 // 专业模式补拍界面
#define PREINSTALL_SET1 13 // 专业模式界面3,参数的设置
#define PREINSTALL_MOVE 14 // 专业模式MOVE界面3

#define ORIGIN_SET_MOVE 15
#define REVERSE_MODE	16 //提示是否进行AB 点设置
#define FINDING_ORIGIN 	17
#define MANUAL_MODE 	18
#define ORIGIN_ACK		19 //提示是否进行原点设置

#define PAGE_MAX		20


#define SECONDS_DIS		1
#define MINUTES_DIS		2
#define HOURS_DIS		3


#define SPEED_GUDIN 90

#define TRUE	1
#define FALSE	0
#define NOT_EMPTY	0xff

#define PARA_ERROR	20

#define ENGLISH		1
#define CHINESE		0
#define NELANDS		7 // Netherlands 
#define FRENCH 		2 // Fran?ais 
#define SPANISH		6
#define ITALIANO	5
#define GERMAN		3
#define JAPANESE	4
#define KOREANH		8

#define ORBITAL		0
#define YUNTAI		1

#define A_POINT_END		3000
#define B_POINT_END		1000

#define A_TO_B		0
#define B_TO_A		0xff
#define SE_AB_PINT_ENABLE	3

//#define TRUE		1
#define SLIDER_CONTROLOR	0X01
#define YUNTAI_CONTROLOR	0X02
#define SMART_CAR			0X03
#define CONNECTION_FLAG		0X04

#define VIDEO_SHOOTING		0X01
#define TIME_LAPSE			0X02
#define AB_POINT_SET			0X03
#define PANORAMA_SHOOT		0X03
#define CUSTOM_SHOOT		0X04

#define SLOW_START	0x01
#define SLOW_STOP	0x02
#define PRESCALE_MAX	(ORBITAL_SPEED_MAX-100)*2

#define NODE_A		0
#define NODE_B		1
#define NODE_C		2
#define NODE_D		3

#define LOCUS_A		0
#define LOCUS_B		1
#define LOCUS_C		2

#define CHARGING	0
#define N_CHARGE	1

#define PARA_MAXID	24

typedef enum
{
	REACHED_ABPOINT=0, 	//Point A or B has been reached
	LOOKING_A_POINT, 		//Finding the a piont
	LOOKING_B_POINT, 		//Finding the a piont
	LOOKING_ORIGIN_POINT, 	//Finding the origin piont
	REACHED_ORIGIN_POINT 	//Finding the origin piont
	
}MOTOR_STATUS;

typedef enum
{
	G30 = 1,
	ER1,
	CAR
	
}PRODUCT;


typedef enum
{
	DEVICE = 1,
	SHOOT_MODE,
	DIRECTION_SEL,
	SPEED_SET,
	COMFIRM,
	SL_FRAME_TIME,
	SL_SHUTTER_TIME,
	SL_PHOTOMUN,
	APK_CONNECTING
	
}APP_PROTOCOL;

typedef enum
{
	SLIDER_VIDEO=1,
	SLIDER_TIME_LAPSE,
	YUNTAI_VIDEO,
	YUNTAI_TIME_LAPSE,
	YUNTAI_PANORAMIC,
}MODE_SELECT;

typedef enum
{
  FLASH_BUSY = 1,
  FLASH_ERROR_WRP,
  FLASH_ERROR_PROGRAM,
  FLASH_COMPLETE,
  FLASH_TIMEOUT
}FLASH_Status;

typedef struct{
	uint16_t ele_angle; // 仰角
	uint16_t lev_angle; //水平角
	uint16_t shut_times; //快门次数，即每个角度需要拍的张数
	uint8_t amout;
	uint8_t exposure; //快门速度
	uint8_t interval; // 缓停时间
	uint8_t set_flag;
	uint8_t sys_stop_t; // 快门拍完关闭之后需要等待的时间和能开始下一张的动作
}MODE_PARA;


typedef struct{
	uint8_t set_tag;
	MODE_PARA modes[PARA_MAXID+1];
	uint8_t delay_t; // 延迟开始时间
	uint8_t manul; // 手动功能，默认为关闭，实现自动功能
	uint16_t total_aount;
	uint8_t para_id;
	uint8_t orbital_speed; // 速度档位
}SPECIALTY_PARA;

typedef struct{
	uint8_t id_default;
	uint8_t language_sel;
	uint8_t wifi_rfc;
	uint8_t TX_adress[5];
	uint8_t usart_number;			  /* usart select: usart 1==AA or usart 2==BB*/
	uint8_t ISR_tag;                /*the flag of isr source */
	uint8_t brightness;
	uint8_t sleep_time;
	uint8_t wakaup_tag;
	uint8_t brightback;
}ID_CODE;


typedef struct{
	uint8_t read_fisrt;
	//uint8_t language_sel;
	uint8_t orbital_dir;
	uint8_t orbital_speed; // 速度档位
	uint8_t auto_return_tag;
	uint8_t stop_shoot;
	uint8_t delay_time_s; //标准模式下的推迟启动的时间
	uint8_t delay_time_m;
	uint8_t delay_time_h;
	uint8_t wid_time_s; //广角模式下的推迟启动的时间
	
	uint8_t fram_time_h; // usual timelapse
	uint8_t fram_time_m;
	uint8_t fram_time_s;

	uint8_t shut_time_h; // usual timelapse
	uint8_t shut_time_m;
	float shut_time_s;
	float ssinterval;
	
	uint8_t tatol_time_m; // usual timelapse
	uint16_t tatol_time_h;
	uint8_t tatol_time_s;

	//uint8_t exposure_h;
	//uint8_t exposure_m;
	
	
	
	uint8_t duration_s; // advance video move time
	uint8_t duration_m;
	uint8_t duration_h;
	uint8_t fps;
	uint8_t play_back_s; // playback time
	uint8_t play_back_m; // playback time
	uint8_t play_back_h; // playback time
	uint16_t playback;
	uint32_t shoot_amount; //张数
	
	uint32_t a_point;
	uint32_t b_point;
	int  pulse_cout;
	uint32_t sd_amount; //单层全景模式张数
	uint32_t wid_amount; //广角模式张数


	uint16_t interval;
	uint16_t exposure;
	uint16_t shut_times; //快门次数，即每个角度需要拍的张数
	float sys_stop_t; // 快门拍完关闭之后需要等待的时间和能开始下一张的动作
	
}PARA_STRUCT;


typedef struct{
	int pose_a;
	int pose_b;

	uint8_t pose_dir_atob;
	uint8_t locus_tag;
	uint32_t locus_distance;
 
 }SLIDER_POSE_STRUCT;

typedef struct{

	int pose_a;
	int pose_b;

	uint8_t pose_dir_atob;
	uint32_t locus_distance;
	
}YUNTAI_POSE_STRUCT;


void if_write_flash(void);
void write_flash_active(void);
void time_delay_ms(uint16_t counts);
uint8_t usart_read_byte(void);
void usart_syn_data_to_apk(void);
void mcu_set_data_to_apk(uint8_t opcode, uint16_t data);
void usart_timer_test(void);
void adc_cout(void);
void A_d_c_int(void);
void read_ADC_value(void);
void motor_init(void);
void motor_slowly_startedORstop(void);
void time_count(void);
void delay_mode_main(void);
void Feed_IWDG(void);
void slow_spee_check(void);
void blue_led_status(void);
void delay_move_time_compara(void);
void para_read_from_flash(void);
void para_init(void);
void boot_screen(void);
void GPIO_OUT_init(GPIO_TypeDef *GPIOx, uint32_t PinMask);
void keyscan(void);
void Lcd_Init(void);
void motor_start(void);
void motor_stop(void);
void motor_direction_change(void);
void motor_speed_adjust(uint8_t p_speed);
void auto_return_display(uint8_t cur);
void cursor_count(uint8_t dir, uint8_t max, uint8_t mix);////////////////////////////////////////////////////////////////
void keyscan_pin_init(void);
void scan_cycle_count(void);
void auto_return_check(void);
void slider_upORdown(uint8_t p_type);
void Logo_display(void);
void dynamic_speed_calculat(uint8_t typep);
void set_boot_time(uint16_t timep);
void auto_stop_1S(void);
void set_1S(uint8_t stop_timep);
void display_test(uint8_t param, uint8_t param11);

///////////////////////yun/////////////////////
void yuntai_direction_change(void);
void yuntai_speed_change(uint8_t p_speed);
void yuntai_start(void);
void yuntai_stop(void);
//////////////////////////////////////////////
void slider_change_node(void);
void yuntai_check_node(void);
void control_sendto_A650(void);
void control_sendto_E200(void);
void Send_to_E200_enable(void);

uint8_t check_sum_add(uint8_t index, uint8_t *spt);
void display_test11(uint8_t param, uint8_t cur);
void receiver_data_from_A650(void);
void get_init_from_A650(void);
void wait_for_init(void);

void usart22_read_byte(void);
void usart22_Recdata_Handling(void);
void Usart22_Open_Send_Dma(void);
void Usart22_Dma_SendIRQ_Dispose (void);

void usart11_Recdata_Handling(void);
void Open_Send_Dma(void);
void Dma_SendIRQ_Dispose (void);
void usart1_receiver(void);
void send_interval_count(void);
void Usart22_Interval_set(void);
uint8_t Check_Buffer_Empty(void);
void Burn_code_value(void);
uint8_t Usart22_return_RX_Complete(void);
void pair_key_check(void);
void check_battery_low(void);
void Return_main_manual(void);
void Set_idCode(void);

void Device_init(void);
void test_ttt(uint8_t x);
void Pairing_time(void);
// low_power.c
void Time_Out_And_Enter_Stop_Mode(void)  ;
void active_time_out_countdown(void);
void set_active_time_out(void);
void Enter_Sleep_Mode(void);
void ShutDown(void);

//USART.C
void String_Printf(uint8_t *sptf, uint8_t sizes);

//BOOTLOADER.C
void bootloader_action_if(uint8_t *spt);
void check_enter_boot_if(uint8_t *spt);

// orbira.c
void change_page(void);
void Time_DCount(void);
void check_connection(void);
void controller_send_data_to_motor(uint8_t opcode, uint8_t mode, uint8_t data);

// rf24l01.c
void Enter_into_Pair(void);
void RF24L01_GPIO_Init(void);
// keyscan.c
void Key_To_Pair(void);
void set_press_time(void);
uint8_t return_powers(void);

//flash.c
void check_uid(void);

// oled.c
void version_dis(void);
void battery_init(void);
void TurnOff_ShutDonw_Battery_dis(uint8_t type, float datat, uint8_t bbb);

// main.c
void MX_TIM3_Init(void);
void SystemClock_Config(void);

// adc,c
void Set_adc_back(void);
//AB_SET.C
void ab_set_move(uint8_t dir, uint8_t types);
uint8_t check_abpoint_Set_if(uint8_t modes);
void Set_ab_set_if(uint8_t datas);
void Set_DDab_set_if(uint8_t datas);
void set_mode_back(uint8_t datad);
uint8_t return_mode_back(void);

// specialty_mode.c
void specialty_mode_OKkey(void);
void specialty_page_mode_dis(void);
void specialty_cursor_id_shift(uint8_t dir);
void specialty_cursor_id_dis(uint8_t dir);
void specielty_para_adjust(uint8_t dir);
void specielty_para_dis(uint8_t cur);
void jiantou_display(void);
void specialty_para_clear(void);
void pecialty_pix_amt_display(uint8_t cur);
void specialty_move_dis(uint8_t cur);
void specialty_para_sendTo_motor(uint8_t curt);
void mode_and_para_sendt(uint8_t types);
void delet_paraORmode(void);
void specialty_totaol_time_dis(void);
void Get_data_from_Motor(uint8_t *fofob);
void specilty_clear_han_display(uint8_t cur);
void pwr_key_enter_moveInterfer(void);
void splty_mode_time_count(void);
void Set_enter_time(void);
void specialty_page_para_dis(void);
void ReturnOrNext_dis(void);
void Page_preinstall_move_return(void);
void specialty_mode_para_clear(void);
void Cunrent_para_display(uint8_t cur);
void shotting_Get_data_from_controller(uint8_t *sptt);
void Current_Status_display(uint8_t statuss);
void cear_the_id_add(uint8_t datas);
void specilty_keyscan_send(uint8_t dir);
void specilty_page_return(void);
void specilty_get_reshot_form_motor(uint8_t datas);

//video.c
void Video_times_downcount(void);
void Video_check_the_motor_status(MOTOR_STATUS statuss);
uint8_t Video_motor_status_return(void);
void Video_Get_downcount_times(void);
void count_tag_set(uint8_t dat);


extern ID_CODE wifi_id;
extern PARA_STRUCT glob_para;
extern YUNTAI_POSE_STRUCT yuPoses[3];
extern uint8_t *ToE200;

extern uint8_t page_id;
extern uint8_t cursor_id_back;
extern uint8_t cursor_id;
extern uint8_t m_start ;
extern uint8_t slowly_now;
extern uint16_t MS_count;
extern uint8_t ab_set_if;
extern uint8_t Inverse_Dis;
extern uint8_t Factory_set;
extern uint32_t boot_time;
extern uint8_t ramp_flag;
extern uint8_t ramp_flag11;
extern uint8_t reverse_cur;
extern uint8_t p_upORdown;
extern uint16_t delay_speed;
extern uint8_t delay_time_set;
extern uint8_t check_start_point;
extern uint8_t adj_parm_sel;
extern uint8_t locus_numb;
extern uint8_t auto_fine_abpoint;
extern uint8_t product_s;
extern uint8_t charge_tag;
extern uint8_t Battery_percentage;
extern uint8_t mode_id;
extern uint8_t mid_cr;
extern uint8_t mid_cr_back;
extern uint8_t specialty_get;
extern uint8_t sp_start;

//extern uint8_t lowbattery_dis;
//extern uint8_t lowbattery_dis_if;

#endif

