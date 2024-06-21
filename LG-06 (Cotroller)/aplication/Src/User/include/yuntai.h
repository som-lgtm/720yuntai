#ifndef __YUNTAI_H
#define __YUNTAI_H

#include "stm32f0xx.h"

//#define ONE_ANGLE_PULSE	23
#define SUB_P_BASI_0	8
#define SUB_P_BASI_1	19
#define SUB_P_BASI_2	30
#define YUNTAI_MOVE_SPEED	10
#define YUNTAI_ANGLE_SUB_VALUE	80

void yuntai_page_selset_main(void);
void yuntai_mode_display(void);
void yuntai_page_cursor_upordown(uint8_t dir);
void yuntai_mode_cursor_upordown(uint8_t dir);
void yuntai_mode_page_ok(void);
void yutai_speed_adjust(uint8_t dir);
void yuntai_dir_adjust(uint8_t dir);
void yuntai_video_ok(void);
void yuntai_video_mode_display(void);
void Yun_Moving_All_Dis(void);
uint16_t  para_count(uint8_t dir, uint16_t data, uint16_t p_range);
float move_time_cout(uint8_t dir, float data, float p_range);
void Yun_page_return(void);
void Yun_para_init(void);
void Yun_Panorama_Main(void);
void yuntai_move_mode_cursor_upordown(uint8_t dir);
void yu_move_mode_para_adjust(uint8_t dir);
void Yun_move_mode_OK(void);
void Yun_move_mode_Main(void);
void yuntai_run(void);
void press_key_continue_if(uint8_t dir);
void yuntai_main_display(void);
void yun_move_mode_dir_adjust(uint8_t dir);
void yuntai_dir_display(uint8_t p_dir, uint8_t y);/////////////////////////////////////////////////////
void yuntai_video_para_adjust(uint8_t dir);
void angle_cout_dir_bakeup(void);
void angle_cout_dir_change(void);
void angle_cout_function(void);
void yuntai_auto_reverse(void);
void yun_move_speed_auto_add(void);
void yun_mode_pulse_compara(void);
void speed_bakeup_get(void);
void yuntai_set_ab_if_adjust(uint8_t dir);
void yuntai_set_ab_if_OK(void);
void yuntai_set_ab_if_display(void);
void yuntai_ab_page_display(uint8_t sel);
void yuntai_set_ab_move(uint8_t dir);
void yuntai_set_ab_OK(void);
void yun_ab_compara_functiion(void);
void yuntai_ab_point_stop(void);

#endif
