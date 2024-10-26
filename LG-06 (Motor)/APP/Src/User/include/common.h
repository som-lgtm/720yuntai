#ifndef __COMMON_H
#define __COMMON_H
#include "stm32f0xx.h"

#define TRUE	1

#define FALSE	0

#define STAND_IDMAX	5

#define FW_VERSIONS	122

#define PARA_MAX_ID	25


#define BATERRY_LOW	0xFF


#define SLOW_START	0x01
#define SLOW_STOP	0x02

#define SET_ORIGIN		15
#define MAIN_ID			3
#define PANORAMA_MAIN		4
#define STANDAR_MODE		4
#define GROUP_PHOTO		5
#define WIDE_ANGLE_MODE	6
#define PREINSTALL_MODE	7 //专业模式界面1
#define PREINSTALL_SET		8 // 专业模式界面2，参数的添加
#define DELAY_SET		2
#define DELAY_DIS			10
#define VIDEO_MODE			1
#define MANUAL_MODE			9


//#define ORIGIN_POINT		0x80000000
//#define A_POINT_END		0x7FFE0CFA // 0x7FFD53B1负的360度
//#define B_POINT_END		0x8001F401// 0x8002AC4F // 正的360度
//#define M_POINT_END		0x8000FA00 //0x80015627 // 正的180度
//#define POINT_360_END		(B_POINT_END-ORIGIN_POINT) //360脉冲数

#define STANDARD_SPEED		100
#define DELAY_SPEED	60 //定时器预分频数

#define RAM_ID	50 //15

#define SLOW_BASIC_TIME11	30 // 79

#define MODE_ID_MAX	8 // 专业模式的预设最大只有8个

// video.c宏定义
#define REACHED_ABPOINT 0 //Point A or B has been reached
#define LOOKING_A_POINT 1 //Finding the a piont
#define LOOKING_B_POINT 2 //Finding the a piont
#define LOOKING_ORIGIN_POINT 3 //Finding the origin piont
#define REACHED_ORIGIN_POINT 4 //Finding the origin piont

typedef enum
{
  MOTOR_Vertical=0, // 垂直轴电机
  MOTOR_HORITAL = 1, // 水平轴电机
}MOTOR_TYPE;

typedef enum
{
  USART_1 = 1,
  USART_2,
  USART_SEND,
  USART_READ
}USART_NUM;


typedef enum
{
  FLASH_BUSY = 1,
  FLASH_ERROR_WRP,
  FLASH_ERROR_PROGRAM,
  FLASH_COMPLETE,
  FLASH_TIMEOUT
}FLASH_Status;

typedef struct{
	uint16_t lev_angle; //水平角
	uint8_t amout;
	uint16_t exposure;
	uint16_t interval;
	uint8_t set_flag;
	uint8_t shut_times; //快门次数
	uint16_t sys_stop; //延时时间，快门关闭后的等待时间
}SPECIALTY_PARA;

typedef struct{
	uint32_t lev_pulse; // 水平每张照片的脉 冲
	uint32_t ele_pulse; // 仰角的脉冲
	uint8_t ele_dir; //  仰角的方向
	uint8_t speed;
}SPEDCIALTY_MOVE_PARA;

typedef struct{
	uint8_t set_tag;
	SPECIALTY_PARA Param[PARA_MAX_ID];
	uint8_t delay_t; // 延迟开始时间
	uint8_t manul; // 手动功能，默认为关闭，实现自动功能
	uint16_t total_aount;
	uint8_t para_id;
	uint8_t speed_range; //速度档位
}MODE_PARA;

typedef struct{
	uint8_t id_default;
	uint8_t wifi_rfc;
	uint8_t TX_adress[5];
}ID_CODE;

typedef struct
{
	uint8_t MCU_uid[12];			/* the uid of mcu */
	uint8_t first_used; 		   /* first use the product*/
	uint8_t usart_number;			  /* usart select: usart 1==AA or usart 2==BB*/
	uint8_t  ISR_tag;                /*the flag of isr source */
	uint8_t  Reserved;                /*Reserved */
	uint16_t firmware; // firmware sizes
	uint16_t firmware_version;
	uint32_t firmware_checksum;
	//uint8_t Reserves[1000];
}IAP_INF_T;


