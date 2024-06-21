
#include "common.h" 
#include "motor_dri.h"
#include "gpio.h"

uint8_t remote_ramp = 0;
uint8_t shutter_tag = 0;
uint16_t shutter_time = 0;
uint8_t pressing = 0;

void Touch_The_shut(void)
{
	shutter_action(SHUT_ON);				
	shutter_tag = 1;
	shutter_time = 400;
}
	

void Set_pressing(uint8_t data)
{
	pressing = data;
}

void Ramp_Check(void)
{
	uint16_t speed_b = 0;

	speed_b = speed_calculat(glob_para.remode_speed);

	if(speed_b < SLOW_THRESHOLD && glob_para.Re_ramp)
	{
		remote_ramp = 0xff;
	}
	else
	{
		remote_ramp = 0;
		slider_upORdown(0);
	}
	
	Ramp_Speed_Load(speed_b);
}

uint8_t EndPoint_Check(uint8_t dir)
{
	if(glob_para.a_point > glob_para.b_point)
	{
		if(glob_para.pulse_cout >= glob_para.a_point)
		{
			if(dir == A_TO_B)
			{
				m_start = 0; // stop moving
				return 1;
			}
		}
		else if(glob_para.pulse_cout <= glob_para.b_point)
		{
			if(dir == B_TO_A)
			{
				m_start = 0; // stop moving
				return 1;
			}
		}
	}
	else if(glob_para.a_point < glob_para.b_point)
	{
		if(glob_para.pulse_cout <= glob_para.a_point)
		{
			if(dir == B_TO_A)
			{
				m_start = 0; // stop moving
				return 1;
			}
		}
		else if(glob_para.pulse_cout >= glob_para.b_point)
		{
			if(dir == A_TO_B)
			{
				m_start = 0; // stop moving
				return 1;
			}
		}
	}

	return 0;
}

void Remote_Usart_Get(uint8_t *sptb)
{
	if(mode_backup != REMOTE_PICTRURE_MODE && mode_backup != REMOTE_VIDEO_MODE)return;
	
	if(sptb[4] == 0x00) // speed
	{
		if(glob_para.remode_speed != sptb[5])
		{
			glob_para.remode_speed = sptb[5];
			Ramp_Check();
			if(m_start)motor_speed_adjust(glob_para.remode_speed);
			Send_Forecast_move_time();
			if_write_flash();
		}
		/*if(m_start)
		{
			if(abs(sptb[5], glob_para.remode_speed) > 1) //变速超过两档就需要慢加或慢减
			{
				if(check_speed_tag) // 正在调速加减进行中
				{
					if(jeidan_speed > speed_calculat(sptb[5]))
					{
						check_speed_tag = SLOW_START;
					}
					else
					{
						check_speed_tag = SLOW_STOP;
					}
				}
				else
				{
					jeidan_speed = speed_calculat(glob_para.remode_speed);
					if(sptb[5] > glob_para.remode_speed)
					{
						check_speed_tag = SLOW_START;
					}
					else
					{
						check_speed_tag = SLOW_STOP;
					}
				}
			}
			else
			{
				if(p_upORdown || check_speed_tag) // 缓起停地间中不给调速度
				{
					delay_send_pake(0);
					return;
				}
				check_speed_tag = 0;
				//dynamic_speed = 0;
				motor_speed_adjust(sptb[5]);
			}
			
			//glob_para.orbital_speed = app_read_buffer[5];
			//Video_move_speed_calculat();
		}
		
		glob_para.remode_speed = sptb[5];
		Ramp_Check();
		
		check_adjust_and_ramp(sptb[5]);
		
		if_write_flash();*/
	}
	else if(sptb[4] == 0x01) // direction
	{
		if(EndPoint_Check(sptb[5]))
		{
			mcu_set_data_to_apk(0x09, 1);
			return;
		}
		glob_para.slider_dir = sptb[5];
		glob_para.orbital_dir = glob_para.slider_dir;
		motor_direction_change(glob_para.orbital_dir);
		if(sptb[6] == 0xff) // press down
		{
			if(remote_ramp)
			{
				if(pressing == 0)
				{
					pressing = 0xff;
					dynamic_speed = SLOW_THRESHOLD;
					motor_speed_set(dynamic_speed);
				}
				
				slider_upORdown(SLOW_START);
			}
			else
			{
				motor_speed_adjust(glob_para.remode_speed);
			}
			
			motor_start();
			m_start = 0xff;
		}
		else if(sptb[6] == 0) // free
		{
			m_start = 0;
			if(remote_ramp)
			{
				slider_upORdown(SLOW_STOP);
			}
			else
			{
				motor_stop();
				pressing = 0;
			}
/*
// 当在慢速手动调慢后又立马按暂停并且在自动缓起停范围之后时的缓停动作的开始速度
// 即按暂停时要缓慢停下来的那个起始速度等于jeidan_speed
			if(check_speed_tag == SLOW_START)
			{
				if(p_upORdown == SLOW_STOP)
				{
					if(jeidan_speed < SLOW_THRESHOLD)
					{
						dynamic_speed = jeidan_speed;
					}
					else 
					{
						dynamic_speed = SLOW_THRESHOLD;
						slider_upORdown(0);
						motor_stop();
					}
					check_speed_tag = 0;
				}
				else
				{
					motor_stop();
				}
				
				check_speed_tag = 0;
			}*/
		}
	}
	else if(sptb[4] == 0x02) // ramp set
	{
		glob_para.Re_ramp = sptb[5];
		//String_Printf(USART_2, &glob_para.Re_ramp, 1);
		Ramp_Check();
		Send_Forecast_move_time();
	}
	else if(sptb[4] == 0x03) // start or stop
	{
		write_flash_active();
		/*if(con_b.remote_mode_if == REMOTE_VIDEO_MODE)
		{
			if(sptb[5] == 0xff) // 
			{
				shutter_action(SHUT_ON);
			}
			else
			{
				shutter_action(SHUT_OFF);
			}
		}
		else if(con_b.remote_mode_if == REMOTE_PICTRURE_MODE)*/
		{
			m_start = sptb[5];
			if(sptb[5] == 0xff)
			{
				Touch_The_shut();
			}
			else 
			{
				Touch_The_shut();
			}
		}
	}
	
}

