
#include "common.h"
#include "gpio.h"
#include "motor.h"

#define FOCU_STOP_TIME		200

uint32_t move_time_bakeup=0;
uint32_t exposures_t = 0;
uint32_t sys_stop_t = 0;
uint32_t focu_sopt = 0;
uint8_t minutes =0;
uint8_t seconds =0;
uint8_t hours =0;
uint8_t countdown=0;


DELAY_PARA delay_p = {0};

void delay_mode_para_clear(void)
{
	delay_p.ab_set_if = 0;
	delay_p.locuHH.pulse = 0;
	delay_p.locuVV.pulse = 0;
}

//计算两轴的最大可用张数，取用较小的那个
void Delay_Max_amount_check(void)
{
	uint32_t h_mount=0;
	uint32_t v_mount=0;

	h_mount = delay_p.locuHH.pulse / 10;
	v_mount = delay_p.locuVV.pulse / 10;

	if(h_mount > v_mount)
	{
		delay_p.Max_amount = v_mount;
	}
	else
	{
		delay_p.Max_amount = h_mount;
	}
}

void Delay_total_time_calculate(void)
{
	uint32_t tol_time = 0;
	uint8_t bufer[10] ={ 0};

	delay_p.all_times = delay_p.interval*1000 + delay_p.exposure*1000 + delay_p.move_times + FOCU_STOP_TIME;
	tol_time = delay_p.all_times * (delay_p.amount)-delay_p.move_times;
	if(tol_time % 1000)
	{
		tol_time /= 1000 + 1;
	}
	else
	{
		tol_time /= 1000;
	}
	//delay_p.all_times = tol_time*1000;
//	tol_time = delay_p.interval + delay_p + delay_p.move_times / 200; //delay_p.interval * delay_p.amount;

	delay_p.total_time_h = tol_time / 3600;
	delay_p.total_time_m = (tol_time - delay_p.total_time_h * 3600) / 60;
	delay_p.total_time_s = tol_time - delay_p.total_time_h * 3600 - delay_p.total_time_m * 60;

	bufer[0] = 0x09;
	bufer[1] = 0x06;
	bufer[2] = 0x09;
	bufer[3] = 0;
	bufer[4] = delay_p.total_time_s;
	bufer[5] = delay_p.total_time_m;
	bufer[6] = delay_p.total_time_h;
	bufer[7] = check_sum_add(7, bufer);

	Package_dataBufer(8, bufer);
}

// 计算每张照移动的速度，两个轴的速度要非常接近才能同步
void Delay_move_speed_calculate(uint8_t count_types)
{
	uint32_t Hmove_times = 0;
//	uint32_t move_times = 0;
//	uint16_t int_time = 0;
	uint8_t sendt = 0;

	if(count_types)
	{
		if(delay_p.locuHH.pulse) // 如果水平轴有轨迹就以水平轴的为主，V 轴为次主
		{
			delay_p.locuHH.remainder_p = (double)delay_p.locuHH.pulse / (double)delay_p.amount;
			delay_p.locuHH.move_pulse = delay_p.locuHH.remainder_p;
			
			Hmove_times = (float)((DELAY_SPEED + 1) * (float)501 / (float)48000) * (float)delay_p.locuHH.move_pulse;
			delay_p.locuHH.speed = DELAY_SPEED;
			delay_p.move_times = Hmove_times;
		}

		// 计算间隔时间
		/*int_time = delay_p.move_times + delay_p.exposure*1000 + 1000 + 200;
		if(int_time > (delay_p.interval*1000))
		{
			sendt = 1;
			if((int_time % 1000) > 10)
			{
				delay_p.interval = int_time / 1000 + 1;
			}
			else
			{
				delay_p.interval = int_time / 1000;
			}
		}*/
	}
	else
	{
		/*int_time = delay_p.move_times + delay_p.exposure*1000 + 1000 + 200;
		if(int_time > (delay_p.interval*1000))
		{
			sendt = 1;
			if((int_time % 1000) > 10)
			{
				delay_p.interval = int_time / 1000 + 1;
			}
			else
			{
				delay_p.interval = int_time / 1000;
			}
		}*/
	}

	if(sendt)
	{
		Send_connect_data_to_controller();
	}
	
	Delay_total_time_calculate();
}

