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

#define A_TO_B		0 // 脉冲数递增//府仰轴 向上转也是顺时针 // 水平轴顺时针
#define B_TO_A		0xff // 脉冲数递减//府仰轴向下转//水平轴逆时针

uint32_t calculate_fast_speed_and_ramp_time(uint32_t pulses, uint32_t speeds);
uint32_t speed_calculat(uint8_t p_speed);
void Ramp_Speed_Load(uint32_t speed);

#endif