typedef struct{
	uint16_t Amount; //张数
	uint16_t ramp_stop; //移动后的停止时间,毫秒单位
	uint16_t st_time_s; //快门时间速度
	uint16_t sys_stop; //快门拍摄后等待的时间
	uint16_t delay_time; //延时开始时间
	uint8_t speed_range; //速度档位
	uint8_t Manual; //手动快门
	uint8_t shutTimes; //快门次数
	
}MONOLAYER; //单层全景模式参数

typedef struct{
	uint8_t read_fisrt;
	ID_CODE wifi_id;
	MODE_PARA spe_para[MODE_ID_MAX];
	uint8_t mode_id; // 预计号
	//uint8_t duration_h; //video mode move time
	//uint8_t duration_m;
	//uint8_t duration_s;
	//uint8_t ramp;
	uint8_t speed; // 手动模式的速度档位
	MONOLAYER SLayer;
	
	uint16_t lens_folcal; // 镜头焦距
	uint8_t Roverlap; // 重叠率
	uint8_t GP_shut_mode; // 横竖拍
	uint16_t GP_exposure;
	uint8_t GP_speed;
	uint16_t GP_shut_t;
	uint16_t GP_sys_t;

}PARA_STRUCT;

typedef struct{
	
	uint8_t Ramp_id;
	uint8_t wf_tx_id;
	uint8_t wf_pk_id;
	uint8_t section_id;
	uint8_t VVsection_id;
	uint8_t go_back;
	uint16_t taltol_amount;
	uint8_t delay_time_s;
	uint8_t wid_time_s;
	uint8_t std_manul;
	uint8_t wid_manul;
	uint16_t ramp_buffer[20];
	uint16_t time_down;
	uint16_t time_down_count;
	uint16_t fix_stop_time;
	uint16_t sys_stop_time;
	uint16_t exposure;
	uint16_t interval; // 标准模式和广角模式下的缓停速度
	__IO uint16_t MS_count;
	
	uint32_t p_amount_back;
	uint32_t Dynamic_pulse;
	uint32_t VVDynamic_pulse;
	uint32_t slow_point;
	uint32_t rampp_time;
	
	uint8_t begin;
	uint8_t cur_begin; // 当前的参数组是否拍完的标志变量，值为0表示拍完，非0为正在拍当前的参数组
	uint8_t begin_back;
	uint16_t amount;
	uint32_t shut_time;
	uint32_t sys_stop;
	uint32_t interval_t;
	uint8_t dynamic;
	uint8_t p_upDown;
	uint8_t slw_time;
	uint8_t VRamp_id;
	uint16_t Vslow_point ;
	uint16_t Vramp_buffer[20];
	uint32_t VDynamic_pulse;
	
	uint8_t VVgo_back;
	uint8_t HHgo_back;
	uint8_t ddAB_set;
	uint8_t compensation;
	float remainder_p[PARA_MAX_ID]; //小数点脉冲数，用于计算脉冲数误差的补尝
	float diff;

	uint8_t move_time_s; // video mode move time
	uint8_t move_time_m;
	uint8_t move_time_h;
	uint8_t ramp_if; // 视频模式缓起停的标志
	uint32_t total_fast_t; //视频模式的总的ramp最快时间
	

	uint8_t vvAB_set; // 视频模式AB点设置状态
	uint8_t shut_time_b; //快门次数计数

	
	
}CONSTANT;

typedef struct{
	uint8_t DIR;
	__IO uint32_t pulse_count;
	__IO uint32_t DVpulse_count; //视频模式长度的脉冲计数
	__IO uint32_t DLLpulse_count;
	__IO uint32_t GPpulse_count;

}MOTOR_PARA;

typedef struct{
	uint8_t find_abpoint;
	__IO uint8_t speed_add; 
	//uint8_t VVfind_Apoint;
	uint8_t HHfind_Apoint;
	uint16_t compensation;
	uint16_t slow_pulse;
	
}FIND_P;

typedef struct{
	uint8_t VV_speed;
	uint8_t HH_speed;
	uint8_t VV_Time;
	uint8_t HH_Ttime;
	uint8_t VV_UpRoDown;
	uint8_t HH_UpRoDown;
	
}MANAUL_SLOW;

typedef struct{
	uint8_t VV_speed;
	uint8_t HH_speed;
	uint8_t VV_Time;
	__IO uint8_t HH_Ttime;
	uint8_t VV_UpRoDown;
	uint8_t HH_UpRoDown;
	
}STANDARD_SLOW;