void TurnOff_Shutter_If_TheTimeOver(void)
{
	if(shutter_tag)
	{
		if(shutter_time)return;
		shutter_action(SHUT_OFF);
		shutter_tag = 0;
	}
}

void Time_count_InRemoteMode(void)
{
	if(shutter_time)shutter_time--;
}

void Remote_mode_Eenpoint_Loop(void)
{
	//uint8_t temp = 0;
	if(mode_backup != REMOTE_PICTRURE_MODE && mode_backup != REMOTE_VIDEO_MODE)return;
	/*if(glob_para.a_point > glob_para.b_point)
	{
		if(glob_para.pulse_cout == glob_para.b_point)
		{
			temp = TURE;
			//glob_para.slider_dir = B_TO_A;
		}
		else if(glob_para.pulse_cout == glob_para.a_point)
		{
			temp = TURE;
			//glob_para.slider_dir = A_TO_B;
		}
	}
	else if(glob_para.a_point < glob_para.b_point)
	{
			if(glob_para.pulse_cout == glob_para.b_point)
			{
				temp = TURE;
				//glob_para.slider_dir = B_TO_A;
			}
			else if(glob_para.pulse_cout == glob_para.a_point)
			{
				temp = TURE;
				//glob_para.slider_dir = A_TO_B;
			}
	}*/
	if(glob_para.pulse_cout == glob_para.a_point || glob_para.pulse_cout == glob_para.b_point)
	{
		motor_stop();
		m_start = 0;
		pressing = 0;
		mcu_set_data_to_apk(0x09, 1);
	}
	
}

void Send_Forecast_move_time(void)
{
	uint32_t times = 0;
	times = Forecast_move_time_calculate(glob_para.remode_speed)/1000;
	con_b.r_expected_time_h = times / 3600;
	con_b.r_expected_time_m = times / 60 - con_b.r_expected_time_h * 60;
	con_b.r_expected_time_s = times - con_b.r_expected_time_h * 3600 - con_b.r_expected_time_m*60;

	mcu_set_data_to_apk(0x09,0x03);
}

