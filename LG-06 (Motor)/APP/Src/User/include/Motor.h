#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f0xx.h"
#include "uart.h"

#define STANDAR_SLOW_THRESHOLD		203

#define SLOW_THRESHOLD		120
#define SLOW_BASIC_TIME00		80
#define SLOW_BASIC_TIME22		30
#define SLOW_BASIC_TIME33		30
#define SLOW_BASIC_TIME44		60
#define BASE_SPEED	5 //15

#define A_TO_B		0 // ����������//������ ����תҲ��˳ʱ�� // ˮƽ��˳ʱ��
#define B_TO_A		0xff // �������ݼ�//����������ת//ˮƽ����ʱ��

uint32_t calculate_fast_speed_and_ramp_time(uint32_t pulses, uint32_t speeds);
uint32_t speed_calculat(uint8_t p_speed);
void Ramp_Speed_Load(uint32_t speed);

#endif