// 延时模式启动涵数
void delay_mode_start(void)
{
	uint8_t hhdir_check = 0;
//	uint8_t vvdir_check = 0;
	
	if(mode_backup != DELAY_SET)return;
	if(delay_p.m_start)
	{
		if(move_begin_backup)
		{
			move_begin = move_begin_backup;
			if(move_begin == 1)
			{
				motorHH_start();
				//motorVV_start();
				delay_p.locuHH.begins = 1;
				delay_p.locuVV.begins = 1;
			}
			else
			{
			
			}
		}
		else
		{
			delay_move_para_get();
			hhdir_check = Delay_MotorHH_Start_check_diretion();
			//vvdir_check = Delay_MotorVV_Start_check_diretion();
			if(hhdir_check==0)
			{
				p_amount = 0;
//				SHUTTER_ON;
//				BLE_shut_start();
				move_begin = 2;
				countdown = 1;
			}
		}
	}
	else
	{
		SHUTTER_OFF;
		motorHH_stop();
		motorVV_stop();
		move_begin_backup = move_begin;
		move_begin = 0;
	}
}

void Delay_get_data_from_controller(uint8_t *fifos)
{
	if(fifos[4] == 0x01) // interval times
	{
		delay_p.interval = (uint16_t)fifos[5] | (uint16_t)fifos[6]<<8;
		Delay_move_speed_calculate(0);
		sys_stop_t = delay_p.interval*1000;
	}
	else if(fifos[4] == 0x02) // exposure time
	{
		delay_p.exposure = (uint16_t)fifos[5] | (uint16_t)fifos[6]<<8;
		if(delay_p.exposure > 990)delay_p.exposure = 990;
		else if(delay_p.exposure < 1)delay_p.exposure = 1;
		Delay_move_speed_calculate(0);
		
	}
	else if(fifos[4] == 0x03) // 张数
	{
		delay_p.amount = (uint16_t)fifos[5] | (uint16_t)fifos[6]<<8;
		Delay_move_speed_calculate(1);
	}
	else if(fifos[4] == 0x06) // start or stop
	{
		delay_p.m_start = fifos[5];
		delay_mode_start();
	}
	else if(fifos[4] == 0x05) // dir
	{
		if(video_p.Pause)return;
		delay_p.locuHH.dir = fifos[5];
		delay_p.locuVV.dir = fifos[5];
		Delay_mode_Dir_check(MOTOR_Vertical);
		Delay_mode_Dir_check(MOTOR_HORITAL);
	}
}

void delay_move_para_get(void)
{
	exposures_t = delay_p.exposure * 1000;
	sys_stop_t = delay_p.interval*1000;
	
	focu_sopt = FOCU_STOP_TIME; //delay_p.all_times - exposures_t - delay_p.move_times - sys_stop_t;

	seconds = delay_p.total_time_s;
	minutes = delay_p.total_time_m;
	hours = delay_p.total_time_h;
}

void Delay_set_abPoint(uint8_t typess)
{
	uint32_t pulse_h=0;
	uint32_t pulse_v=0;
	
	delay_p.ab_set_if = typess;

	if(delay_p.ab_set_if == 1)
	{
		delay_p.locuHH.or_point = motorHH_p.pulse_count;
		delay_p.locuVV.or_point = motorVV_p.pulse_count;

		delay_p.locuHH.point_a = ORIGIN_POINT;
		motorHH_p.DLLpulse_count = ORIGIN_POINT;

		delay_p.locuVV.point_a = ORIGIN_POINT;
		motorVV_p.DLLpulse_count = ORIGIN_POINT;
	}
	else if(delay_p.ab_set_if == 2)
	{
		motorHH_stop();
		motorVV_stop();
		pulse_h = Absolute_value_calculation(delay_p.locuHH.point_a, motorHH_p.DLLpulse_count);
		pulse_v = Absolute_value_calculation(delay_p.locuVV.point_a, motorVV_p.DLLpulse_count);

		if(pulse_h < (uint32_t)PULSE_SUM(1) && pulse_v < (uint32_t)PULSE_SUM(1))
		{
			delay_p.ab_set_if = 0;
			delay_p.locuHH.pulse = 0;
			delay_p.locuVV.pulse = 0;
		}
		else
		{
			delay_p.ab_set_if = 3;
			
			if(pulse_h > (uint32_t)PULSE_SUM(1))
			{
				delay_p.locuHH.point_b = motorHH_p.DLLpulse_count;
				delay_p.locuHH.pulse = pulse_h;
			}

			if(pulse_v > (uint32_t)PULSE_SUM(1))
			{
				delay_p.locuVV.point_b = motorVV_p.DLLpulse_count;
				delay_p.locuVV.pulse = pulse_v;
			}

			delay_p.amount = 180;
			delay_p.exposure = 1;
			delay_p.interval = 1;
			
			delay_p.DIR = B_TO_A;
			delay_p.locuHH.dir = delay_p.DIR;
			delay_p.locuVV.dir = delay_p.DIR;
			Delay_mode_Dir_check(MOTOR_HORITAL);
			Delay_mode_Dir_check(MOTOR_Vertical);
			
			Delay_Max_amount_check();
			//Delay_move_speed_calculate(0);
			Delay_move_speed_calculate(1);
			Delay_total_time_calculate();
		}
		
		Send_para_data_to_controller(1, 1);
	}
}