typedef struct{
	uint8_t move_time_s;
	uint8_t move_time_m;
	uint8_t move_time_h;
	uint8_t ram_if;
	uint8_t dir;
	uint8_t check_dir;
	uint8_t p_upORdown;
	uint8_t dynamic_speed;
	uint8_t slow_time;
	uint8_t locus_set;
	uint32_t point_a; // 从其它模式回到视频模式的校准点
	uint32_t point_b;
	uint32_t point_pulse_a; //视频模式长度的AB 点
	uint32_t point_pulse_b;
	uint32_t pluses;
	uint16_t ram_buffer[RAM_ID];
	uint16_t ram_dist;
	uint32_t ram_time;
	uint16_t ram_id;
	uint32_t speed;
	uint8_t X_l_tag; //  滑轨是否有长度的标志位
	uint16_t Fast_time;
	uint32_t RampFast_time;
	uint32_t act_speed; // 滑轨的实际运行时间
	uint32_t com_pulse_a; // 补尝数，需要变数的脉冲点位
	uint32_t com_pulse_b; // 补尝数，需要变数的脉冲点位
	uint32_t com_tag; // 补尝数，需要变数的脉冲点位
	
}VIDEO_LOCUS;

typedef struct{
	uint8_t ram;
	uint8_t DIR;
	uint8_t VV_DIR; // z 轴方向
	uint8_t loop;
	uint16_t activate;
	uint8_t Pause;
	uint8_t VVPause;
	uint8_t locus_id;
	uint8_t locus_dy; // 动态ID
	uint8_t curent_id; //当前ID

	VIDEO_LOCUS locusH;
	VIDEO_LOCUS locusV;
	
	uint8_t tx_id;
	uint8_t rx_id;
	uint8_t HH_dynamic;
	uint8_t VV_dynamic;
	uint8_t shut_switch;
	
}VIDEO_PARA;

typedef struct{
	uint8_t begins;
	uint8_t dir;
	uint16_t speed;
	uint32_t or_point; // 源点位置
	uint32_t point_a;//延时模式长度的AB 点
	uint32_t point_b;
	uint32_t pulse;
	uint32_t move_pulse_count; // 每张照片移动的脉冲计数器
	uint32_t move_pulse; // 每张照片移动的脉冲数，移动脉冲数到达此数后停止移动
	float diff; //每张脉冲数的差分
	float remainder_p; //每张照片移动的脉冲数的余数，用于脉冲数的补尝计算
	uint16_t ram_dist;
	uint16_t ram_id;
	 uint8_t ramspeed;
	uint8_t ramTime;
	uint8_t Ram_UpRoDown;
	uint8_t check_dir;
}DELAY_LOCUS;

typedef struct{
	uint8_t m_start;
	uint8_t ab_set_if;
	uint8_t total_time_s;
	uint8_t total_time_m;
	uint8_t total_time_h;
	uint8_t DIR;
	
	uint16_t interval; // 缓停时间，即是移动停下来后暂停这个时间后开始拍
	uint16_t exposure;
	uint32_t amount; //张数
	uint32_t Max_amount; //张数
	uint32_t move_times;

	DELAY_LOCUS locuHH;
	DELAY_LOCUS locuVV;

	uint32_t all_times; // 每一张的总时间
	__IO uint16_t activate;
}DELAY_PARA;

typedef struct{
	// 合影模式参数
	uint16_t lens_folcal; // 镜头焦距
	uint8_t Roverlap; // 重叠率
	uint8_t GP_manul;
	uint8_t GP_dir;
	uint8_t GP_set_if; // 是否已设置AB点
	uint8_t check_dir; //找A/B点标志
	uint8_t amount;
	uint8_t GP_shut_mode; // 横竖拍

	uint16_t Gp_angle;
	uint32_t origin_pulse; // 从其它模式回到视频模式的校准点
	uint32_t point_pulse_a; //视频模式长度的AB 点
	uint32_t point_pulse_b;
	uint32_t lens_pulse;
	uint32_t Max_amount; //张数
	uint32_t move_pulse; // 每张照片移动的脉冲数，移动脉冲数到达此数后停止移动
	
}GROUP_PARA;

void Feed_IWDG(void);
void Device_init(void);
void adc_cout(void);

void adc_time_cout(void);
void motor_speed_change(uint8_t p_speed);
void delay_move_time_compara(void);
void if_write_flash(void);
void write_flash_active(void);
void Gpio_init(void);
void time_count(void);
void pulse_count(void);
void slow_spee_check(void);
void auto_return_check(void);
void time_2_init(void);
void A_d_c_int(void);
void timer3_pwm_init(void);
void motor_init(void);
void ab_to_a_point_stop(void);
void usart1_Receiver(uint8_t *Rbuffer);// from controller
void read_ADC_value(void);
void para_init(void);
void Manual_VV_slowStartOrStop(void);

