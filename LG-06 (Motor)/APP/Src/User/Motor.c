
#include "Motor.h"
#include "common.h"
#include "gpio.h"
#include "flash.h"


void motor_init(void)
{
	para_read_from_flash();
	para_init_set();
	con_b.exposure = 1000;
	con_b.interval = 1000;
	motorHH_p.DVpulse_count = ORIGIN_POINT;
	motorVV_p.DVpulse_count = ORIGIN_POINT;
	sp_para_init();
	glob_para.speed = 3;
	Group_mode_para_init();
}

// 水平轴每张相片移动的RAMP
// 需要缓起缓停的区间速度缓冲区
void Ramp_Speed_Load(uint32_t speed)
{
	uint8_t i;
	uint16_t timeb = 0;
	
	con_b.slow_point = 0;
	con_b.ramp_buffer[0] = speed;
	for(i=1; i<14; i++)
	{
		timeb = speed + (i + 1) * i / 2;
		if(timeb > STANDAR_SLOW_THRESHOLD)
		{
			con_b.Ramp_id = i-1;
			break;
		}
		
		con_b.ramp_buffer[i] = timeb;
		con_b.slow_point += SLOW_BASIC_TIME44 / ((float)(con_b.ramp_buffer[i] + 1) * (float)501 / (float)48000);
		con_b.Ramp_id = i;
	}
	con_b.rampp_time = SLOW_BASIC_TIME44 * (con_b.Ramp_id-1);
	//con_b.slow_point +=160;
}

// 计时最快时间的快大ramp所需要的时间
uint32_t calculate_fast_speed_and_ramp_time(uint32_t pulses, uint32_t speeds)
{
	uint32_t ramp =0;
//	uint8_t temp = 0;
	
	if(pulses == 0)return 0;
	Ramp_Speed_Load(speeds);
	ramp = (pulses - con_b.slow_point*2) * (float)(speeds + 1) * (float)501 / (float)48000;
	ramp += con_b.rampp_time*2;
	
	return ramp;
}


uint32_t speed_calculat(uint8_t p_speed)
{
	uint32_t temp=0;

	temp = BASE_SPEED + (1 + (100 - p_speed)) * (100 - p_speed) / 2;
//	temp = speed_shaw[p_speed];

	return temp;
}