void delay_mode_main(void)
{
	if(mode_backup != DELAY_SET)return;
	if(delay_p.m_start == 0)return;
	if(move_begin == 0)return;

	if(move_begin == 2)
	{
		if(p_move_time >= sys_stop_t) //消抖一秒开快门
		{
			countdown = 0;
			SHUTTER_ON;
			BLE_shut_start();
			Red_led_tack();
			move_begin = 3;
			p_move_time = 0;
			countdown = 1;
		}
	}
	else if(move_begin == 3)
	{
		if(p_move_time >= exposures_t)
		{
			countdown = 0;
			SHUTTER_OFF;
			//Panorama_mode_compensation();
			p_amount += 1;
			//con_b.p_amount_back += 1;
			delay_mode_countdwon_display();
			move_begin = 4;
			p_move_time = 0;
			countdown = 1;
		}
	}
	else if(move_begin == 4)
	{
		if(p_move_time >= focu_sopt)
		{
			//String_Printf(USART_2,"AA",2);
			//String_Printf(USART_2,(uint8_t * )&focu_sopt,4);
			
			if(p_amount >= delay_p.amount) // 所有照片全部拍完，结束
			{
				motorHH_stop();
				//motorVV_stop();
				countdown = 0;
				move_begin = 0;
				delay_p.m_start = 0;
				delay_mode_countdwon_display();
				move_begin_backup = 0;
				delay_p.DIR = ~delay_p.DIR;
				delay_p.locuHH.dir = delay_p.DIR;
				//delay_p.locuVV.dir = delay_p.DIR;
				Delay_mode_Dir_check(MOTOR_Vertical);
				//Delay_mode_Dir_check(MOTOR_HORITAL);
			}
			else
			{
				countdown = 0;
				p_move_time = 0;
				motorHH_speed_set(delay_p.locuHH.speed);
			//	motorVV_speed_set(delay_p.locuVV.speed);
				delay_p.locuHH.begins = 1;
				//delay_p.locuVV.begins = 1;
				move_begin = 1;
				p_move_time = 0;
				countdown = 1;
				motorHH_start();
				//motorVV_start();
			}
		}
	}
}

void delay_mode_countdwon_display(void)
{
	uint8_t bufers[10]={0};

	bufers[0] = 0x09;
	bufers[1] = 0x07;
	bufers[2] = 0x0a;
	
	bufers[3] = seconds;
	bufers[4] = minutes;
	bufers[5] = hours;
	bufers[6] = p_amount;
	bufers[7] = p_amount>>8;
	bufers[8] = delay_p.m_start;
	bufers[9] = check_sum_add(9, bufers);
	Package_dataBufer(10, bufers);
}

void delay_mode_time_countdwon(void)
{
	if(mode_backup != DELAY_SET)return;
	if(move_begin==0)return;
	if(seconds==0 && minutes==0 && hours==0)return;
	if(countdown == 0)return;
	
	con_b.MS_count++;
	if(con_b.MS_count >= 1000)
	{
		con_b.MS_count = 0;
		if(seconds)
		{
			seconds -= 1;
		}
		else
		{
			if(minutes)
			{
				minutes -=1;
				seconds = 59;
			}
			else
			{
				if(hours)
				{
					hours -=1;
					minutes = 59;
					seconds = 59;
				}
			}
		}

	//	if(mode_backup == DELAY_MODE)
		{
			delay_mode_countdwon_display();
		}
	}
}