//common.c
void Get_data_from_controller(uint8_t *fifos);
void slider_upORdown(uint8_t p_type);
void set_slow_time_data(void);
void battery_send_to_controller(uint16_t bats);
void set_abpoint_slow_stopORstart(MOTOR_TYPE motor_t);
uint8_t check_sum_add(uint8_t index, uint8_t *spt);
void Package_dataBufer(uint8_t txsize, uint8_t *buffer);
void Send_connect_data_to_controller(void);
void angle_accuracy_test(void);
void HHangle_accuracy_test(void);
uint32_t Absolute_value_calculation(uint32_t a, uint32_t b);
void Send_para_data_to_controller(uint8_t ipcode, uint8_t datas);
void Manual_HH_slowStartOrStop(void);
void Manul_SartOrStop(MOTOR_TYPE motor_t, uint8_t status);
void Manaul_speed_conversion(void);
void Angle_real_time_transmission(uint8_t tpes);
double PULSE_SUM(uint16_t anglel);
void para_init_set(void);
void ManulHH_mode_pulse_check(void);
void ManulVV_mode_pulse_check(void);
void Manu_mode_dir_check(MOTOR_TYPE motor_t);
void Manual_mode_dir_set(uint8_t ttemp);
void All_slow_check(void);

// Motor.c
void motor_slowly_startedORstop(void);
void Loop_Dispose(void);
void time_delay_ms(uint16_t counts);

// usart.c
void MX_USART1_UART_Init(void);
void Open_Send_Dma(void);
void usart11_Recdata_Handling(void);
void usart1_receiver(void);
void send_interval_count(void);
void Dma_SendIRQ_Dispose (void);
void Usart11_Interval_set(void);
void Usart_Data_Dispose(USART_NUM Usart);

void usart22_Recdata_Handling(void);
void Usart22_Open_Send_Dma(void);
void usart22_read_byte(void);
void Usart22_Dma_SendIRQ_Dispose (void);
void Usart22_Interval_set(void);
void MX_USART2_UART_Init(void);

void MX_USART33_UART_Init(void);
void Usar33_Dma_SendIRQ_Dispose (void);
void Usart33_Send_Handling(uint8_t TX_size, uint8_t *value_da);
void Launch_Cache(void);


void MultLocus_Auto_Return(uint8_t tests);
void Advance_VideoMlutLocus_SlowlySatrtStop(void);
void Video_MultLocus_slow_spee_check(void);
void TimeLapse_MultLocus_AutoReturn(void);
void TimeLapse_MultLocus_MoveStop(void);
void TimeLapse_MultLocus_MoveMain(void);
void Burn_code_value(void);
//void Enter_into_Pair(void);
void Pairing_time(void);
void BLE_Change_Name(void);
void BLE_Change_Name(void);


//MAIN.C
void SystemClock_Config(void);
//ADC.C
void check_ADC_init(void);
uint32_t return_battery(void);
//flash.c
void check_uid(void);
void write_bootTag(uint8_t tag, uint16_t sizes);
void String_Printf(USART_NUM Usart, uint8_t *sptf, uint8_t sizes);
//BLE.C
void Ble_init(void);
// GPIO.C
void Device_init(void);
void motor_reset(void);
void usb_voltage_tatus(void);
void usb_status_initerrupt_init(void);
void usb_protection(void);
void motorHH_ToUpdata_timer(void);
void Green_Led_Brightness(void);
void motorHH_start(void);
void motorHH_stop(void);
void motorHH_speed_set(uint32_t speedd);
void shutter_processing(uint8_t satuss);
void bluetooth_pair_led(void);
void Gpio_time_count(void);
void Red_led_tack(void);

//BOOTLOADER.C
void bootloader_action_if(uint8_t *spt);
void check_enter_boot_if(uint8_t *spt);
void enter_bootloader(uint8_t tag, uint16_t sizes);
void Enter_OTA_If(uint8_t *sspt);
uint8_t string_lengt(uint8_t *strings);
uint32_t return_checksum(void);
void Read_versions(uint8_t *sspt);

// low_power.c
void Time_Out_And_Enter_Stop_Mode(void)  ;
void active_time_out_countdown(void);
void set_active_time_out(uint32_t aaa);
void Enter_Sleep_Mode(void);
void ShutDown(void);
void RTC_AlarmConfig(void);
void MX_RTC_Init(void);
void RTC_disable(void);

