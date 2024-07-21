

#include "common.h"
#include "motor.h"
#include "string.h"

VIDEO_PARA video_p = {0};
uint16_t Fastest_time=0;//三轴总的最快时间，不带RAMP
uint16_t RampFastest_time=0; //三轴总的最快时间，带RAMP
uint8_t limit_360_tag = 0;

void vidoe_mode_para_clear(void)
{
	con_b.vvAB_set = 0; // ab 点设置失败
	video_p.locusV.pluses = 0;
	video_p.locusH.pluses = 0;
}

void Video_Get_data_from_controller(uint8_t *fifo)
{
	//uint8_t index = 0;
	uint8_t temphh=0;
//	uint8_t tempvv=0;
	
	 if(fifo[4] == 0x01) // 每段轨迹的运行时长	
	{
		//if(motorV.apPointCheck || motorH.apPointCheck)return;
		if(video_p.Pause)return;
		con_b.move_time_s = fifo[5];
		con_b.move_time_m = fifo[6];
		con_b.move_time_h = fifo[7];
		//String_Printf(USART_2,"aa",2);
		video_p.locusV.move_time_h = fifo[5];
		video_p.locusV.move_time_m = fifo[6];
		video_p.locusV.move_time_s = fifo[7];
		
		video_p.locusH.move_time_h = fifo[5];
		video_p.locusH.move_time_m = fifo[6];
		video_p.locusH.move_time_s = fifo[7];
		Video_move_times_limit();
		MotorVV_Video_move_time_check();
		MotorHH_Video_move_time_check();
	}
	else if(fifo[4] == 0x07) // loop
	{
		video_p.loop = fifo[5];
	}
	else if(fifo[4] == 0x05) // pause  or continue
	{
		if(/*find_pataVV.HHfind_Apoint || */find_pata.HHfind_Apoint)
		{
			Send_connect_data_to_controller();
			return;
		}
		video_p.Pause = fifo[5];
		video_p.VVPause = fifo[5];
		if(video_p.Pause)
		{
			temphh = Video_MotorHH_Start_check_diretion();
			//tempvv = Video_MotorVV_Start_check_diretion();
			if(temphh==0)
			{
				//MotorVV_Video_move_time_check();
				MotorHH_Video_move_time_check();
				video_Dir_check(MOTOR_HORITAL);
				video_Dir_check(MOTOR_Vertical);

				if(limit_360_tag)
				{
					if(video_p.locusH.dir == A_TO_B)motorHH_p.DVpulse_count = video_p.locusH.point_pulse_a;
					else if(video_p.locusH.dir == B_TO_A)motorHH_p.DVpulse_count = video_p.locusH.point_pulse_b;
				}
				Video_start();
			}
		}
		else
		{
			Video_start();
		}
	}
	else if(fifo[4] == 0x06) // dir
	{
		if(video_p.Pause)return;
		video_p.locusH.dir = fifo[5];
		video_p.locusV.dir = fifo[5];
		video_Dir_check(MOTOR_HORITAL);
		video_Dir_check(MOTOR_Vertical);
	}
}

void Video_speed_calculate(void)
{
	//Video_move_time_init();
	Video_Fastest_time_check();
	MotorVV_Video_move_time_check();
	MotorHH_Video_move_time_check();

	/*if(VVvideo_p.ramp_if && HHvideo_p.ramp_if==0) // 处理H 轴的速度与V 轴一至
	{
	
	}
	else if(VVvideo_p.ramp_if==0 && HHvideo_p.ramp_if) // 处理V  轴的速度与H 轴一至
	{
		
	}*/
}


void Video_abPoint_set(uint8_t types)
{
	uint32_t V_pulse = 0;
	uint32_t H_pulse = 0;
	
	con_b.vvAB_set = types;
	
	if(con_b.vvAB_set == 1)
	{
		motorVV_p.DVpulse_count = ORIGIN_POINT;
		video_p.locusV.point_a = motorVV_p.pulse_count;
		video_p.locusV.point_pulse_a = ORIGIN_POINT;
		
		motorHH_p.DVpulse_count = ORIGIN_POINT;
		video_p.locusH.point_a = motorHH_p.pulse_count;
		video_p.locusH.point_pulse_a = ORIGIN_POINT;
	}
	else if(con_b.vvAB_set == 2)
	{
		video_p.locusV.point_b = motorVV_p.pulse_count;
		video_p.locusV.point_pulse_b = motorVV_p.DVpulse_count;
		
		video_p.locusH.point_b = motorHH_p.pulse_count;
		video_p.locusH.point_pulse_b = motorHH_p.DVpulse_count;

		V_pulse = Absolute_value_calculation(video_p.locusV.point_pulse_a, video_p.locusV.point_pulse_b);
		H_pulse = Absolute_value_calculation(video_p.locusH.point_pulse_a, video_p.locusH.point_pulse_b);
		
		if(H_pulse < (uint32_t)PULSE_SUM(1) && V_pulse < (uint32_t)PULSE_SUM(1)) // 少于10度不可以用
		{
			con_b.vvAB_set = 0; // ab 点设置失败
			video_p.locusV.pluses = 0;
			video_p.locusH.pluses = 0;
			//return;
		}
		else
		{
			con_b.vvAB_set = 3;
			video_p.DIR = B_TO_A;
			video_p.locusH.dir = video_p.DIR;
			video_p.locusV.dir = video_p.DIR;
			if(V_pulse < (uint32_t)PULSE_SUM(1))
			{
				video_p.locusV.pluses = 0;
			}
			else
			{
				video_p.locusV.pluses = V_pulse;
			}

			if(H_pulse < (uint32_t)PULSE_SUM(1))
			{
				video_p.locusH.pluses  = 0;
			}
			else
			{
				video_p.locusH.pluses = H_pulse;
			}
			
		/*	if(video_p.locusH.point_pulse_b < video_p.locusH.point_pulse_a)
			{
				H_pulse = video_p.locusH.point_pulse_a;
				video_p.locusH.point_pulse_a = video_p.locusH.point_pulse_b;
				video_p.locusH.point_pulse_b = H_pulse;
			}
			if(video_p.locusV.point_pulse_b < video_p.locusV.point_pulse_a)
			{
				H_pulse = video_p.locusV.point_pulse_a;
				video_p.locusV.point_pulse_a = video_p.locusV.point_pulse_b;
				video_p.locusV.point_pulse_b = H_pulse;
			}*/
			//Send_para_data_to_controller(1, 1);
			Video_speed_calculate();
			video_Dir_check(MOTOR_HORITAL);
			video_Dir_check(MOTOR_Vertical);
			if((video_p.locusH.pluses >= POINT_360_END))
			{
				limit_360_tag = 1;
			}
			else
			{
				limit_360_tag = 0;
			}
		}
		Send_para_data_to_controller(1, 1);
		//Send_para_data_to_controller(8, 1);
	}
}

void Video_ram_load(uint8_t speedV, MOTOR_TYPE motor_tt)
{
	uint8_t i;
	uint16_t tampV=0;
	uint16_t tampH=0;
	uint8_t ramp_idd = 15;

	if(motor_tt) // motor H  水平轴
	{
		video_p.locusH.ram_dist = 0;
		video_p.locusH.ram_buffer[0]=speedV;
		
		for(i=1; i<ramp_idd;i++)
		{
			tampH =  speedV + i*(i+1)/2;
			video_p.locusH.ram_buffer[i] = tampH;
			video_p.locusH.ram_dist += (float)SLOW_BASIC_TIME11 / ((float)(tampH + 1) * 501.0 / 48000.0);
		}
		
		video_p.locusH.ram_time = SLOW_BASIC_TIME11 * (ramp_idd-1) * 2;
		
		video_p.locusH.ram_id = ramp_idd-1;
	}
	else
	{
		video_p.locusV.ram_dist = 0;
		video_p.locusV.ram_buffer[0] = speedV;
		
		for(i=1; i<ramp_idd;i++)
		{
			tampV =  speedV + i*(i+1)/2;
			video_p.locusV.ram_buffer[i] = tampV;
			video_p.locusV.ram_dist += (float)SLOW_BASIC_TIME11 / ((float)(tampV + 1) * 501.0 / 48000.0);
		}
		video_p.locusV.ram_time = SLOW_BASIC_TIME11 * (ramp_idd-1) * 2;
		video_p.locusV.ram_id = ramp_idd-1;
	}


}