// 进入延时模式要自动找A 点
void Delay_mode_find_Apoint(void)
{	
	if(mode_backup != DELAY_SET)return;

	if(delay_p.locuVV.pulse)
	{
		if(motorVV_p.pulse_count != delay_p.locuVV.or_point)
		{
			if(motorVV_p.pulse_count > delay_p.locuVV.or_point)
			{
				if((motorVV_p.pulse_count - delay_p.locuVV.or_point) > (M_POINT_END-ORIGIN_POINT))
				{
					motorVV_p.DIR = A_TO_B;
				}
				else
				{
					motorVV_p.DIR = B_TO_A;
				}
			}
			else if(motorVV_p.pulse_count < delay_p.locuVV.or_point)
			{
				if((delay_p.locuVV.or_point - motorVV_p.pulse_count) > (M_POINT_END-ORIGIN_POINT))
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

	if(delay_p.locuHH.pulse)
	{
		if(motorHH_p.pulse_count != delay_p.locuHH.or_point)
		{
			if(motorHH_p.pulse_count > delay_p.locuHH.or_point)
			{
				if((motorHH_p.pulse_count - delay_p.locuHH.or_point) > (M_POINT_END-ORIGIN_POINT))
				{
					motorHH_p.DIR = A_TO_B;
				}
				else
				{
					motorHH_p.DIR = B_TO_A;
				}
			}
			else if(motorHH_p.pulse_count < delay_p.locuHH.or_point)
			{
				if((delay_p.locuHH.or_point - motorHH_p.pulse_count) > (M_POINT_END-ORIGIN_POINT))
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
}

void Delay_mode_Dir_check(MOTOR_TYPE motor_t)
{
	if(mode_backup != DELAY_SET)return;

	if(motor_t == MOTOR_HORITAL)
	{
		if(delay_p.locuHH.dir == A_TO_B)
		{
			motorHH_p.DIR = (delay_p.locuHH.point_a > delay_p.locuHH.point_b)?B_TO_A : A_TO_B;
		}
		else
		{
			motorHH_p.DIR = (delay_p.locuHH.point_a > delay_p.locuHH.point_b)?A_TO_B : B_TO_A;
			//glob_para.orbital_dir = ~glob_para.orbital_dir;
		}

		motorHH_direction_change(motorHH_p.DIR);
	}
	/*else if(motor_t == MOTOR_Vertical)
	{
		if(delay_p.locuVV.dir == A_TO_B)
		{
			motorVV_p.DIR = (delay_p.locuVV.point_a > delay_p.locuVV.point_b)?B_TO_A : A_TO_B;
		}
		else
		{
			motorVV_p.DIR = (delay_p.locuVV.point_a > delay_p.locuVV.point_b)?A_TO_B : B_TO_A;
			//glob_para.orbital_dir = ~glob_para.orbital_dir;
		}

		motorVV_direction_change(motorVV_p.DIR);
	}*/
}


// 延时模式自动找原点到达后的停止
void Delay_find_Origin_checkStop(MOTOR_TYPE motor_t)
{
//	if(mode_backup != DELAY_SET)return;
	
	if(motor_t == MOTOR_HORITAL)
	{
		if(delay_p.locuHH.check_dir)return;
		if(find_pata.HHfind_Apoint == 0)return;
		if(motorHH_p.pulse_count == delay_p.locuHH.or_point)
		{
			find_pata.HHfind_Apoint = 0;
			find_pata.slow_pulse = 0;
			find_pata.find_abpoint = 0;
			//if(find_pataVV.HHfind_Apoint == 0)find_pata.find_abpoint = 0;
			motorHH_stop();
			motorHH_p.DLLpulse_count = delay_p.locuHH.point_a;
			delay_p.DIR = A_TO_B;
			delay_p.locuHH.dir = delay_p.DIR;
			//video_p.locusV.dir = video_p.DIR;
			Delay_mode_Dir_check(MOTOR_HORITAL);
			Video_Find_ABpoint_notify(REACHED_ORIGIN_POINT);
		}
	}
	
}

void Delay_find_Origin_slow_check(MOTOR_TYPE motor_t)
{
	if(mode_backup != DELAY_SET)return;
	
	if(motor_t == MOTOR_HORITAL)
	{
		if(delay_p.locuHH.check_dir)return;
		if(find_pata.HHfind_Apoint == 0)return;
		if(delay_p.locuHH.point_a==0)return;

		if(motorHH_p.DIR == A_TO_B)
		{
			if(motorHH_p.pulse_count >= delay_p.locuHH.or_point - find_pata.slow_pulse)
			{
				find_pata.find_abpoint = SLOW_STOP;
			}
		}
		else
		{
			if(motorHH_p.pulse_count <= delay_p.locuHH.or_point + find_pata.slow_pulse)
			{
				find_pata.find_abpoint = SLOW_STOP;
			}
		}
	}
	else if(motor_t == MOTOR_Vertical)
	{
		if(delay_p.locuVV.check_dir)return;
		if(find_pataVV.HHfind_Apoint == 0)return;
		if(delay_p.locuVV.point_a==0)return;

		if(motorVV_p.DIR == A_TO_B)
		{
			if(motorVV_p.pulse_count >= delay_p.locuVV.or_point - find_pataVV.slow_pulse)
			{
				find_pataVV.find_abpoint = SLOW_STOP;
			}
		}
		else
		{
			if(motorVV_p.pulse_count <= delay_p.locuVV.or_point + find_pataVV.slow_pulse)
			{
				find_pataVV.find_abpoint = SLOW_STOP;
			}
		}
	}
}


/////////////////////// motor HH  //////////////////////////////////////
//延时模式
void Delay_motorHH_move_time_compara(void)
{
	static uint32_t pulse_add = 0;
	if(mode_backup != DELAY_SET)return;
	if(delay_p.locuHH.begins != 1)return;
	if(delay_p.locuHH.check_dir)return;
	if(delay_p.locuVV.check_dir)return;
	if(find_pata.HHfind_Apoint)return;
	
	pulse_add += 1;
	if(pulse_add >= delay_p.locuHH.move_pulse)
	{
		motorHH_stop();
		pulse_add = 0;
		delay_p.locuHH.begins = 0;
		//if(delay_p.locuVV.begins == 0)
		{
			move_begin = 2;
			p_move_time = 0;
		}
		
	}
}

void Delay_mode_para_clear(void)
{
	delay_p.m_start = 0;
	delay_p.locuHH.begins = 0;
	delay_p.locuVV.begins = 0;
	find_pataVV.HHfind_Apoint= 0;
	find_pataVV.find_abpoint = 0;
	find_pata.HHfind_Apoint= 0;
	find_pata.find_abpoint = 0;
	move_begin = 0;
	move_begin_backup = 0;
	delay_p.locuVV.check_dir= 0;
	delay_p.locuVV.Ram_UpRoDown = 0;
}

/////////////////////// motor HH  //////////////////////////////////////
//延时模式
void Delay_motorVV_move_time_compara(void)
{
	static uint32_t pulse_addVV = 0;
	if(mode_backup != DELAY_SET)return;
	if(delay_p.locuVV.begins != 1)return;
	if(delay_p.locuHH.check_dir)return;
	if(delay_p.locuVV.check_dir)return;
	if(find_pataVV.HHfind_Apoint)return;
	
	pulse_addVV += 1;
	if(pulse_addVV >= delay_p.locuVV.move_pulse)
	{
		motorVV_stop();
		pulse_addVV = 0;
		delay_p.locuVV.begins = 0;
		if(delay_p.locuHH.begins == 0)
		{
			move_begin = 2;
			p_move_time = 0;
		}
		
	}
}

// 启动时检测方向是否正确，不正确就先找A /B
uint8_t Delay_MotorHH_Start_check_diretion(void)
{
	uint8_t temp = 0;
	if(mode_backup != DELAY_SET)return 0;
	if(delay_p.locuHH.pulse==0)return 0;
	
	if(delay_p.locuHH.dir == A_TO_B)
	{
		if(motorHH_p.DLLpulse_count == delay_p.locuHH.point_a)return 0;
		if(motorHH_p.DLLpulse_count > delay_p.locuHH.point_a)
		{
			motorHH_p.DIR = B_TO_A;
			temp = LOOKING_A_POINT;
		}
		else if(motorHH_p.DLLpulse_count < delay_p.locuHH.point_a)
		{
			motorHH_p.DIR = A_TO_B;
			temp = LOOKING_A_POINT;
		}
	}
	else if(delay_p.locuHH.dir == B_TO_A )
	{
		if(motorHH_p.DLLpulse_count == delay_p.locuHH.point_b)return 0;
		if(motorHH_p.DLLpulse_count > delay_p.locuHH.point_b)
		{
			motorHH_p.DIR = B_TO_A;
			temp = LOOKING_B_POINT;
		}
		else if(motorHH_p.DLLpulse_count < delay_p.locuHH.point_b)
		{
			motorHH_p.DIR = A_TO_B;
			temp = LOOKING_B_POINT;
		}
	}

	if(temp)
	{
		delay_p.locuHH.check_dir= 1;
		//Delay_mode_Dir_check(MOTOR_HORITAL);
		motorHH_direction_change(motorHH_p.DIR);
		set_abpoint_start_setting(MOTOR_HORITAL);
		Red_led_tack();
		Video_Find_ABpoint_notify(temp);
		/*DelayMode_ram_load( MOTOR_HORITAL);
		delay_p.locuHH.ramspeed = delay_p.locuHH.ram_id;
		motorHH_speed_set(delay_p.locuHH.ramspeed );
		delay_p.locuHH.Ram_UpRoDown = SLOW_START;
		motorHH_start();*/
	}

	return temp;
}

// 启动时检测方向是否正确，不正确就先找A /B
uint8_t Delay_MotorVV_Start_check_diretion(void)
{
	uint8_t temp = 0;
	if(mode_backup != DELAY_SET)return 0;
	if(delay_p.locuVV.pulse==0)return 0;
	
	if(delay_p.locuVV.dir == A_TO_B)
	{
		if(motorVV_p.DLLpulse_count == delay_p.locuVV.point_a)return 0;
		if(motorVV_p.DLLpulse_count > delay_p.locuVV.point_a)
		{
			motorVV_p.DIR = B_TO_A;
			temp = 1;
		}
		else if(motorVV_p.DLLpulse_count < delay_p.locuVV.point_a)
		{
			motorVV_p.DIR = A_TO_B;
			temp = 1;
		}
	}
	else if(delay_p.locuVV.dir == B_TO_A )
	{
		if(motorVV_p.DLLpulse_count == delay_p.locuVV.point_b)return 0;
		if(motorVV_p.DLLpulse_count > delay_p.locuVV.point_b)
		{
			motorVV_p.DIR = B_TO_A;
			temp = 1;
		}
		else if(motorVV_p.DLLpulse_count < delay_p.locuVV.point_b)
		{
			motorVV_p.DIR = A_TO_B;
			temp = 1;
		}
	}

	if(temp)
	{
		delay_p.locuVV.check_dir= 1;
		//Delay_mode_Dir_check(MOTOR_Vertical);
		motorVV_direction_change(motorVV_p.DIR);
		set_abpoint_start_setting(MOTOR_Vertical);
		Red_led_tack();
		/*DelayMode_ram_load( MOTOR_Vertical);
		delay_p.locuVV.ramspeed = delay_p.locuVV.ram_id;
		motorVV_speed_set(delay_p.locuVV.ramspeed );
		delay_p.locuVV.Ram_UpRoDown = SLOW_START;
		motorVV_start();*/
	}

	return temp;
}

void DelayMode_ram_load(MOTOR_TYPE motor_tt)
{
	uint8_t i;
	uint16_t tampV=0;
	uint16_t tampH=0;
	if(mode_backup != DELAY_SET)return;

	if(motor_tt) // motor H  水平轴
	{
		delay_p.locuHH.ram_dist = 0;
		//delay_p.locuHH.ram_buffer[0]=speedV;
		
		for(i=BASE_SPEED; i<RAM_ID+1;i++)
		{
			tampH =  i;
			delay_p.locuHH.ram_dist += (float)SLOW_BASIC_TIME22 / ((float)(tampH + 1) * 501.0 / 48000.0);
		}
		
	//	delay_p.locuHH.ram_time = SLOW_BASIC_TIME11 * (RAM_ID-1) * 2;
		
		delay_p.locuHH.ram_id = RAM_ID;
	}
	else
	{
		delay_p.locuVV.ram_dist = 0;
		
		for(i=BASE_SPEED; i<RAM_ID+1;i++)
		{
			tampV =  i;
			delay_p.locuVV.ram_dist += (float)SLOW_BASIC_TIME22 / ((float)(tampV + 1) * 501.0 / 48000.0);
		}
		delay_p.locuVV.ram_id = RAM_ID;
	}
}


//标准模式缓启停处理涵数
void DelayMode_slowly_startedORstop(MOTOR_TYPE motor_t)
{
//	if(ramp_flag == 0)return;
	if(mode_backup != DELAY_SET)return;
	if(motor_t == MOTOR_HORITAL)
	{
		if(delay_p.locuHH.check_dir == 0)return;
		if(delay_p.locuHH.Ram_UpRoDown==0)return;
		//if(p_upORdown == check_speed_tag)return;
		if(delay_p.locuHH.ramTime)return;

		if(delay_p.locuHH.Ram_UpRoDown == SLOW_STOP)
		{
			//motor_speed_set(dynamic_speed);
			
			if(delay_p.locuHH.ramspeed < delay_p.locuHH.ram_id)
			{
				delay_p.locuHH.ramspeed++;
				motorHH_speed_set(delay_p.locuHH.ramspeed);
				//motorVV_speed_set(con_b.ramp_buffer[standard_p.HH_speed]);
			}
			else 
			{
				delay_p.locuHH.Ram_UpRoDown = 0;
				
			//String_Printf(USART_2, (uint8_t *)&glob_para.pulse_cout, 4);
			//	time_delay_us(10000);
				//String_Printf(USART_2, (uint8_t *)&glob_para.pulse_cout11, 4);
				
			}
		}
		else if(delay_p.locuHH.Ram_UpRoDown == SLOW_START)
		{
			//motor_speed_set(dynamic_speed);
			
			if(delay_p.locuHH.ramspeed > BASE_SPEED)
			{
				delay_p.locuHH.ramspeed--;
				motorHH_speed_set(delay_p.locuHH.ramspeed);
				//motorVV_speed_set(con_b.ramp_buffer[standard_p.HH_speed]);
				//String_Printf(USART_2, (uint8_t *)&con_b.ramp_buffer[dynamic_speed], 2);
			}
			else 
			{
				delay_p.locuHH.Ram_UpRoDown = 0;
			//	String_Printf(USART_2, (uint8_t *)&con_b.Dynamic_pulse, 4);
				//String_Printf(USART_2, (uint8_t *)&con_b.slow_point, 4);
			}
		}
		
		delay_p.locuHH.ramTime = SLOW_BASIC_TIME22;
	}
	else
	{
		if(delay_p.locuVV.check_dir == 0)return;
		if(delay_p.locuVV.Ram_UpRoDown==0)return;
		//if(p_upORdown == check_speed_tag)return;
		if(delay_p.locuVV.ramTime)return;

		if(delay_p.locuVV.Ram_UpRoDown == SLOW_STOP)
		{
			//motor_speed_set(dynamic_speed);
			
			if(delay_p.locuVV.ramspeed < delay_p.locuVV.ram_id)
			{
				delay_p.locuVV.ramspeed++;
				motorVV_speed_set(delay_p.locuVV.ramspeed);
				//motorVV_speed_set(con_b.ramp_buffer[standard_p.HH_speed]);
			}
			else 
			{
				delay_p.locuVV.Ram_UpRoDown = 0;
				
			//String_Printf(USART_2, (uint8_t *)&glob_para.pulse_cout, 4);
			//	time_delay_us(10000);
				//String_Printf(USART_2, (uint8_t *)&glob_para.pulse_cout11, 4);
				
			}
		}
		else if(delay_p.locuVV.Ram_UpRoDown == SLOW_START)
		{
			//motor_speed_set(dynamic_speed);
			
			if(delay_p.locuVV.ramspeed > BASE_SPEED)
			{
				delay_p.locuVV.ramspeed--;
				motorVV_speed_set(delay_p.locuVV.ramspeed);
				//motorVV_speed_set(con_b.ramp_buffer[standard_p.HH_speed]);
				//String_Printf(USART_2, (uint8_t *)&con_b.ramp_buffer[dynamic_speed], 2);
			}
			else 
			{
				delay_p.locuVV.Ram_UpRoDown = 0;
			//	String_Printf(USART_2, (uint8_t *)&con_b.Dynamic_pulse, 4);
				//String_Printf(USART_2, (uint8_t *)&con_b.slow_point, 4);
			}
		}
		
		delay_p.locuVV.ramTime = SLOW_BASIC_TIME22;
	}
}


// 启动前找AB 点到达后的自动启动
void DelayMode_FindABpoint_End(void)
{
	if(mode_backup != DELAY_SET)return;
	if((delay_p.locuHH.check_dir ==2 && delay_p.locuVV.check_dir==2))
	{
		if(delay_p.activate)return;		
		//MotorVV_Video_move_time_check();
		//MotorHH_Video_move_time_check();
		//Video_start();
		Delay_mode_Dir_check(MOTOR_Vertical);
		Delay_mode_Dir_check(MOTOR_HORITAL);
		delay_p.locuHH.check_dir = 0;
		delay_p.locuVV.check_dir = 0;
		p_amount = 0;
		//SHUTTER_ON;
		//BLE_shut_start();
		move_begin = 2;
		countdown = 1;
	}
}

// 找A/B 点到达后的停止处理涵数
void Delay_FindABpoint_pluse_check(MOTOR_TYPE motor_tt)
{
//	if(mode_backup != DELAY_SET)return;
	if(motor_tt == MOTOR_HORITAL)
	{
		if(delay_p.locuHH.check_dir==0)return;

		if(delay_p.locuHH.dir == A_TO_B) // 实时是按B TO A 找A点
		{
			if(motorHH_p.DLLpulse_count == delay_p.locuHH.point_a)
			{
				motorHH_stop();
				find_pata.HHfind_Apoint = 0;
				find_pata.slow_pulse = 0;
				find_pata.find_abpoint = 0;
				delay_p.locuHH.check_dir = 2;
				if(delay_p.locuVV.check_dir == 2 || delay_p.locuVV.check_dir==0)
				{
					delay_p.activate = 1000;
					delay_p.locuVV.check_dir=2;
				}
				Video_Find_ABpoint_notify(REACHED_ABPOINT);
			}
		}
		else if(delay_p.locuHH.dir == B_TO_A)
		{
			if(motorHH_p.DLLpulse_count == delay_p.locuHH.point_b)
			{
				motorHH_stop();
				find_pata.HHfind_Apoint = 0;
				find_pata.slow_pulse = 0;
				find_pata.find_abpoint = 0;
				delay_p.locuHH.check_dir = 2;
				if(delay_p.locuVV.check_dir == 2 || delay_p.locuVV.check_dir==0)
				{
					delay_p.activate = 1000;
					delay_p.locuVV.check_dir=2;
				}
				Video_Find_ABpoint_notify(REACHED_ABPOINT);
			}
		}
	}
}

//启动时检测到方向不对就启动找点后的缓停动作
void Delay_FindABpoint_slow_check(MOTOR_TYPE motor_t)
{
//	if(mode_backup != DELAY_SET)return;
	
	if(motor_t == MOTOR_HORITAL)
	{
		if(delay_p.locuHH.pulse== 0)return;
		if(delay_p.locuHH.check_dir==0)return;
		if(find_pata.HHfind_Apoint==0)return;
				if(delay_p.locuHH.dir == A_TO_B) // 回到A点再开始
				{
					if(motorHH_p.DIR == A_TO_B)
					{
						if(motorHH_p.DLLpulse_count == (delay_p.locuHH.point_a - find_pata.slow_pulse))
						{
							find_pata.find_abpoint = SLOW_STOP;
						}
					}
					else
					{
						if(motorHH_p.DLLpulse_count == (delay_p.locuHH.point_a + find_pata.slow_pulse))
						{
							find_pata.find_abpoint = SLOW_STOP;
						}
					}
				}
				else
				{
					if(motorHH_p.DIR == A_TO_B)
					{
						if(motorHH_p.DLLpulse_count == (delay_p.locuHH.point_b - find_pata.slow_pulse))
						{
							find_pata.find_abpoint = SLOW_STOP;
						}
					}
					else
					{
						if(motorHH_p.DLLpulse_count == (delay_p.locuHH.point_b + find_pata.slow_pulse))
						{
							find_pata.find_abpoint = SLOW_STOP;
						}
					}
				}
	}	
}

// 缓停检测
void Delay_slow_pulse_check(MOTOR_TYPE motor_t)
{
	if(mode_backup != DELAY_SET)return;
	//if(find_pata.VVfind_Apoint)return;
	
	if(motor_t == MOTOR_HORITAL)
	{
		if(delay_p.locuHH.check_dir==0)return;
		//if(video_p.locusH.pluses == 0)return;
	//	if(delay_p.locuHH.ram_if)
		{
			if(delay_p.locuHH.point_a > delay_p.locuHH.point_b)
			{
				if(motorHH_p.DLLpulse_count == delay_p.locuHH.point_a - delay_p.locuHH.ram_dist)
				{
					if(delay_p.locuHH.dir == B_TO_A)
					{
						delay_p.locuHH.Ram_UpRoDown = SLOW_STOP;
					}
				}
				else if(motorHH_p.DLLpulse_count == delay_p.locuHH.point_b + delay_p.locuHH.ram_dist)
				{
					if(delay_p.locuHH.dir == A_TO_B)
					{
						delay_p.locuHH.Ram_UpRoDown = SLOW_STOP;
					}
				}
			}
			else if(delay_p.locuHH.point_a < delay_p.locuHH.point_b)
			{
				if(motorHH_p.DLLpulse_count == delay_p.locuHH.point_a + delay_p.locuHH.ram_dist)
				{
					if(delay_p.locuHH.dir == B_TO_A)
					{
						delay_p.locuHH.Ram_UpRoDown = SLOW_STOP;
					}
				}
				else if(motorHH_p.DLLpulse_count == delay_p.locuHH.point_b - delay_p.locuHH.ram_dist)
				{
					if(delay_p.locuHH.dir == A_TO_B)
					{
						delay_p.locuHH.Ram_UpRoDown = SLOW_STOP;
					}
				}
			}
		}
	}	
	else if(motor_t == MOTOR_Vertical)
	{
		if(delay_p.locuVV.check_dir==0)return;
		
	//	if(delay_p.locusV.ram_if)
		{
			if(delay_p.locuVV.point_a > delay_p.locuVV.point_b)
			{
				if(motorVV_p.DLLpulse_count == delay_p.locuVV.point_a - delay_p.locuVV.ram_dist)
				{
					if(delay_p.locuVV.dir == B_TO_A)
					{
						delay_p.locuVV.Ram_UpRoDown = SLOW_STOP;
					}
				}
				else if(motorVV_p.DLLpulse_count == delay_p.locuVV.point_b + delay_p.locuVV.ram_dist)
				{
					if(delay_p.locuVV.dir == A_TO_B)
					{
						delay_p.locuVV.Ram_UpRoDown = SLOW_STOP;
					}
				}
			}
			else if(delay_p.locuVV.point_a < delay_p.locuVV.point_b)
			{
				if(motorVV_p.DLLpulse_count == delay_p.locuVV.point_a + delay_p.locuVV.ram_dist)
				{
					if(delay_p.locuVV.dir == B_TO_A)
					{
						delay_p.locuVV.Ram_UpRoDown = SLOW_STOP;
					}
				}
				else if(motorVV_p.DLLpulse_count == delay_p.locuVV.point_b - delay_p.locuVV.ram_dist)
				{
					if(delay_p.locuVV.dir == A_TO_B)
					{
						delay_p.locuVV.Ram_UpRoDown = SLOW_STOP;
					}
				}
			}
		}
	}
}

void Timelapse_loop_check(void)
{
	if(mode_backup != DELAY_SET)return;
		Delay_find_Origin_checkStop(MOTOR_HORITAL);
		//Delay_find_Origin_slow_check(MOTOR_HORITAL);
		Delay_motorHH_move_time_compara();
		Delay_FindABpoint_pluse_check(MOTOR_HORITAL);
		Delay_FindABpoint_slow_check(MOTOR_HORITAL);
	//	Delay_slow_pulse_check(MOTOR_HORITAL);
}
///////////////////////MOTOR VV /////////////////////////////////////


