#ifndef __ORBITAL_H
#define __ORBITAL_H

#include "stm32f030x8.h"

// MANUAL MODE CURSOR
#define VINDEX_ZERO		0
#define VINDEX_ONE		1
#define VINDEX_TWO		2
#define VINDEX_THREE	3
#define VINDEX_FOUR		4

// AOTU MODE CURSOR
#define VAINDEX_ZERO	0
#define VAINDEX_ONE		1
#define VAINDEX_TWO		2
#define VAINDEX_THREE	3
#define VAINDEX_FOUR	4
#define VAINDEX_FIVE	5

// DELAY MODE CURSOR
#define TINDEX_ZERO		0
#define TINDEX_ONE		1
#define TINDEX_TWO		2
#define TINDEX_THREE	3
#define TINDEX_FOUR		4
#define TINDEX_FIVE		5
#define TINDEX_SIX		6
#define TINDEX_SEVEN	7

// DELAY DIS  MODE CURSOR
#define TDINDEX_ZERO	0
#define TDINDEX_ONE		1
#define TDINDEX_TWO		2
#define TDINDEX_THREE	3

// CONFI  MODE CURSOR
#define COINDEX_ZERO	0
#define COINDEX_ONE		1
#define COINDEX_TWO		2
#define COINDEX_THREE	3
// SET POINT  MODE CURSOR
#define SEINDEX_ZERO	0
#define SEINDEX_ONE		1
#define SEINDEX_TWO		2
#define SEINDEX_THREE	3
#define SEINDEX_FOUR	4
#define SEINDEX_FIVE	5



#define SPEED_SLOWN_DIF_VALUE		2000
#define AB_RETURN_STOP_TIME		100
#define PIX_PULSE	16 // 16 PULSE TO MOVE ONE HPOTO
#define SLOW_THRESHOLD	50
#define THRESHOLD(X)	speed_calculat(X)
//#define MIIX_THRESHOLD(X)	speed_calculat(X)
#define SLOW_NEED_TIME	4000 // 2000
#define FW_VERSION	101

void video_page_OK(void);
void delay_mode_main(void);
void delay_page_return(void);
void delay_dir_change(uint8_t dir);
void orbital_para_init(void);
void delay_param_compara(void);
void Fast_time_display(void);
void press_key_continue_if(uint8_t dir);
void ab_point_change_dir(void);
uint32_t move_time_calculate(void);
void speed_display(uint8_t cur, uint8_t speed);
float  para_count(uint8_t dir, float data, uint16_t max, uint8_t mix);
void Vertical_Scroll(void);
void duration_time_dis(uint8_t types, uint8_t cur);
void move_to_disp(uint8_t cur);
void versions_display(void);
void completed_disp(uint8_t per);
void countdown_display(uint8_t x, uint8_t param, uint8_t cur);
void pix_amt_compara_dis(uint8_t cur);
void cursor_shift(uint8_t dir);
void param_adjust(uint8_t dir);
void UpOrDown_adjust_para(uint8_t dir);
void status_display(uint8_t cur);
void video_dir_display(uint8_t cur);/////////////////////////////////////////////////////
void durationORdelay_time_dis(uint16_t param, uint8_t cur);
void Fast_time_dis(uint8_t cur);
void fps_amt_display(uint8_t cur);
void language_display(uint8_t p_langu);
void factory_set_display(uint8_t cur);
void inverse_get_value(uint8_t datat);
void cursor_glint(void);
void parameter_init(void);
void set_addend_data(uint16_t pvalue, float fvalue);
void minutes_time_dis(uint16_t param, uint8_t cur);
void hours_time_dis(uint32_t param, uint8_t cur);
void secondes_time_dis(uint16_t param, uint8_t cur);
void param_display(uint8_t cur);
void single_cursor_dis(uint8_t cur);
void Task_Percentage_Dis(uint8_t datat);
void total_time_display(void);
void Brightness_Dis(void);
void move_speed_calculat(void);
void delay_time_over(void);
void interval_time_error_adjust(void);
void fisrt_time_begin(void);
void move_mode_dis(uint8_t cur);
void shoot_mode_adjust(void);
void locus_disp(void);
void stop_shoot_dis(uint8_t cur);
void pix_amt_display(uint8_t cur);
void play_back_dis(uint8_t cur);
void adv_time_OK(void);
void get_data_form_A650(void);
void delay_move_page_dis(void);
void delay_mode_tatol_time_dis(void);
void delay_dis_interface(void);
void tatol_hours_time_dis(uint32_t param, uint8_t cur);
void Brightness_dis(uint8_t cur);
void sleep_time_dis(uint8_t cur);
void shutKey_start(void);
void connection_set(void);
uint8_t remotoe_cursor_dispose(uint8_t tage);
void manulORauto_dis(uint8_t dat, uint8_t cur);
uint16_t data_count(uint16_t data, uint8_t dir, uint16_t max, uint8_t mix);
void slow_start_stop_dis(uint8_t cur);
void Time_DownCount(uint8_t cur);
void Manual_page_OK(void);
void Speed_display(uint8_t cur);
void Video_mode_cursor_shift(uint8_t dir);
void Delay_amount_adjust(uint8_t dir);
void Delay_mode_cursor_shift(uint8_t dir);
void Delay_pix_amt_display(uint8_t cur, uint8_t numb);
void angle_display(uint8_t cur, uint16_t angles);
void angle_cear(void);
void integer_display(uint16_t datat, uint8_t cur);	
void decimal_point_time_dis(float datat, uint8_t cur);
void integer_position_display(uint16_t datat, uint8_t cur, uint8_t curxx);
void Group_pix_amt_compara_dis(uint8_t max_p, uint8_t cur);
void Group_page_OK(void);

extern uint16_t p_amount;
extern __IO uint16_t get_init_time ;
extern __IO uint16_t led_time;
extern uint32_t expouse_time;
extern __IO uint16_t connection_flag;
extern __IO uint16_t continues ;
extern uint16_t low_voltage;

#endif