// motorV.c
void motorVV_ToUpdata_timer(void);
void motorVV_speed_set(uint32_t speedd);
void motorVV_start(void);
void motorVV_stop(void);
void motorVV_direction_change(uint8_t dir);
void motorVV_pulse_count(void);

// motorH.c
void motorHH_ToUpdata_timer(void);
void motorHH_speed_set(uint32_t speedd);
void motorHH_start(void);
void motorHH_stop(void);
void motorHH_direction_change(uint8_t dir);
void motorHH_pulse_count(void);

// standard.c
void Standard_Get_data_from_controller(uint8_t *fifob);
void standard_mode_main(void);
void Vertical_slow_check(void);
void Horizontal_slow_check(void);
void Vertical_pulse_check(void);
void Horizontal_pulse_check(void);
void Vertical_pulse_goBack(void);
void standard_countdown_time_display(void);
void standard_mode_param_display(void);
void SDmotor_slowly_startedORstop(MOTOR_TYPE motor_t);
void standard_mode_start(void);
void Horizontal_start(void);
void Vertical_start(void);
void BLE_shut_stop(void);
void standard_mode_para_init(void);
void ble_shut_time_count(void);
void BLE_shut_start(void);
void Manul_Turn_the_shuter(void);
void Panorama_mode_compensation(void);
void Panorama_mode_find_Apoint(void);
void Panorama_find_end_point_checkStop(MOTOR_TYPE motor_t);
void Panorama_find_end_point_slowChcek(MOTOR_TYPE motor_t);

// wide_agnle.c
void wideAngle_Get_data_from_controller(uint8_t *fifob);

// specialty_mode.c
void Specialty_Get_data_from_controller(uint8_t *fifob);
void Specialty_Send_data(uint8_t m_id);
void Cancel_the_para_set_return(uint8_t indx);
void FindOrigin_over_to_Sart(void);
void SpVVRamp_Speed_Load(uint32_t speed);
void Speialty_Horizontal_pulse_check(void);
void Speialty_Vertical_pulse_check(void);
void SpVVertical_slow_check(void);
void SpHHorizontal_slow_check(void);
void specialty_mode_main(void);
void SpHHmotor_slowly_startedORstop(void);
void SpVVmotor_slowly_startedORstop(void);
void Spe_manul_Shuting(void);
void Specialty_mode_Start(void);
void Horizontal_pulse_goBack(void);
void specialty_mode_compensation(void);
void Specialty_modeID_sendt(void);
void sp_para_init(void);
void specialty_parameter_calculate(uint8_t idxs);
void spe_mode_clear(void);
void Specialty_mode_downcount_begin(void);
void Sync_data_for_controller(void);
void specialty_mode_para_clear(void);
uint8_t SpecialtyHH_Ramp_Speed_Load(uint32_t speed, uint8_t idx);
void Specialty_loop_check(void);
void Specialty_Key_start(void);
void Specialty_Send_reshot_to_controller(uint8_t types);
void Reshot_Clear(void);
void Reshot_move(uint8_t dir);
void Reshot_pulses_check_stop(void);
void SpResot_slow_check(void);
void Sp_para_start_init(void);
void shutter_time_counts(void);
void camera_shutter_shot(uint8_t typess);
void sp_Factory_default(void);
void SpHHorizontal_start(void);

// video_mode.c
void Video_pulse_check(void);
void Video_abPoint_set(uint8_t types);
//video.c
void MotorVV_Video_move_time_check(void);
void MotorHH_Video_move_time_check(void);
void Video_Fastest_time_check(void);
void Video_Get_data_from_controller(uint8_t *fofis);
void Video_move_times_limit(void);
void Video_find_Apoint(void);
void Video_find_Origin_checkStop(MOTOR_TYPE motor_t);
void Video_MotorVV_slowly_startedORstop(void);
void Video_MotorHH_slowly_startedORstop(void);
void Video_start(void);
void video_Dir_check(MOTOR_TYPE motor_t);
void video_motorVV_pulse_check(void);
void video_motorHH_pulse_check(void);
void Video_slow_pulse_check(MOTOR_TYPE motor_t);
void Video_compensation_change_speed(MOTOR_TYPE motor_tt);
void set_abpoint_start_setting(MOTOR_TYPE motor_t);
uint32_t Times_convert_to_speed(uint32_t times, uint32_t pulse_p);
void Video_mode_para_clear(void);
void Video_FindABpoint_End(void);
void Video_FindABpoint_pluse_check(MOTOR_TYPE motor_tt);
uint8_t Video_MotorVV_Start_check_diretion(void);
uint8_t Video_MotorHH_Start_check_diretion(void);
void Video_FindABpoint_slow_check(MOTOR_TYPE motor_t);
void limit_angle_360(void);
void vidoe_mode_para_clear(void);
void Video_find_Origin_slow_check(MOTOR_TYPE motor_t);
void Video_loop_check(void);
void Video_Find_ABpoint_notify(uint8_t dis_type);