// 将运行时间换算为定时器的预分频数，即是电机的速度
uint32_t Times_convert_to_speed(uint32_t times, uint32_t pulse_p)
{
	uint32_t adv_speed_back =0;
	uint32_t time_all =0;
	double basic_time=0; //it is time of each pulse
	uint16_t tag = 0;
	double temp =0;
	
	time_all = times ;
		
	basic_time = (double)time_all / (double)pulse_p;
	temp = (double)48000 * basic_time / (double)501 -1;
	
	tag = temp*10;
	tag %=10;
	//if( tag >=5)
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

// 计时最快时间的快大ramp所需要的时间
uint32_t calculate_fast_speed(uint32_t speeds, MOTOR_TYPE motor_t)
{
	uint32_t ramp =0;
//	uint8_t temp = 0;
	uint16_t r_dist=0;
	uint32_t p_pulse=0;
	uint32_t r_time = 0;
	
	
	Video_ram_load(speeds, motor_t);
	//String_Printf(USART_2,"BB",2);
	//String_Printf(USART_2,(uint8_t * )&video_p.locusH.ram_dist,2);
	if(motor_t == MOTOR_HORITAL) // motor H  水平轴
	{
		if(video_p.locusH.pluses == 0)return 0;
		if(video_p.locusH.pluses < (video_p.locusH.ram_dist * 2))
		{
			return 1000;
		}
		r_dist = video_p.locusH.ram_dist;
		p_pulse = video_p.locusH.pluses;
		r_time = video_p.locusH.ram_time;
	}
	else if(motor_t == MOTOR_Vertical)
	{
		if(video_p.locusV.pluses == 0)return 0;
		if(video_p.locusV.pluses < (video_p.locusV.ram_dist * 2))
		{
			return 1000;
		}
		r_dist = video_p.locusV.ram_dist;
		p_pulse = video_p.locusV.pluses;
		r_time = video_p.locusV.ram_time;
	}
	
	ramp = (p_pulse - r_dist*2) * ((float)(speeds + 1) * (float)501 / (float)48000);
	ramp += r_time ;
	
	return ramp;
}

// 检测设置的运行时间是否超出最快时间
void Video_move_times_limit(void)
{
	uint32_t times = 0;
	
	times = (con_b.move_time_s + con_b.move_time_m*60 + con_b.move_time_h*3600)*1000;
	if((times) < RampFastest_time)
	{
		times = RampFastest_time / 1000;
		con_b.move_time_h = times / 3600;
		con_b.move_time_m = (times - con_b.move_time_h * 3600) / 60;
		con_b.move_time_s = times - con_b.move_time_h * 3600 - con_b.move_time_m * 60;
		
		Send_connect_data_to_controller();
	}

}

// 设置AB点时计算出水平H轴与V轴的最快极限速试，即最快运行时间
void Video_Fastest_time_check(void)
{
//	uint32_t time_mH=0;
	uint32_t HFast_t=0;
	uint32_t VFast_t=0;
	uint32_t tatol_fast = 0;
	//uint8_t send_t=0;
//	float time_basic=0;
/*	
	time_mH = video_p.locusH[video_p.locus_id].move_time_s + video_p.locusH[video_p.locus_id].move_time_m * 60 + \
					video_p.locusH[video_p.locus_id].move_time_h*3600;
	time_mH *= 1000; //毫秒级
*/
	// 只要APP开启缓起停，云台不管速度多少都要计算RAM，启动时再跟随滑轨轴决定是否缓起停
	HFast_t = calculate_fast_speed(speed_calculat(100), MOTOR_HORITAL); //毫秒级
	VFast_t = calculate_fast_speed(speed_calculat(100), MOTOR_Vertical); //毫秒级

	video_p.locusH.RampFast_time = HFast_t;
	video_p.locusV.RampFast_time = VFast_t;
	if(HFast_t > VFast_t) // 谁的最快速度慢就用谁的速度来做计算
	{
		tatol_fast = HFast_t;
	}
	else
	{
		tatol_fast = VFast_t;
	}
	
	tatol_fast = (tatol_fast / 1000 + 2);// 太快的话，两轴不同步
	RampFastest_time = tatol_fast * 1000; 
	
	con_b.move_time_h = tatol_fast / 3600;
	con_b.move_time_m = (tatol_fast - con_b.move_time_h * 3600) / 60;
	con_b.move_time_s = tatol_fast - con_b.move_time_h * 3600 - con_b.move_time_m * 60;

	Send_connect_data_to_controller();
//	Usart11_load_data((uint8_t *)&HFast_t,4);
//	Usart11_load_data((uint8_t *)&VFast_t,4);
//Usart11_load_data((uint8_t *)&Fastest_time[dix],2);
//Usart11_load_data((uint8_t *)&RampFastest_time[dix],2);
//Usart11_load_data((uint8_t *)&video_p.locusH[dix].Fast_time,4);
//Usart11_load_data((uint8_t *)&video_p.locusV[dix].Fast_time,4);

}

//APP设置的运行时长与实际计算出来的运行时长偏差太大时， 进行计算变速补尝
void MotorVV_Video_compensation_check(uint16_t speeds)
{
	double speed_slow = 0.0;
	double speed_fast = 0.0;
	uint32_t com_pulse = 0;
	uint32_t times = 0;
	uint32_t total_p = 0;
//	uint8_t psd_bsic = 0;

/*	if(video_p.locusV[iid].ram_if)
	{
		speeds +=1;
	}*/
	// 公式x * s1 + (p-x)*s2 = t; 即x = (t - p*s2) / (s1 - s2)；X是对应S1所需的脉冲数；
	//S1是慢速的预分频数算出来的单脉冲时间；P是已知的总脉冲数；S2是快速的预分频数计算出
	//来的单脉 冲时间；T是已知总脉冲跑完所需要的总时间即是APP设置的时间

	//计算S1
	speed_slow = (double)(speeds + 1) * (double)501 / (double)48000;
	//计算S2
	speed_fast = (double)(speeds ) * (double)501 / (double)48000;

	//计算T, P
	times = con_b.move_time_s + con_b.move_time_m*60 + con_b.move_time_h*3600;
	if(video_p.locusV.ram_if)
	{
		times = times * 1000 - video_p.locusV.ram_time;
		total_p = video_p.locusV.pluses - video_p.locusV.ram_dist * 2;
	}
	else
	{
		times = times * 1000;
		total_p = video_p.locusV.pluses;
	}
	
	//求出X
	com_pulse = (double)(times - total_p*speed_fast) / (double)(speed_slow - speed_fast);
	if(com_pulse >= (total_p-1000))
	{
		com_pulse = 0;
	}
	
	
	if(video_p.locusV.ram_if==0)
	{
		if(com_pulse)
		{
			video_p.locusV.com_tag = 1;
			if(video_p.locusV.point_pulse_a > video_p.locusV.point_pulse_b)
			{
				video_p.locusV.com_pulse_a = video_p.locusV.point_pulse_a - com_pulse;
				video_p.locusV.com_pulse_b = video_p.locusV.point_pulse_b + com_pulse;
			}
			else
			{
				video_p.locusV.com_pulse_a = video_p.locusV.point_pulse_a + com_pulse;
				video_p.locusV.com_pulse_b = video_p.locusV.point_pulse_b - com_pulse;
			}
		}
		else
		{
			video_p.locusV.com_tag = 0;
		}
	}
	else
	{
		if(com_pulse)
		{
			video_p.locusV.com_tag = 1;
			if(video_p.locusV.point_pulse_a > video_p.locusV.point_pulse_b)
			{
				video_p.locusV.com_pulse_a = video_p.locusV.point_pulse_a - com_pulse - video_p.locusV.ram_dist;
				video_p.locusV.com_pulse_b = video_p.locusV.point_pulse_b + com_pulse + video_p.locusV.ram_dist;
			}
			else
			{
				video_p.locusV.com_pulse_a = video_p.locusV.point_pulse_a + com_pulse + video_p.locusV.ram_dist;
				video_p.locusV.com_pulse_b = video_p.locusV.point_pulse_b - com_pulse - video_p.locusV.ram_dist;
			}
		}
		else
		{
			video_p.locusV.com_tag = 0;
		}
	}

	//String_Printf(USART_2,"BB",2);
	//String_Printf(USART_2,(uint8_t *)&video_p.locusV.pluses,4);
	//String_Printf(USART_2,(uint8_t *)&com_pulse,4);
	//String_Printf(USART_2,(uint8_t *)&video_p.locusV.ram_dist,2);
}

//计算：最快时间，最慢时间，定时器的预分频数，缓起缓停
void MotorVV_Video_move_time_check(void)
{
	uint32_t time_mH=0;
//	uint32_t HFast_t=0;
	uint32_t mini_t=0;
	uint32_t temp=0;
//	uint8_t send_t=0;
	uint16_t i;
	float time_basic=0.0;

/////////////////  motor H /////////////////////////////////
	if(video_p.locusV.pluses == 0)return;
	
	time_mH = con_b.move_time_s + con_b.move_time_m*60 + con_b.move_time_h*3600;
	time_mH *= 1000; //毫秒级
	
	time_basic = (SLOW_THRESHOLD+1)*501.0 / 48000.0;
	mini_t = time_basic * video_p.locusV.pluses;
	
			if(time_mH <= mini_t)
			{
				video_p.locusV.ram_if = 0xff;
				for(i=BASE_SPEED; i<SLOW_THRESHOLD;i++)
				{
					temp =	calculate_fast_speed(i, MOTOR_Vertical);
					
					if(temp > time_mH)
					{
						// 取大于运行时间的数，便于计算补尝速度
						video_p.locusV.speed = i;
						break;
					}
					//time_delay_ms(40);
				}

				if(i>=SLOW_THRESHOLD)video_p.locusV.speed = SLOW_THRESHOLD;
				
				Video_ram_load(video_p.locusV.speed, MOTOR_Vertical);
			}
			else
			{
				video_p.locusV.speed = Times_convert_to_speed(time_mH, video_p.locusV.pluses);
				video_p.locusV.ram_if = 0;
			}
		
		if(video_p.locusV.pluses < (video_p.locusV.ram_dist * 2))
		{
			video_p.locusV.speed = Times_convert_to_speed(time_mH, video_p.locusV.pluses);
			video_p.locusV.ram_if = 0;
		}
	
	MotorVV_Video_compensation_check(video_p.locusV.speed);
	
}


//设置的运行时长与实际计算出来的运行时长偏差太大时， 进行计算变速补尝
void MotorHH_Video_compensation_check(uint16_t speeds)
{
	double speed_slow = 0.0;
	double speed_fast = 0.0;
	uint32_t com_pulse = 0;
	uint32_t times = 0;
	uint32_t total_p = 0;
//	uint8_t psd_bsic = 0;

/*	if(video_p.locusH[iid].ram_if)
	{
		speeds +=1;
	}*/
	// 公式x * s1 + (p-x)*s2 = t; 即x = (t - p*s2) / (s1 - s2)；X是对应S1所需的脉冲数；
	//S1是慢速的预分频数算出来的单脉冲时间；P是已知的总脉冲数；S2是快速的预分频数计算出
	//来的单脉 冲时间；T是已知总脉冲跑完所需要的总时间即是APP设置的时间

	//计算S1
	speed_slow = (double)(speeds + 1) * (double)501 / (double)48000;
	//计算S2
	speed_fast = (double)(speeds ) * (double)501 / (double)48000;

	//计算T, P
	times = con_b.move_time_s + con_b.move_time_m*60 + con_b.move_time_h*3600;
	if(video_p.locusH.ram_if)
	{
		times = times * 1000 - video_p.locusH.ram_time;
		total_p = video_p.locusH.pluses - video_p.locusH.ram_dist* 2;
	}
	else
	{
		times = times * 1000;
		total_p = video_p.locusH.pluses;
	}
	
	//求出X
	com_pulse = (double)(times - total_p*speed_fast) / (double)(speed_slow - speed_fast);
	if(com_pulse >= (total_p-1000))
	{
		com_pulse = 0;
	}
	
	if(video_p.locusH.ram_if==0)
	{
		if(com_pulse)
		{
			video_p.locusH.com_tag = 1;
			if(video_p.locusH.point_pulse_a > video_p.locusH.point_pulse_b)
			{
				video_p.locusH.com_pulse_a = video_p.locusH.point_pulse_a - com_pulse;
				video_p.locusH.com_pulse_b = video_p.locusH.point_pulse_b + com_pulse;
			}
			else
			{
				video_p.locusH.com_pulse_a = video_p.locusH.point_pulse_a + com_pulse;
				video_p.locusH.com_pulse_b = video_p.locusH.point_pulse_b - com_pulse;
			}
		}
		else
		{
			video_p.locusH.com_tag = 0;
		}
	}
	else
	{
		if(com_pulse)
		{
			video_p.locusH.com_tag = 1;
			if(video_p.locusH.point_pulse_a > video_p.locusH.point_pulse_b)
			{
				video_p.locusH.com_pulse_a = video_p.locusH.point_pulse_a - com_pulse - video_p.locusH.ram_dist;
				video_p.locusH.com_pulse_b = video_p.locusH.point_pulse_b + com_pulse + video_p.locusH.ram_dist;
			}
			else
			{
				video_p.locusH.com_pulse_a = video_p.locusH.point_pulse_a + com_pulse + video_p.locusH.ram_dist;
				video_p.locusH.com_pulse_b = video_p.locusH.point_pulse_b - com_pulse - video_p.locusH.ram_dist;
			}
		}
		else
		{
			video_p.locusH.com_tag = 0;
		}
	}
}


//计算：最快时间，最慢时间，定时器的预分频数，缓起缓停
void MotorHH_Video_move_time_check(void)
{
	uint32_t time_mH=0;
//	uint32_t HFast_t=0;
//	uint32_t VFast_t=0;
	uint32_t temp=0;
//	uint8_t send_t=0;
	uint16_t i;
	float time_basic=0.0;
	uint32_t mini_t=0;

/////////////////  motor H /////////////////////////////////
	if(video_p.locusH.pluses == 0)return;
	
	time_mH = con_b.move_time_s + con_b.move_time_m*60 + con_b.move_time_h*3600;
	time_mH *= 1000; //毫秒级
	
	time_basic = (SLOW_THRESHOLD+1)*501.0 / 48000.0;
	mini_t = time_basic * video_p.locusH.pluses;
	
			if(time_mH <= mini_t)
			{
				video_p.locusH.ram_if = 0xff;
				for(i=BASE_SPEED; i<SLOW_THRESHOLD;i++)
				{
					temp =	calculate_fast_speed(i, MOTOR_HORITAL);
					
					if(temp > time_mH)
					{
						// 取大于运行时间的数，便于计算补尝速度
						video_p.locusH.speed = i;
						break;
					}
					//time_delay_ms(40);
				}

				if(i>=SLOW_THRESHOLD)video_p.locusH.speed = SLOW_THRESHOLD;
				
				Video_ram_load(video_p.locusH.speed, MOTOR_HORITAL);

			}
			else
			{
				video_p.locusH.speed = Times_convert_to_speed(time_mH, video_p.locusH.pluses);
				video_p.locusH.ram_if = 0;
			}
		
		if(video_p.locusH.pluses < (video_p.locusH.ram_dist * 2))
		{
			video_p.locusH.speed = Times_convert_to_speed(time_mH, video_p.locusH.pluses);
			video_p.locusH.ram_if = 0;
		}

	MotorHH_Video_compensation_check(video_p.locusH.speed);
}


// 只限找AB点或设置AB点时使用
void set_abpoint_start_setting(MOTOR_TYPE motor_t)
{
	//abpt_slstart.dynamic_speed = 0;
/*	if(motor_t == MOTOR_Vertical)
	{
		find_pataVV.compensation = 266;
		find_pataVV.HHfind_Apoint= 1;
		find_pataVV.speed_add = SLOW_THRESHOLD;
		motorVV_speed_set(find_pataVV.speed_add);
		find_pataVV.find_abpoint = SLOW_START;
		motorVV_start();
	}
	else */if(motor_t == MOTOR_HORITAL)
	{
		//find_pata.compensation = 266;
		find_pata.HHfind_Apoint= 1;
		find_pata.speed_add = SLOW_THRESHOLD;
		motorHH_speed_set(find_pata.speed_add);
		find_pata.find_abpoint = SLOW_START;
		motorHH_start();
	}
}

// 进入视频模式要自动找原点
void Video_find_Apoint(void)
{
	if(motorVV_p.pulse_count != video_p.locusV.point_a)
	{
		if(video_p.locusV.pluses)
		{
			if(motorVV_p.pulse_count > video_p.locusV.point_a)
			{
				if((motorVV_p.pulse_count - video_p.locusV.point_a) > (M_POINT_END-ORIGIN_POINT))
				{
					motorVV_p.DIR = A_TO_B;
				}
				else
				{
					motorVV_p.DIR = B_TO_A;
				}
			}
			else if(motorVV_p.pulse_count < video_p.locusV.point_a)
			{
				if((video_p.locusV.point_a - motorVV_p.pulse_count) > (M_POINT_END-ORIGIN_POINT))
				{
					motorVV_p.DIR = B_TO_A;
				}
				else
				{
					motorVV_p.DIR = A_TO_B;
				}
			}
			motorVV_direction_change(motorVV_p.DIR);

			set_abpoint_start_setting(MOTOR_Vertical);
			Red_led_tack();
		}
	}
	else
	{
		motorVV_p.DVpulse_count = video_p.locusV.point_pulse_a;
		video_p.DIR = A_TO_B;
		video_p.locusV.dir = video_p.DIR;
		//video_p.locusV.dir = video_p.DIR;
		video_Dir_check(MOTOR_Vertical);
	}

	if(motorHH_p.pulse_count != video_p.locusH.point_a)
	{
		if(video_p.locusH.pluses)
		{
			if(motorHH_p.pulse_count > video_p.locusH.point_a)
			{
				if((motorHH_p.pulse_count - video_p.locusH.point_a) > (M_POINT_END-ORIGIN_POINT))
				{
					motorHH_p.DIR = A_TO_B;
				}
				else
				{
					motorHH_p.DIR = B_TO_A;
				}
			}
			else if(motorHH_p.pulse_count < video_p.locusH.point_a)
			{
				if((video_p.locusH.point_a - motorHH_p.pulse_count) > (M_POINT_END-ORIGIN_POINT))
				{
					motorHH_p.DIR = B_TO_A;
				}
				else
				{
					motorHH_p.DIR = A_TO_B;
				}
			}
			motorHH_direction_change(motorHH_p.DIR);

			set_abpoint_start_setting(MOTOR_HORITAL);
			Red_led_tack();
			Video_Find_ABpoint_notify(LOOKING_ORIGIN_POINT);
		}
	}
	else
	{
		motorHH_p.DVpulse_count = video_p.locusH.point_pulse_a;
		video_p.DIR = A_TO_B;
		video_p.locusH.dir = video_p.DIR;
		//video_p.locusV.dir = video_p.DIR;
		video_Dir_check(MOTOR_HORITAL);
	}
}

// 视频模式自动找原点到达后的停止
void Video_find_Origin_checkStop(MOTOR_TYPE motor_t)
{
//	if(mode_backup != VIDEO_MODE)return;
	
	if(motor_t == MOTOR_HORITAL)
	{
		if(find_pata.HHfind_Apoint == 0)return;
		if(video_p.locusH.check_dir)return; //不是找原点而是 找A /B点的检测时退出
		if(video_p.locusH.point_a==0)return;
		
		if(motorHH_p.pulse_count == video_p.locusH.point_a)
		{
			find_pata.HHfind_Apoint = 0;
			find_pata.slow_pulse = 0;
			find_pata.find_abpoint = 0;
			motorHH_stop();
			motorHH_p.DVpulse_count = video_p.locusH.point_pulse_a;
			video_p.DIR = A_TO_B;
			video_p.locusH.dir = video_p.DIR;
			//video_p.locusV.dir = video_p.DIR;
			video_Dir_check(motor_t);
			Send_connect_data_to_controller();
			Video_Find_ABpoint_notify(REACHED_ORIGIN_POINT);
		}
	}
	else if(motor_t == MOTOR_Vertical)
	{
		if(video_p.locusV.check_dir)return; //不是找原点而是 找A /B点的检测时退出
		if(find_pataVV.HHfind_Apoint == 0)return;
		if(video_p.locusV.point_a==0)return;
		if(motorVV_p.pulse_count == video_p.locusV.point_a)
		{
			find_pataVV.HHfind_Apoint = 0;
			find_pataVV.slow_pulse = 0;
			find_pataVV.find_abpoint = 0;
			motorVV_stop();
			motorVV_p.DVpulse_count = video_p.locusV.point_pulse_a;
			
			video_p.DIR = A_TO_B;
			//video_p.locusH.dir = video_p.DIR;
			video_p.locusV.dir = video_p.DIR;
			video_Dir_check(motor_t);
			Send_connect_data_to_controller();
		}
	}
}

void Video_find_Origin_slow_check(MOTOR_TYPE motor_t)
{
	if(mode_backup != VIDEO_MODE)return;
	
	if(motor_t == MOTOR_HORITAL)
	{
		if(video_p.locusH.check_dir)return;
		if(find_pata.HHfind_Apoint == 0)return;
		if(video_p.locusH.point_a==0)return;

		if(motorHH_p.DIR == A_TO_B)
		{
			if(video_p.locusH.point_a < find_pata.slow_pulse)
			{
				if(motorHH_p.pulse_count>= B_POINT_END - (find_pata.slow_pulse-video_p.locusH.point_a))
				{
					find_pata.find_abpoint = SLOW_STOP;
				}
			}
			else
			{
				if(motorHH_p.pulse_count < video_p.locusH.point_a)
				{
					if(motorHH_p.pulse_count >= video_p.locusH.point_a - find_pata.slow_pulse)
					{
						find_pata.find_abpoint = SLOW_STOP;
					}
				}
			}
		}
		else
		{
			if((video_p.locusH.point_a + find_pata.slow_pulse) > B_POINT_END)
			{
				if(motorHH_p.pulse_count <= ((video_p.locusH.point_a + find_pata.slow_pulse)-B_POINT_END))
				{
					find_pata.find_abpoint = SLOW_STOP;
				}
			}
			else
			{
				if(motorHH_p.pulse_count > video_p.locusH.point_a)
				{
					if(motorHH_p.pulse_count <= video_p.locusH.point_a + find_pata.slow_pulse)
					{
						find_pata.find_abpoint = SLOW_STOP;
					}
				}
			}
		}
	}
	else if(motor_t == MOTOR_Vertical)
	{
		if(video_p.locusV.check_dir)return;
		if(find_pataVV.HHfind_Apoint == 0)return;
		if(video_p.locusV.point_a==0)return;

		if(motorVV_p.DIR == A_TO_B)
		{
			if(video_p.locusV.point_a < find_pataVV.slow_pulse)
			{
				if(motorVV_p.pulse_count>= B_POINT_END - (find_pataVV.slow_pulse-video_p.locusV.point_a))
				{
					find_pataVV.find_abpoint = SLOW_STOP;
				}
			}
			else
			{
				if(motorVV_p.pulse_count < video_p.locusV.point_a)
				{
					if(motorVV_p.pulse_count >= video_p.locusV.point_a - find_pataVV.slow_pulse)
					{
						find_pataVV.find_abpoint = SLOW_STOP;
					}
				}
			}
		}
		else
		{
			if((video_p.locusV.point_a + find_pata.slow_pulse) > B_POINT_END)
			{
				if(motorVV_p.pulse_count <= ((video_p.locusV.point_a + find_pataVV.slow_pulse)-B_POINT_END))
				{
					find_pataVV.find_abpoint = SLOW_STOP;
				}
			}
			else
			{
				if(motorVV_p.pulse_count > video_p.locusV.point_a)
				{
					if(motorVV_p.pulse_count <= video_p.locusV.point_a + find_pataVV.slow_pulse)
					{
						find_pataVV.find_abpoint = SLOW_STOP;
					}
				}
			}
		}
	}
}

void video_Dir_check(MOTOR_TYPE motor_t)
{
	if(motor_t == MOTOR_HORITAL)
	{
		if(video_p.locusH.dir == A_TO_B)
		{
			motorHH_p.DIR = (video_p.locusH.point_pulse_a > video_p.locusH.point_pulse_b)?B_TO_A : A_TO_B;
		}
		else
		{
			motorHH_p.DIR = (video_p.locusH.point_pulse_a > video_p.locusH.point_pulse_b)?A_TO_B : B_TO_A;
			//glob_para.orbital_dir = ~glob_para.orbital_dir;
		}

		motorHH_direction_change(motorHH_p.DIR);
	}
	else if(motor_t == MOTOR_Vertical)
	{
		if(video_p.locusV.dir == A_TO_B)
		{
			motorVV_p.DIR = (video_p.locusV.point_pulse_a > video_p.locusV.point_pulse_b)?B_TO_A : A_TO_B;
		}
		else
		{
			motorVV_p.DIR = (video_p.locusV.point_pulse_a > video_p.locusV.point_pulse_b)?A_TO_B : B_TO_A;
			//glob_para.orbital_dir = ~glob_para.orbital_dir;
		}

		motorVV_direction_change(motorVV_p.DIR);
	}
}


//视频模式缓启停处理涵数
void Video_MotorHH_slowly_startedORstop(void)
{
//	if(ramp_flag == 0)return;
	if(mode_backup != VIDEO_MODE)return;
	if(video_p.locusH.p_upORdown==0)return;
	//if(p_upORdown == check_speed_tag)return;
	if(video_p.locusH.slow_time)return;

	if(video_p.locusH.p_upORdown == SLOW_STOP)
	{
		//motor_speed_set(dynamic_speed);
		
		if(video_p.locusH.dynamic_speed < video_p.locusV.ram_id)
		{
			video_p.locusH.dynamic_speed++;
			motorHH_speed_set(video_p.locusH.ram_buffer[video_p.locusH.dynamic_speed]);
		}
		else 
		{
			video_p.locusH.p_upORdown = 0;
			
			if(video_p.Pause == 0)
			{	
				motorHH_stop();
			}
		//String_Printf(USART_2, (uint8_t *)&glob_para.pulse_cout, 4);
		//	time_delay_us(10000);
			//String_Printf(USART_2, (uint8_t *)&glob_para.pulse_cout11, 4);
			
		}
	}
	else if(video_p.locusH.p_upORdown == SLOW_START)
	{
		//motor_speed_set(dynamic_speed);
		
		if(video_p.locusH.dynamic_speed > 0)
		{
			video_p.locusH.dynamic_speed--;
			motorHH_speed_set(video_p.locusH.ram_buffer[video_p.locusH.dynamic_speed]);
			//String_Printf(USART_2, (uint8_t *)&con_b.ramp_buffer[dynamic_speed], 2);
		}
		else 
		{
			video_p.locusH.p_upORdown = 0;
		//	String_Printf(USART_2, (uint8_t *)&con_b.Dynamic_pulse, 4);
			//String_Printf(USART_2, (uint8_t *)&con_b.slow_point, 4);
		}
	}
	
	//set_slow_time_data();
	video_p.locusH.slow_time = SLOW_BASIC_TIME11;
}


//视频模式缓启停处理涵数
void Video_MotorVV_slowly_startedORstop(void)
{
//	if(ramp_flag == 0)return;
	if(mode_backup != VIDEO_MODE)return;
	if(video_p.locusV.p_upORdown==0)return;
	//if(p_upORdown == check_speed_tag)return;
	if(video_p.locusV.slow_time)return;

	if(video_p.locusV.p_upORdown == SLOW_STOP)
	{
		//motor_speed_set(dynamic_speed);
		
		if(video_p.locusV.dynamic_speed < video_p.locusV.ram_id)
		{
			video_p.locusV.dynamic_speed++;
			motorVV_speed_set(video_p.locusV.ram_buffer[video_p.locusV.dynamic_speed]);
		}
		else 
		{
			video_p.locusV.p_upORdown = 0;
			
			if(video_p.VVPause == 0)
			{	
				motorVV_stop();
			}
		//String_Printf(USART_2, (uint8_t *)&glob_para.pulse_cout, 4);
		//	time_delay_us(10000);
			//String_Printf(USART_2, (uint8_t *)&glob_para.pulse_cout11, 4);
			
		}
	}
	else if(video_p.locusV.p_upORdown == SLOW_START)
	{
		//motor_speed_set(dynamic_speed);
		
		if(video_p.locusV.dynamic_speed > 0)
		{
			video_p.locusV.dynamic_speed--;
			motorVV_speed_set(video_p.locusV.ram_buffer[video_p.locusV.dynamic_speed]);
			//String_Printf(USART_2, (uint8_t *)&con_b.ramp_buffer[dynamic_speed], 2);
		}
		else 
		{
			video_p.locusV.p_upORdown = 0;
		//	String_Printf(USART_2, (uint8_t *)&con_b.Dynamic_pulse, 4);
			//String_Printf(USART_2, (uint8_t *)&con_b.slow_point, 4);
		}
	}
	
	//set_slow_time_data();
	video_p.locusV.slow_time = SLOW_BASIC_TIME11;
}

void Video_motorHH_start(uint8_t types)
{
	if(video_p.locusH.pluses)
	{
		if(types)
		{
			if(video_p.locusH.ram_if)
			{
				video_p.locusH.dynamic_speed = video_p.locusH.ram_id;
				motorHH_speed_set(video_p.locusH.ram_buffer[video_p.locusH.dynamic_speed]);
				video_p.locusH.p_upORdown = SLOW_START;
				video_p.locusH.slow_time = SLOW_BASIC_TIME11;
			}
			else
			{
				motorHH_speed_set(video_p.locusH.speed);
			}
			motorHH_start();
		}
		else
		{
			if(video_p.locusH.ram_if)
			{
				video_p.locusH.p_upORdown = SLOW_STOP;
			}
			else
			{
				motorHH_stop();
			}
		}
	}
	else 
	{
		video_p.Pause = 0;
	}
}

void Video_motorVV_start(uint8_t types)
{
	if(video_p.locusV.pluses)
	{
		if(types)
		{
			if(video_p.locusV.ram_if)
			{
				video_p.locusV.dynamic_speed = video_p.locusV.ram_id;
				motorVV_speed_set(video_p.locusV.ram_buffer[video_p.locusV.dynamic_speed]);
				video_p.locusV.p_upORdown = SLOW_START;
				video_p.locusV.slow_time = SLOW_BASIC_TIME11;
			}
			else
			{
				motorVV_speed_set(video_p.locusV.speed);
			}
			motorVV_start();
		}
		else
		{
			if(video_p.locusV.ram_if)
			{
				video_p.locusV.p_upORdown = SLOW_STOP;
			}
			else
			{
				motorVV_stop();
			}
		}
	}
	else
	{
		video_p.VVPause = 0;
	}
}

void Video_start(void)
{
	if(video_p.Pause)
	{
		BLE_shut_start();
		camera_shutter_shot(1);
	}
	else
	{
		BLE_shut_start();
		camera_shutter_shot(1);
	}
	Video_motorHH_start(video_p.Pause);
	Video_motorVV_start(video_p.VVPause);
}

// 水平抽AB 点检测
void video_motorHH_pulse_check(void)
{
	uint8_t temp = 0;
	
//	if(mode_backup != VIDEO_MODE)return;
	if(video_p.locusH.pluses == 0)return;
	if(find_pata.HHfind_Apoint)return;
	if(video_p.locusH.check_dir)return;
	if(video_p.locusV.check_dir)return;
	if(video_p.locusH.pluses == 0)return;
	
		if(motorHH_p.DVpulse_count == video_p.locusH.point_pulse_a)
		{
			
			if(limit_360_tag==0)video_p.DIR = A_TO_B;
			if(limit_360_tag==0)video_p.locusH.dir = video_p.DIR;
			//video_p.locusV.dir = video_p.DIR;
			temp = 1;
		}
		else if(motorHH_p.DVpulse_count == video_p.locusH.point_pulse_b)
		{
			
			if(limit_360_tag==0)video_p.DIR = B_TO_A;
			if(limit_360_tag==0)video_p.locusH.dir = video_p.DIR;
			//video_p.locusV.dir = video_p.DIR;
			temp = 1;
		}

	if(temp)
	{
		if(limit_360_tag==0)
		{
			video_Dir_check(MOTOR_HORITAL);
		}
		else // 360时，循环检测脉冲数，
		{
			motorHH_p.DVpulse_count = (video_p.locusH.dir == B_TO_A)?video_p.locusH.point_pulse_b : video_p.locusH.point_pulse_a;
		}
		
		if(video_p.loop)
		{
			if(video_p.locusH.ram_if)
			{
				//if(limit_360_tag)return;
				//if(video_p.locusV.dir == video_p.locusH.dir)
				if(limit_360_tag==0)
				{
				//	slider_upORdown(SLOW_START);
					//video_p.locusV.p_upORdown = SLOW_START;
					//video_p.locusV.slow_time = SLOW_BASIC_TIME11;
					video_p.locusH.p_upORdown = SLOW_START;
					video_p.locusH.slow_time = SLOW_BASIC_TIME11;
				}
			}
			Send_connect_data_to_controller();
		}
		else
		{
			motorHH_stop();
			/*if(limit_360_tag)
			{
				if(video_p.locusH.dir == A_TO_B)motorHH_p.DVpulse_count = video_p.locusH.point_pulse_a;
				else if(video_p.locusH.dir == B_TO_A)motorHH_p.DVpulse_count = video_p.locusH.point_pulse_b;
			}*/
			
			video_p.Pause = 0;
			if(video_p.VVPause==0)
			{
				Send_connect_data_to_controller();
				BLE_shut_start();
				camera_shutter_shot(1);
			}
		}
	}
}

// V 抽AB 点检测
void video_motorVV_pulse_check(void)
{
	uint8_t temp = 0;
	
	if(mode_backup != VIDEO_MODE)return;
	if(video_p.locusV.pluses == 0)return;
	if(find_pataVV.HHfind_Apoint)return;
	if(video_p.locusH.check_dir)return;
	if(video_p.locusV.check_dir)return;
	if(video_p.locusV.pluses == 0)return;
	
		if(motorVV_p.DVpulse_count == video_p.locusV.point_pulse_a)
		{
			video_p.DIR = A_TO_B;
			//video_p.locusH.dir = video_p.DIR;
			video_p.locusV.dir = video_p.DIR;
			temp = 1;
		}
		else if(motorVV_p.DVpulse_count == video_p.locusV.point_pulse_b)
		{
			
			video_p.DIR = B_TO_A;
			//video_p.locusH.dir = video_p.DIR;
			video_p.locusV.dir = video_p.DIR;
			temp = 1;
		}

	if(temp)
	{
		video_Dir_check(MOTOR_Vertical);
		
		if(video_p.loop)
		{
		//	if(video_p.locusV.dir == video_p.locusH.dir)
			{
				//slider_upORdown(SLOW_START);
				video_p.locusV.p_upORdown = SLOW_START;
				video_p.locusV.slow_time = SLOW_BASIC_TIME11;
				//video_p.locusH.p_upORdown = SLOW_START;
				//video_p.locusH.slow_time = SLOW_BASIC_TIME11;
			}
		}
		else
		{
			motorVV_stop();
			video_p.VVPause = 0;
			if(video_p.Pause==0)
			{
				Send_connect_data_to_controller();
				BLE_shut_start();
			}
			
		}
	}
}

// 缓停检测
void Video_slow_pulse_check(MOTOR_TYPE motor_t)
{
//	if(mode_backup != VIDEO_MODE)return;
	if(find_pata.HHfind_Apoint)return;
	if(find_pataVV.HHfind_Apoint)return;
	
	if(motor_t == MOTOR_HORITAL)
	{
		if(video_p.locusH.pluses == 0)return;
		if(video_p.locusH.check_dir)return;
		if(video_p.loop)if(limit_360_tag)return;
		
		if(video_p.locusH.ram_if)
		{
			if(video_p.locusH.point_pulse_a > video_p.locusH.point_pulse_b)
			{
				if(motorHH_p.DVpulse_count == video_p.locusH.point_pulse_a - video_p.locusH.ram_dist)
				{
					if(video_p.locusH.dir == B_TO_A)
					{
						video_p.locusH.p_upORdown = SLOW_STOP;
					}
				}
				else if(motorHH_p.DVpulse_count == video_p.locusH.point_pulse_b + video_p.locusH.ram_dist)
				{
					if(video_p.locusH.dir == A_TO_B)
					{
						video_p.locusH.p_upORdown = SLOW_STOP;
					}
				}
			}
			else if(video_p.locusH.point_pulse_a < video_p.locusH.point_pulse_b)
			{
				if(motorHH_p.DVpulse_count == video_p.locusH.point_pulse_a + video_p.locusH.ram_dist)
				{
					if(video_p.locusH.dir == B_TO_A)
					{
						video_p.locusH.p_upORdown = SLOW_STOP;
					}
				}
				else if(motorHH_p.DVpulse_count == video_p.locusH.point_pulse_b - video_p.locusH.ram_dist)
				{
					if(video_p.locusH.dir == A_TO_B)
					{
						video_p.locusH.p_upORdown = SLOW_STOP;
					}
				}
			}
		}
	}	
	else if(motor_t == MOTOR_Vertical)
	{
		if(video_p.locusV.pluses == 0)return;
		if(video_p.locusV.check_dir)return;
		
		if(video_p.locusV.ram_if)
		{
			if(video_p.locusV.point_pulse_a > video_p.locusV.point_pulse_b)
			{
				if(motorVV_p.DVpulse_count == video_p.locusV.point_pulse_a - video_p.locusV.ram_dist)
				{
					if(video_p.locusV.dir == B_TO_A)
					{
						video_p.locusV.p_upORdown = SLOW_STOP;
					}
				}
				else if(motorVV_p.DVpulse_count == video_p.locusV.point_pulse_b + video_p.locusV.ram_dist)
				{
					if(video_p.locusV.dir == A_TO_B)
					{
						video_p.locusV.p_upORdown = SLOW_STOP;
					}
				}
			}
			else if(video_p.locusV.point_pulse_a < video_p.locusV.point_pulse_b)
			{
				if(motorVV_p.DVpulse_count == video_p.locusV.point_pulse_a + video_p.locusV.ram_dist)
				{
					if(video_p.locusV.dir == B_TO_A)
					{
						video_p.locusV.p_upORdown = SLOW_STOP;
					}
				}
				else if(motorVV_p.DVpulse_count == video_p.locusV.point_pulse_b - video_p.locusV.ram_dist)
				{
					if(video_p.locusV.dir == A_TO_B)
					{
						video_p.locusV.p_upORdown = SLOW_STOP;
					}
				}
			}
		}
	}
}

// 变速补尝速度
void Video_compensation_change_speed(MOTOR_TYPE motor_tt)
{
//	if(mode_backup != VIDEO_MODE)return;
	if(video_p.Pause == 0)return;
	
	if(motor_tt == MOTOR_HORITAL)
	{
		if(video_p.locusH.check_dir)return;
		if(video_p.locusH.com_tag)
		{
			if(video_p.locusH.dir == A_TO_B)
			{
				if(motorHH_p.DVpulse_count == video_p.locusH.com_pulse_a)
				{
					motorHH_speed_set(video_p.locusH.speed-1);
				}
			}
			else if(video_p.locusH.dir == B_TO_A)
			{
				if(motorHH_p.DVpulse_count == video_p.locusH.com_pulse_b)
				{
					motorHH_speed_set(video_p.locusH.speed-1);
				}
			}
		}
	}	
	else if(motor_tt == MOTOR_Vertical)
	{
		if(video_p.locusV.check_dir)return;
		if(video_p.locusV.com_tag)
		{
			if(video_p.locusV.dir == A_TO_B)
			{
				if(motorVV_p.DVpulse_count == video_p.locusV.com_pulse_a)
				{
					motorVV_speed_set(video_p.locusV.speed-1);
				}
			}
			else if(video_p.locusV.dir == B_TO_A)
			{
				if(motorVV_p.DVpulse_count == video_p.locusV.com_pulse_b)
				{
					motorVV_speed_set(video_p.locusV.speed-1);
				}
			}
		}
	}
}

void Video_mode_para_clear(void)
{
	find_pataVV.find_abpoint = 0;
	find_pataVV.HHfind_Apoint= 0;
	find_pata.HHfind_Apoint= 0;
	find_pata.find_abpoint = 0;
	video_p.locusH.check_dir = 0;
	video_p.locusV.check_dir = 0;
	delay_p.locuVV.check_dir = 0;
	delay_p.locuHH.check_dir = 0;
	
	find_pata.slow_pulse = 0;
	find_pataVV.slow_pulse = 0;

	video_p.Pause = 0;
	video_p.VVPause = 0;
	video_p.locusH.p_upORdown = 0;
	video_p.locusV.p_upORdown = 0;
}

// 启动时检测方向是否正确，不正确就先找A 后正式开拍
uint8_t Video_MotorHH_Start_check_diretion(void)
{
	uint8_t temp = 0;

	if(video_p.locusH.pluses == 0)return 0;
	if(limit_360_tag)return 0;
//	if(con_b.vvAB_set != 3)return 0;
	
	if(video_p.locusH.dir == A_TO_B)
	{
		if(motorHH_p.DVpulse_count == video_p.locusH.point_pulse_a)return 0;
		if(motorHH_p.DVpulse_count > video_p.locusH.point_pulse_a)
		{
			motorHH_p.DIR = B_TO_A;
			temp = LOOKING_A_POINT;
		}
		else if(motorHH_p.DVpulse_count < video_p.locusH.point_pulse_a)
		{
			motorHH_p.DIR = A_TO_B;
			temp = LOOKING_A_POINT;
		}
	}
	else if(video_p.locusH.dir == B_TO_A )
	{
		if(motorHH_p.DVpulse_count == video_p.locusH.point_pulse_b)return 0;
		if(motorHH_p.DVpulse_count > video_p.locusH.point_pulse_b)
		{
			motorHH_p.DIR = B_TO_A;
			temp = LOOKING_B_POINT;
		}
		else if(motorHH_p.DVpulse_count < video_p.locusH.point_pulse_b)
		{
			motorHH_p.DIR = A_TO_B;
			temp = LOOKING_B_POINT;
		}
	}

	if(temp)
	{
		video_p.locusH.check_dir = 1;
		motorHH_direction_change(motorHH_p.DIR);
		set_abpoint_start_setting(MOTOR_HORITAL);
		Red_led_tack();
		Video_Find_ABpoint_notify(temp);
		/*//video_Dir_check(MOTOR_HORITAL);
		motorHH_direction_change(motorHH_p.DIR);
		Video_ram_load(BASE_SPEED, MOTOR_HORITAL);
		video_p.locusH.dynamic_speed = video_p.locusH.ram_id;
		motorHH_speed_set(video_p.locusH.ram_buffer[video_p.locusH.dynamic_speed]);
		video_p.locusH.p_upORdown = SLOW_START;
		motorHH_start();*/
	}

	return temp;
}

// 启动时检测方向是否正确，不正确就先找A 后正式开拍
uint8_t Video_MotorVV_Start_check_diretion(void)
{
	uint8_t temp = 0;
	
	if(video_p.locusV.pluses == 0)return 0;
//	if(con_b.vvAB_set != 3)return 0;
	
	if(video_p.locusV.dir == A_TO_B)
	{
		if(motorVV_p.DVpulse_count == video_p.locusV.point_pulse_a)return 0;
		if(motorVV_p.DVpulse_count > video_p.locusV.point_pulse_a)
		{
			motorVV_p.DIR = B_TO_A;
			temp = 1;
		}
		else if(motorVV_p.DVpulse_count < video_p.locusV.point_pulse_a)
		{
			motorVV_p.DIR = A_TO_B;
			temp = 1;
		}
	}
	else if(video_p.locusV.dir == B_TO_A )
	{
		if(motorVV_p.DVpulse_count == video_p.locusV.point_pulse_b)return 0;
		if(motorVV_p.DVpulse_count > video_p.locusV.point_pulse_b)
		{
			motorVV_p.DIR = B_TO_A;
			temp = 1;
		}
		else if(motorVV_p.DVpulse_count < video_p.locusV.point_pulse_b)
		{
			motorVV_p.DIR = A_TO_B;
			temp = 1;
		}
	}

	if(temp)
	{
		video_p.locusV.check_dir = 1;
		motorVV_direction_change(motorVV_p.DIR);
		set_abpoint_start_setting(MOTOR_Vertical);
		Red_led_tack();
		/*motorVV_direction_change(motorVV_p.DIR);
		Video_ram_load(BASE_SPEED, MOTOR_Vertical);
		video_p.locusV.dynamic_speed = video_p.locusV.ram_id;
		motorVV_speed_set(video_p.locusV.ram_buffer[video_p.locusV.dynamic_speed]);
		video_p.locusV.p_upORdown = SLOW_START;
		motorVV_start();*/
	}

	return temp;
}

// 找A/B 点到达后的停止处理涵数
void Video_FindABpoint_pluse_check(MOTOR_TYPE motor_tt)
{
//	if(mode_backup != VIDEO_MODE)return;
	if(motor_tt == MOTOR_HORITAL)
	{
		if(video_p.locusH.check_dir==0)return;

		if(video_p.locusH.dir == A_TO_B) // 实时是按B TO A 找A点
		{
			if(motorHH_p.DVpulse_count == video_p.locusH.point_pulse_a)
			{
				motorHH_stop();
				find_pata.HHfind_Apoint = 0;
				find_pata.slow_pulse = 0;
				find_pata.find_abpoint = 0;
				video_p.locusH.check_dir = 2;
				if(video_p.locusV.check_dir == 2 || video_p.locusV.check_dir == 0)
				{
					video_p.activate = 1000;
					video_p.locusV.check_dir = 2;
				}
				Video_Find_ABpoint_notify(REACHED_ABPOINT);
			}
		}
		else if(video_p.locusH.dir == B_TO_A)
		{
			if(motorHH_p.DVpulse_count == video_p.locusH.point_pulse_b)
			{
				motorHH_stop();
				find_pata.HHfind_Apoint = 0;
				find_pata.slow_pulse = 0;
				find_pata.find_abpoint = 0;
				video_p.locusH.check_dir = 2;
				if(video_p.locusV.check_dir == 2 || video_p.locusV.check_dir == 0)
				{
					video_p.activate = 1000;
					video_p.locusV.check_dir = 2;
				}
				Video_Find_ABpoint_notify(REACHED_ABPOINT);
			}
		}
	}
	/*else if(motor_tt == MOTOR_Vertical)
	{
		if(video_p.locusV.check_dir==0)return;

		if(video_p.locusV.dir == A_TO_B) // 实时是按B TO A 找A点
		{
			if(motorVV_p.DVpulse_count == video_p.locusV.point_pulse_a)
			{
				motorVV_stop();
				find_pataVV.HHfind_Apoint = 0;
				find_pataVV.slow_pulse = 0;
				find_pataVV.find_abpoint = 0;
				video_p.locusV.check_dir = 2;
				if(video_p.locusH.check_dir == 2 || video_p.locusH.check_dir == 0)
				{
					video_p.activate = 1000;
					video_p.locusH.check_dir = 2;
				}
			}
		}
		else if(video_p.locusV.dir == B_TO_A)
		{
			if(motorVV_p.DVpulse_count == video_p.locusV.point_pulse_b)
			{
				motorVV_stop();
				find_pataVV.HHfind_Apoint = 0;
				find_pataVV.slow_pulse = 0;
				find_pataVV.find_abpoint = 0;
				video_p.locusV.check_dir = 2;
				if(video_p.locusH.check_dir == 2 || video_p.locusH.check_dir == 0)
				{
					video_p.activate = 1000;
					video_p.locusH.check_dir = 2;
				}
			}
		}
	}*/
}

// 启动前找AB 点到达后的自动启动
void Video_FindABpoint_End(void)
{
	if(mode_backup != VIDEO_MODE)return;
	if(video_p.locusH.check_dir ==2 && video_p.locusV.check_dir==2)
	{
		if(video_p.Pause == 0) // 不是启动的找A / B 点时清零
		{
			video_p.locusH.check_dir = 0;
			video_p.locusV.check_dir = 0;
			video_p.activate = 0;
		}
		
		if(video_p.activate)return;		
		video_Dir_check(MOTOR_Vertical);
		video_Dir_check(MOTOR_HORITAL);
		MotorVV_Video_move_time_check();
		MotorHH_Video_move_time_check();
		Video_start();
		video_p.locusH.check_dir = 0;
		video_p.locusV.check_dir = 0;
	}
}

//启动时检测到方向不对就启动找点后的缓停动作
void Video_FindABpoint_slow_check(MOTOR_TYPE motor_t)
{
//	if(mode_backup != VIDEO_MODE)return;
	
	if(motor_t == MOTOR_HORITAL)
	{
		if(video_p.locusH.pluses == 0)return;
		if(video_p.locusH.check_dir==0)return;
		if(find_pata.HHfind_Apoint==0)return;
		//if(video_p.locusH.ram_if)
		{
		//	if(video_p.locusH.point_pulse_a > video_p.locusH.point_pulse_b)
			{
				if(video_p.locusH.dir == A_TO_B) // 回到A点再开始
				{
					if(motorHH_p.DIR == A_TO_B)
					{
						if(motorHH_p.DVpulse_count == (video_p.locusH.point_pulse_a - find_pata.slow_pulse))
						{
							find_pata.find_abpoint = SLOW_STOP;
						}
					}
					else
					{
						if(motorHH_p.DVpulse_count == (video_p.locusH.point_pulse_a + find_pata.slow_pulse))
						{
							find_pata.find_abpoint = SLOW_STOP;
						}
					}
				}
				else
				{
					if(motorHH_p.DIR == A_TO_B)
					{
						if(motorHH_p.DVpulse_count == (video_p.locusH.point_pulse_b - find_pata.slow_pulse))
						{
							find_pata.find_abpoint = SLOW_STOP;
						}
					}
					else
					{
						if(motorHH_p.DVpulse_count == (video_p.locusH.point_pulse_b + find_pata.slow_pulse))
						{
							find_pata.find_abpoint = SLOW_STOP;
						}
					}
				}
				
			/*	if(motorHH_p.DVpulse_count == video_p.locusH.point_pulse_a - video_p.locusH.ram_dist)
				{
					if(video_p.locusH.dir == A_TO_B)
					{
						video_p.locusH.p_upORdown = SLOW_STOP;
					}
				}
				else if(motorHH_p.DVpulse_count == video_p.locusH.point_pulse_b + video_p.locusH.ram_dist)
				{
					if(video_p.locusH.dir == B_TO_A)
					{
						video_p.locusH.p_upORdown = SLOW_STOP;
					}
				}*/
			}
		/*	else if(video_p.locusH.point_pulse_a < video_p.locusH.point_pulse_b)
			{
				if(motorHH_p.DVpulse_count == video_p.locusH.point_pulse_a + video_p.locusH.ram_dist)
				{
					if(video_p.locusH.dir == A_TO_B)
					{
						video_p.locusH.p_upORdown = SLOW_STOP;
					}
				}
				else if(motorHH_p.DVpulse_count == video_p.locusH.point_pulse_b - video_p.locusH.ram_dist)
				{
					if(video_p.locusH.dir == B_TO_A)
					{
						video_p.locusH.p_upORdown = SLOW_STOP;
					}
				}
			}*/
		}
	}	
	else if(motor_t == MOTOR_Vertical)
	{
		if(video_p.locusV.pluses == 0)return;
		if(video_p.locusV.check_dir==0)return;
		if(find_pataVV.HHfind_Apoint==0)return;

		if(video_p.locusV.dir == A_TO_B) // 回到A点再开始
		{
			if(motorVV_p.DIR == A_TO_B)
			{
				if(motorVV_p.DVpulse_count == (video_p.locusV.point_pulse_a - find_pataVV.slow_pulse))
				{
					find_pataVV.find_abpoint = SLOW_STOP;
				}
			}
			else
			{
				if(motorVV_p.DVpulse_count == (video_p.locusV.point_pulse_a + find_pataVV.slow_pulse))
				{
					find_pataVV.find_abpoint = SLOW_STOP;
				}
			}
		}
		else
		{
			if(motorVV_p.DIR == A_TO_B)
			{
				if(motorVV_p.DVpulse_count == (video_p.locusV.point_pulse_b - find_pataVV.slow_pulse))
				{
					find_pataVV.find_abpoint = SLOW_STOP;
				}
			}
			else
			{
				if(motorVV_p.DVpulse_count == (video_p.locusV.point_pulse_b + find_pataVV.slow_pulse))
				{
					find_pataVV.find_abpoint = SLOW_STOP;
				}
			}
		}
		/*if(video_p.locusV.ram_if)
		{
			if(video_p.locusV.point_pulse_a > video_p.locusV.point_pulse_b)
			{
				if(motorVV_p.DVpulse_count == video_p.locusV.point_pulse_a - video_p.locusV.ram_dist)
				{
					if(video_p.locusV.dir == A_TO_B)
					{
						video_p.locusV.p_upORdown = SLOW_STOP;
					}
				}
				else if(motorVV_p.DVpulse_count == video_p.locusV.point_pulse_b + video_p.locusV.ram_dist)
				{
					if(video_p.locusV.dir == B_TO_A)
					{
						video_p.locusV.p_upORdown = SLOW_STOP;
					}
				}
			}
			else if(video_p.locusV.point_pulse_a < video_p.locusV.point_pulse_b)
			{
				if(motorVV_p.DVpulse_count == video_p.locusV.point_pulse_a + video_p.locusV.ram_dist)
				{
					if(video_p.locusV.dir == A_TO_B)
					{
						video_p.locusV.p_upORdown = SLOW_STOP;
					}
				}
				else if(motorVV_p.DVpulse_count == video_p.locusV.point_pulse_b - video_p.locusV.ram_dist)
				{
					if(video_p.locusV.dir == B_TO_A)
					{
						video_p.locusV.p_upORdown = SLOW_STOP;
					}
				}
			}
		}*/
	}
}

// AB 点设置不得超过360 度
void limit_angle_360(void)
{
		if(mode_backup == VIDEO_MODE)
		{	
			if(con_b.vvAB_set)
			{
				if(con_b.vvAB_set > 2)return;
				if((motorHH_p.DVpulse_count >= B_POINT_END))
				{
					if(motorHH_p.DIR == A_TO_B)motorHH_stop();
				}
				else if(motorHH_p.DVpulse_count <= A_POINT_END)
				{
					if(motorHH_p.DIR == B_TO_A)motorHH_stop();
				}

				if((motorVV_p.DVpulse_count >= B_POINT_END))
				{
					if(motorVV_p.DIR == A_TO_B)motorVV_stop();
				}
				else if(motorVV_p.DVpulse_count <= A_POINT_END)
				{
					if(motorVV_p.DIR == B_TO_A)motorVV_stop();
				}
			}
		}		
		else if(mode_backup == DELAY_SET)
		{
			if(delay_p.ab_set_if)
			{
				if(delay_p.ab_set_if > 2)return;
				
				if((motorHH_p.DLLpulse_count >= B_POINT_END))
				{
					if(motorHH_p.DIR == A_TO_B)motorHH_stop();
				}
				else if(motorHH_p.DLLpulse_count <= A_POINT_END)
				{
					if(motorHH_p.DIR == B_TO_A)motorHH_stop();
				}

				if((motorVV_p.DLLpulse_count >= B_POINT_END))
				{
					if(motorVV_p.DIR == A_TO_B)motorVV_stop();
				}
				else if(motorVV_p.DLLpulse_count <= A_POINT_END)
				{
					if(motorVV_p.DIR == B_TO_A)motorVV_stop();
				}
			}
		}
	
}

void Video_loop_check(void)
{
	if(mode_backup != VIDEO_MODE)return;
	video_motorHH_pulse_check();
	Video_slow_pulse_check(MOTOR_HORITAL);
	Video_FindABpoint_pluse_check(MOTOR_HORITAL);
	Video_FindABpoint_slow_check(MOTOR_HORITAL);
	Video_find_Origin_checkStop(MOTOR_HORITAL);
	//Video_find_Origin_slow_check(MOTOR_HORITAL);
	Video_compensation_change_speed(MOTOR_HORITAL);
}

// 视频模式找AB点时通知遥控器显示给用户看
void Video_Find_ABpoint_notify(uint8_t dis_type)
{
	uint8_t buffers[5]={0};

	buffers[0] = 0x09;
	buffers[1] = 0x0d;
	buffers[2] = dis_type; // 0: 已找到A/B点；1: 正在找A点; 2: 正在找B点
	buffers[3] = check_sum_add(3, buffers);
	Package_dataBufer(4, buffers);
}

