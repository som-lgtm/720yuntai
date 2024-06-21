
#include "common.h"
#include "motor.h"

VIDEO_PARA VVvideo_p ={0};
VIDEO_PARA HHvideo_p ={0};

uint32_t Time_is_converted_to_speed(uint32_t move_timess, uint32_t pulse_p)
{
	uint32_t adv_speed_back =0;
	uint32_t time_all =0;
	double basic_time=0; //it is time of each pulse
	uint16_t tag = 0;
	double temp =0;
	
	time_all = times;
		
	basic_time = (double)time_all / (double)pulse_p;
	temp = (double)48000 * basic_time / (double)501 -1;
	
	tag = temp*10;
	tag %=10;
	if( tag)
	{
		adv_speed_back = (temp+1);
	}
	else
	{
		adv_speed_back = temp;
	}

	return adv_speed_back;
}

// 最快运行时间的计算
void Video_move_time_init(void)
{
	uint32_t times = 0;

	if(VVvideo_p.lenght_pulse)
	{
		times = calculate_fast_speed_and_ramp_time(VVvideo_p.lenght_pulse, speed_calculat(STANDARD_SPEED));
	}

	VVvideo_p.fast_times = times;

	if(HHvideo_p.lenght_pulse)
	{
		times = calculate_fast_speed_and_ramp_time(HHvideo_p.lenght_pulse, speed_calculat(STANDARD_SPEED));
	}
	
	HHvideo_p.fast_times = times;

	if(HHvideo_p.fast_times < VVvideo_p.fast_times) // 对比两电机最快速度，并取慢的那个
	{
		con_b.total_fast_t = VVvideo_p.fast_times;
	}
	else
	{
		con_b.total_fast_t = HHvideo_p.fast_times;
	}
	
	if((con_b.total_fast_t % 1000) >200)
	{
		times = con_b.total_fast_t / 1000 + 1;
	}
	else
	{
		times = con_b.total_fast_t / 1000;
	}
	
	con_b.move_time_h = times / 3600;
	con_b.move_time_m = (times - con_b.move_time_m * 3600) / 60;
	con_b.move_time_s = times - con_b.move_time_m * 3600 - con_b.move_time_m * 60;

	Send_connect_data_to_controller();
	
}

void Video_Get_data_from_controller(uint8_t *fofis)
{
	if(fofis[4] == 2) // move time setting
	{
		con_b.move_time_s = fofis[5];
		con_b.move_time_m = fofis[6];
		con_b.move_time_h = fofis[7];
		
		Video_speed_calculate();
	}
	else if(fofis[4] == 5)
	{
		motorHH_p.pulse_count = ORIGIN_POINT;
		motorHH_p.DVpulse_count = ORIGIN_POINT;

		motorHH_p.DIR = A_TO_B;
		motorHH_direction_change(motorHH_p.DIR);
		motorHH_speed_set(20);
		motorHH_start();
	}
}

void Video_pulse_check(void)
{
	if(motorHH_p.DVpulse_count == B_POINT_END)
	{
		motorHH_stop();
		Send_connect_data_to_controller();
	}
}

void Video_abPoint_set(uint8_t types)
{
	uint32_t V_pulse = 0;
	uint32_t H_pulse = 0;
	
	con_b.vvAB_set = types;
	
	if(con_b.vvAB_set == 1)
	{
		motorVV_p.DVpulse_count = ORIGIN_POINT;
		VVvideo_p.point_a = motorVV_p.pulse_count;
		VVvideo_p.point_pulse_a = ORIGIN_POINT;
		
		motorHH_p.DVpulse_count = ORIGIN_POINT;
		HHvideo_p.point_a = motorHH_p.pulse_count;
		HHvideo_p.point_pulse_a = ORIGIN_POINT;
	}
	else if(con_b.vvAB_set == 2)
	{
		VVvideo_p.point_b = motorVV_p.pulse_count;
		VVvideo_p.point_pulse_b = motorVV_p.DVpulse_count;
		
		HHvideo_p.point_b = motorHH_p.pulse_count;
		HHvideo_p.point_pulse_b = motorHH_p.DVpulse_count;

		Send_para_data_to_controller(1, 1);
		V_pulse = Absolute_value_calculation(VVvideo_p.point_pulse_a, VVvideo_p.point_pulse_b);
		H_pulse = Absolute_value_calculation(HHvideo_p.point_pulse_a, HHvideo_p.point_pulse_b);
		
		if(H_pulse < (uint32_t)PULSE_SUM(10) && V_pulse < (uint32_t)PULSE_SUM(10)) // 少于10度不可以用
		{
			con_b.vvAB_set = 0; // ab 点设置失败
			VVvideo_p.lenght_pulse = 0;
			HHvideo_p.lenght_pulse = 0;
			return;
		}
		else
		{
			con_b.vvAB_set = 3;
			if(V_pulse < (uint32_t)PULSE_SUM(10))
			{
				VVvideo_p.lenght_pulse = 0;
			}
			else
			{
				VVvideo_p.lenght_pulse = V_pulse;
			}

			if(H_pulse < (uint32_t)PULSE_SUM(10))
			{
				HHvideo_p.lenght_pulse  = 0;
			}
			else
			{
				HHvideo_p.lenght_pulse = H_pulse;
			}
			
			Video_speed_calculate();
		}
	}
}