// timelapse.c
void delay_mode_main(void);
void delay_mode_countdwon_display(void);
void delay_mode_time_countdwon(void);
void Delay_get_data_from_controller(uint8_t *fifos);
void Delay_set_abPoint(uint8_t typess);
void Delay_mode_find_Apoint(void);
void Delay_mode_Dir_check(MOTOR_TYPE motor_t);
void Delay_find_Origin_checkStop(MOTOR_TYPE motor_t);
void Delay_motorVV_move_time_compara(void);
void Delay_motorHH_move_time_compara(void);
void delay_move_para_get(void);
void Delay_total_time_calculate(void);
void Delay_mode_para_clear(void);
void DelayMode_slowly_startedORstop(MOTOR_TYPE motor_t);
void DelayMode_FindABpoint_End(void);
void Delay_FindABpoint_pluse_check(MOTOR_TYPE motor_tt);
void Delay_slow_pulse_check(MOTOR_TYPE motor_t);
uint8_t Delay_MotorVV_Start_check_diretion(void);
uint8_t Delay_MotorHH_Start_check_diretion(void);
void DelayMode_ram_load(MOTOR_TYPE motor_tt);
void delay_mode_para_clear(void);
void Delay_find_Origin_slow_check(MOTOR_TYPE motor_t);
void Delay_FindABpoint_slow_check(MOTOR_TYPE motor_t);
void Timelapse_loop_check(void);

// keyscan.c
void scan_cycle_count(void);
void set_press_time(void);
void keyscan(void);

//GROUP.C
void Group_set_abPoint(uint8_t typess);
void Group_get_data_from_controller(uint8_t *fifog);
uint32_t Calculate_current_angel(void);
void Group_shots_event(void);
void Group_loop_check(void);
void Group_main(void);
void Group_mode_start(void);
void Group_mode_para_init(void);
void Group_mode_find_Apoint(void);
void Group_mode_Dir_check(void);
void Group_Ramp_Speed_Load(uint8_t speeds);
void Group_manul_Shuting(void);
uint8_t Group_mode_Start_check_diretion(void);
void Group_move_speed_calculate(void);
void Group_mode_countdwon_display(void);

extern PARA_STRUCT glob_para;
extern float battery_back;
extern uint8_t charge_tag;
extern uint8_t rtc_wackup; //
extern uint16_t low_voltage;
extern uint8_t mode_backup;
extern __IO uint32_t boot_time;
extern uint8_t p_upORdown;

extern uint16_t dynamic_speed;
extern uint16_t slow_time;
extern uint8_t move_begin_backup;
extern uint8_t move_begin;
extern uint32_t p_move_time ;
extern uint32_t p_amount;
extern uint8_t m_start;
extern uint8_t Battery_percentage;
extern uint8_t sp_start;
extern CONSTANT con_b;
extern MOTOR_PARA motorHH_p;
extern MOTOR_PARA motorVV_p;
extern uint32_t standard_H_pulse[STAND_IDMAX];// 从仰角70度开始拍
extern uint32_t standard_V_pulse[STAND_IDMAX];// 从仰角70度开始拍
extern uint8_t standard_H_frames[STAND_IDMAX];// 从仰角70度开始拍
extern uint8_t run_id;
extern FIND_P find_pata;
extern FIND_P find_pataVV;
extern VIDEO_PARA video_p;
extern DELAY_PARA delay_p;
extern STANDARD_SLOW standard_p;
extern GROUP_PARA Group_p;
extern MANAUL_SLOW Manul_p;

extern uint8_t Sync_sendt;
extern uint8_t Alon_sendt;
extern double Angle_basic;
extern uint32_t ORIGIN_POINT ;		
extern uint32_t A_POINT_END;
extern uint32_t B_POINT_END;	
extern uint32_t M_POINT_END;		
extern uint32_t POINT_360_END;		

#endif
