
#include "math.h"
#include "common.h"
#include "MOTOR.h"
#include "gpio.h"

typedef struct{
	float lenght;
	float wide;
}CCD_SIZE;


GROUP_PARA Group_p = {0};
CCD_SIZE CCD_size[4] = {0};

void Group_mode_para_init(void)
{
	// 4/3 ccd type
	CCD_size[0].lenght = 18;
	CCD_size[0].wide = 13.5;

	//aps-c ccd type
	CCD_size[1].lenght = 24;
	CCD_size[1].wide = 16;
	
	//aps-h ccd type
	CCD_size[2].lenght = 28.7;
	CCD_size[2].wide = 19.1;

	//全画幅
	CCD_size[3].lenght = 36;
	CCD_size[3].wide = 24;

	Group_p.lens_folcal = glob_para.lens_folcal;
	Group_p.GP_shut_mode= glob_para.GP_shut_mode;
	Group_p.Roverlap = glob_para.Roverlap;
	Group_p.GP_exposure = glob_para.GP_exposure;
}

// 手动快门处理，即电动模式开时，手按快门键
void Group_manul_Shuting(void)
{
	if(mode_backup != GROUP_PHOTO)return;
	//if(move_begin == 0)return;
	//if(m_start == 0)return;
	if(Group_p.GP_manul)
	{
		SHUTTER_ON;
		BLE_shut_start();
		move_begin = 3;
		p_move_time = 0;
	}
	else
	{
		if(move_begin)return;
		if(m_start)return;
			BLE_shut_start();
			camera_shutter_shot(1);
	}
}

uint32_t Calculate_current_angel(void)
{
	return (Absolute_value_calculation(Group_p.point_pulse_a, motorHH_p.GPpulse_count) * Angle_basic);
}


void Group_mode_Dir_check(void)
{
	if(mode_backup != GROUP_PHOTO)return;
	
		if(Group_p.GP_dir == A_TO_B)
		{
			motorHH_p.DIR = (Group_p.point_pulse_a > Group_p.point_pulse_b)?B_TO_A : A_TO_B;
		}
		else
		{
			motorHH_p.DIR = (Group_p.point_pulse_a > Group_p.point_pulse_b)?A_TO_B : B_TO_A;
			//glob_para.orbital_dir = ~glob_para.orbital_dir;
		}

		motorHH_direction_change(motorHH_p.DIR);
}

//计算两轴的最大可用张数，取用较小的那个
void Group_Max_amount_check(void)
{
	Group_p.Max_amount = Group_p.lens_pulse / 10;

}

void Group_set_abPoint(uint8_t typess)
{
	uint32_t pulse_h=0;
	
	Group_p.GP_set_if = typess;

	if(Group_p.GP_set_if == 1)
	{
		Group_p.origin_pulse = motorHH_p.pulse_count;

		Group_p.point_pulse_a = ORIGIN_POINT;
		motorHH_p.GPpulse_count = ORIGIN_POINT;
	}
	else if(Group_p.GP_set_if == 2)
	{
		motorHH_stop();
		Manul_p.HH_UpRoDown = 0;
		Angle_real_time_transmission(0);
		pulse_h = Absolute_value_calculation(Group_p.point_pulse_a, motorHH_p.GPpulse_count);

		if(pulse_h < (uint32_t)PULSE_SUM(1))
		{
			Group_p.GP_set_if = 0;
			Group_p.lens_pulse = 0;
			Send_para_data_to_controller(1, 1);
		}
		else
		{
			Group_p.GP_set_if = 3;
			
			//if(pulse_h > (uint32_t)PULSE_SUM(1))
			{
				Group_p.point_pulse_b = motorHH_p.GPpulse_count;
				Group_p.lens_pulse = pulse_h;
			}

			//Group_p.lens_folcal = 50;
			//Group_p.Roverlap = 30;
			//Group_p.GP_exposure = 1;
			Group_p.GP_dir = A_TO_B;
			Group_mode_Dir_check();
			Group_p.Gp_angle = Group_p.lens_pulse * Angle_basic;
			Group_Max_amount_check();
			//Delay_move_speed_calculate(0);
			Group_shots_event();
			Group_move_speed_calculate();
			Group_Ramp_Speed_Load();
			Send_para_data_to_controller(1, 1);
			Group_mode_Start_check_diretion();
		}
		
	}
}

// 反正切后的角度
float get_angle(double aradian)
{
	float angle = 0;
	float pi = 3.14159265;
	angle = aradian * 180.0 / pi;	
	return angle;
}

// 通过算法计算出需要拍摄 的张数
void Group_shots_event(void)
{
	float overlap_rate = 0;
	float number = 0;
	uint16_t temp = 0;

	overlap_rate = (float)1 - ((float)Group_p.Roverlap / (float)100); //除去重叠率计算出有效的画面

	// 水平张数通过画幅长度和镜头焦距通过反正切算出所需要的拍摄张数
	if(Group_p.GP_shut_mode) //竖拍
	{
		number = (float)Group_p.Gp_angle / ((2.0 * get_angle(atan((float)CCD_size[3].wide/2.0/(float)Group_p.lens_folcal))) * overlap_rate);
	}
	else
	{
		number = (float)Group_p.Gp_angle / ((2.0 * get_angle(atan((float)CCD_size[3].lenght/2.0/(float)Group_p.lens_folcal))) * overlap_rate);
	}
	temp = number * 10;

	if(temp % 10)
	{
		Group_p.amount = number + 1;
	}
	else
	{
		Group_p.amount = number;
	}

	if(Group_p.amount<1)
	{
		Group_p.amount=1;
	}
	else 
	{
		Group_p.amount +=1; //不为0时，需要加上一张(镜头在A/B两端各占半张)
	}
	
	if(Group_p.amount > Group_p.Max_amount)
	{
		Group_p.amount = Group_p.Max_amount;
	}
}

void Group_get_data_from_controller(uint8_t *fifog)
{
//	if(Group_p.check_dir)
//	{
		//Send_connect_data_to_controller();
//		return;
//	}
	
	if(fifog[4] == 0x01) // 镜头焦距
	{
		if(m_start)
		{
		//	Send_connect_data_to_controller();
			return;
		}
		Group_p.lens_folcal = (uint16_t)fifog[5] | (uint16_t)fifog[6]<<8;
		Group_shots_event();
		glob_para.lens_folcal = Group_p.lens_folcal;
		if_write_flash();
	}
	else if(fifog[4] == 0x02) //重叠率
	{
		if(m_start)
		{
		//	Send_connect_data_to_controller();
			return;
		}
		Group_p.Roverlap = fifog[5];
		Group_shots_event();
		glob_para.Roverlap = Group_p.Roverlap;
		if_write_flash();
	}
	else if(fifog[4] == 0x03) //
	{
		if(m_start)
		{
		//	Send_connect_data_to_controller();
			return;
		}
		Group_p.GP_exposure = fifog[5];
	}
	else if(fifog[4] == 0x04) // manul shut
	{
		Group_p.GP_manul = fifog[5];
	}
	else if(fifog[4] == 0x05) // dir 
	{
		if(m_start)
		{
		//	Send_connect_data_to_controller();
			return;
		}
		Group_p.GP_dir = fifog[5];
		Group_mode_Dir_check();
	}
	else if(fifog[4] == 0x06) //start or stop
	{
		m_start = fifog[5];
		Group_mode_start();
	}
	else if(fifog[4] == 0x07)
	{
		if(m_start)
		{
		//	Send_connect_data_to_controller();
			return;
		}
		Group_p.GP_shut_mode = fifog[5];
		glob_para.GP_shut_mode = Group_p.GP_shut_mode;
		if_write_flash();
	}
}

void Group_mode_countdwon_display(void)
{
	uint8_t bufers[12]={0};

	bufers[0] = 0x09;
	bufers[1] = 0x04;
	bufers[2] = 0x0a;
	
	bufers[3] = 4;
	bufers[4] = 0;
	bufers[5] = 0;
	bufers[6] = 0;
	bufers[7] = p_amount;
	bufers[8] = Group_p.amount;
	bufers[9] = m_start;
	bufers[10] = check_sum_add(10, bufers);
	Package_dataBufer(11, bufers);
}

// 计算每张照移动的速度，两个轴的速度要非常接近才能同步
void Group_move_speed_calculate(void)
{
	uint8_t amout_b = 0;
	
	amout_b = Group_p.amount - 1;

		if(amout_b)
		{
			if(Group_p.lens_pulse) // 如果水平轴有轨迹就以水平轴的为主，V 轴为次主
			{
				Group_p.move_pulse = (double)Group_p.lens_pulse / ((double)Group_p.amount-1);
				
			//	Hmove_times = (float)((DELAY_SPEED + 1) * (float)501 / (float)48000) * (float)Group_p.move_pulse;
			}
		}

		if(Group_p.amount == 1)
		{
			Group_p.move_pulse = Group_p.lens_pulse / 2;
		}
}

// 启动时检测方向是否正确，不正确就先找A /B
uint8_t Group_mode_Start_check_diretion(void)
{
	uint8_t temp = 0;
	if(mode_backup != GROUP_PHOTO)return 0;
	if(Group_p.lens_pulse ==0)return 0;
	
	if(Group_p.GP_dir == A_TO_B)
	{
		if(motorHH_p.GPpulse_count == Group_p.point_pulse_a)return 0;
		if(motorHH_p.GPpulse_count > Group_p.point_pulse_a)
		{
			motorHH_p.DIR = B_TO_A;
			temp = LOOKING_A_POINT;
		}
		else if(motorHH_p.GPpulse_count < Group_p.point_pulse_a)
		{
			motorHH_p.DIR = A_TO_B;
			temp = LOOKING_A_POINT;
		}
	}
	else if(Group_p.GP_dir == B_TO_A )
	{
		if(motorHH_p.GPpulse_count == Group_p.point_pulse_b)return 0;
		if(motorHH_p.GPpulse_count > Group_p.point_pulse_b)
		{
			motorHH_p.DIR = B_TO_A;
			temp = LOOKING_B_POINT;
		}
		else if(motorHH_p.GPpulse_count < Group_p.point_pulse_b)
		{
			motorHH_p.DIR = A_TO_B;
			temp = LOOKING_B_POINT;
		}
	}

	if(temp)
	{
		Group_p.check_dir= 1;
		motorHH_direction_change(motorHH_p.DIR);
		//Manul_SartOrStop(MOTOR_HORITAL,0xff);
		dynamic_speed = con_b.Ramp_id;
		motorHH_speed_set(con_b.ramp_buffer[dynamic_speed]);
		p_upORdown = SLOW_START;
		motorHH_start();
		Video_Find_ABpoint_notify(temp);
		/*DelayMode_ram_load( MOTOR_HORITAL);
		delay_p.locuHH.ramspeed = delay_p.locuHH.ram_id;
		motorHH_speed_set(delay_p.locuHH.ramspeed );
		delay_p.locuHH.Ram_UpRoDown = SLOW_START;
		motorHH_start();*/
	}

	return temp;
}

void Group_mode_start(void)
{
	uint8_t hhdir_check = 0;
//	uint8_t vvdir_check = 0;
	
	if(mode_backup != GROUP_PHOTO)return;
	if(m_start)
	{
		write_flash_active();
		if(move_begin_backup)
		{
			move_begin = move_begin_backup;
			if(move_begin == 1)
			{
				motorHH_start();
			}
			else
			{
			
			}
		}
		else
		{
			Group_shots_event();
			Group_move_speed_calculate();
			Group_Ramp_Speed_Load();
			hhdir_check = Group_mode_Start_check_diretion();
			if(hhdir_check==0)
			{
				p_amount = 0;
//				SHUTTER_ON;
//				BLE_shut_start();
				if(Group_p.amount == 1)
				{
					move_begin = 1;
					p_move_time = 0;
					//motorHH_start();
					SpHHorizontal_start();
				}
				else
				{
					p_move_time = 0;
					move_begin = 2;
				}
				
				Group_mode_countdwon_display();
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


void Group_main(void)
{
	if(mode_backup != GROUP_PHOTO)return;
	if(m_start == 0)return;
	if(move_begin == 0)return;

	
	if(move_begin == 2)
	{
		if(p_move_time >= 1000) // 开快门
		{
			if(Group_p.GP_manul==0)
			{
				SHUTTER_ON;
				BLE_shut_start();
				move_begin = 3;
				p_move_time = 0;
			}
		}
	}
	else if(move_begin == 3)
	{
		if(p_move_time >= (Group_p.GP_exposure*1000)) // 关快门
		{
			SHUTTER_OFF;
			//Panorama_mode_compensation();
			p_amount += 1;
			//con_b.p_amount_back += 1;
			Group_mode_countdwon_display();
			move_begin = 4;
			p_move_time = 0;
		}
	}
	else if(move_begin == 4)
	{
		if(p_move_time >= 200)
		{
			if(p_amount >= Group_p.amount)
			{
				motorHH_stop();
				p_move_time = 0;
				move_begin = 0;
				m_start = 0;
				Group_mode_countdwon_display();
				move_begin_backup = 0;
				//Group_p.GP_dir = ~Group_p.GP_dir;
				Group_mode_Dir_check();
				Group_mode_Start_check_diretion();
			}
			else
			{
				//motorHH_speed_set(DELAY_SPEED);
				Group_mode_Dir_check();
				move_begin = 1;
				p_move_time = 0;
				//motorHH_start();
				SpHHorizontal_start();
			}
		}
	}
}

// 水平电机到达慢停时触发此涵数
void Group_slow_check(uint32_t pulse_ck)
{
	//if(mode_backup != PREINSTALL_MODE)return;
	//if(con_b.begin != 2)return;
	if(pulse_ck == (Group_p.move_pulse - con_b.slow_point))
	{
		slider_upORdown(SLOW_STOP);
		set_slow_time_data();
	}
}

//每张照片的步距到达检测
void Group_mode_move_compara(void)
{
	static uint32_t pulse_add = 0;
	if(mode_backup != GROUP_PHOTO)return;
	if(move_begin != 1)return;
	if(find_pata.HHfind_Apoint)return;
	
	pulse_add += 1;
	Group_slow_check(pulse_add);
	if(pulse_add >= Group_p.move_pulse)
	{
		motorHH_stop();
		pulse_add = 0;
		//if(delay_p.locuVV.begins == 0)
		{
			move_begin = 2;
			p_move_time = 0;
		}
		
	}
}

// 进入合影模式找到原点的检测处理
void Group_find_Origin_checkStop(MOTOR_TYPE motor_t)
{
	if(mode_backup != GROUP_PHOTO)return;
	if(Group_p.check_dir)return;
	if(motor_t == MOTOR_HORITAL)
	{
		if(find_pata.HHfind_Apoint == 0)return;
		if(motorHH_p.pulse_count == Group_p.origin_pulse)
		{
			motorHH_stop();
			find_pata.HHfind_Apoint = 0;
			find_pata.slow_pulse = 0;
			find_pata.find_abpoint = 0;
			Group_p.check_dir = 0;
			//if(find_pataVV.HHfind_Apoint == 0)find_pata.find_abpoint = 0;
			motorHH_p.GPpulse_count = Group_p.point_pulse_a;
			Group_p.GP_dir = A_TO_B;
			//video_p.locusV.dir = video_p.DIR;
			Group_mode_Dir_check();
			Video_Find_ABpoint_notify(REACHED_ORIGIN_POINT);
			Send_connect_data_to_controller();
		}
	}
	
}

// 找A/B 点到达后的停止处理涵数
void Group_FindABpoint_pluse_check(MOTOR_TYPE motor_tt)
{
//	if(mode_backup != DELAY_SET)return;
	if(motor_tt == MOTOR_HORITAL)
	{
		if(Group_p.check_dir==0)return;

		if(Group_p.GP_dir == A_TO_B) // 实时是按B TO A 找A点
		{
			if(motorHH_p.GPpulse_count == Group_p.point_pulse_a)
			{
				motorHH_stop();
				find_pata.HHfind_Apoint = 0;
				find_pata.slow_pulse = 0;
				find_pata.find_abpoint = 0;
				Group_p.check_dir = 0;
				Group_mode_start();
				Video_Find_ABpoint_notify(REACHED_ABPOINT);
				Send_connect_data_to_controller();
				/*if(Group_p.amount == 1)
				{
					move_begin = 1;
					p_move_time = 0;
					//motorHH_start();
					SpHHorizontal_start();
				}
				else
				{
					p_move_time = 0;
					move_begin = 2;
				}*/
				//Group_mode_countdwon_display();
			}
		}
		else if(Group_p.GP_dir == B_TO_A)
		{
			if(motorHH_p.GPpulse_count == Group_p.point_pulse_b)
			{
				motorHH_stop();
				find_pata.HHfind_Apoint = 0;
				find_pata.slow_pulse = 0;
				find_pata.find_abpoint = 0;
				Group_p.check_dir = 0;
				Group_mode_start();

				Video_Find_ABpoint_notify(REACHED_ABPOINT);
				Send_connect_data_to_controller();
				/*if(Group_p.amount == 1)
				{
					move_begin = 1;
					p_move_time = 0;
					//motorHH_start();
					SpHHorizontal_start();
				}
				else
				{
					p_move_time = 0;
					move_begin = 2;
				}*/
			//	Group_mode_countdwon_display();
			}
		}
	}
}

//启动时检测到方向不对就启动找点后的缓停动作
void Group_FindABpoint_slow_check(MOTOR_TYPE motor_t)
{
//	if(mode_backup != DELAY_SET)return;
	
	if(motor_t == MOTOR_HORITAL)
	{
		//if(delay_p.locuHH.pulse== 0)return;
		if(Group_p.check_dir==0)return;
		//if(Manul_p.HH_UpRoDown==0)return;
				if(Group_p.GP_dir == A_TO_B) // 回到A点再开始
				{
					if(motorHH_p.DIR == A_TO_B)
					{
						if(motorHH_p.GPpulse_count == (Group_p.point_pulse_a - con_b.slow_point))
						{
							//find_pata.find_abpoint = SLOW_STOP;
							p_upORdown = SLOW_STOP;
						}
					}
					else
					{
						if(motorHH_p.GPpulse_count == (Group_p.point_pulse_a + con_b.slow_point))
						{
						//	find_pata.find_abpoint = SLOW_STOP;
							p_upORdown = SLOW_STOP;
						}
					}
				}
				else
				{
					if(motorHH_p.DIR == A_TO_B)
					{
						if(motorHH_p.GPpulse_count == (Group_p.point_pulse_b - con_b.slow_point))
						{
							//find_pata.find_abpoint = SLOW_STOP;
							p_upORdown = SLOW_STOP;
						}
					}
					else
					{
						if(motorHH_p.GPpulse_count == (Group_p.point_pulse_b + con_b.slow_point))
						{
							//find_pata.find_abpoint = SLOW_STOP;
							p_upORdown = SLOW_STOP;
						}
					}
				}
	}	
}


// 进入延时模式要自动找A 点
void Group_mode_find_Apoint(void)
{	
	if(mode_backup != GROUP_PHOTO)return;

	if(Group_p.lens_pulse)
	{
		if(motorHH_p.pulse_count != Group_p.origin_pulse)
		{
			if(motorHH_p.pulse_count > Group_p.origin_pulse)
			{
				if((motorHH_p.pulse_count - Group_p.origin_pulse) > (M_POINT_END-ORIGIN_POINT))
				{
					motorHH_p.DIR = A_TO_B;
				}
				else
				{
					motorHH_p.DIR = B_TO_A;
				}
			}
			else if(motorHH_p.pulse_count < Group_p.origin_pulse)
			{
				if((Group_p.origin_pulse - motorHH_p.pulse_count) > (M_POINT_END-ORIGIN_POINT))
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
		else
		{
			motorHH_p.GPpulse_count = Group_p.point_pulse_a;
			Group_p.GP_dir = A_TO_B;
			//video_p.locusV.dir = video_p.DIR;
			Group_mode_Dir_check();
		}
	}
}


// 水平轴每张相片移动的RAMP
// 需要缓起缓停的区间速度缓冲区
void Group_Ramp_Speed_Load(void)
{
	uint8_t i;
	uint16_t timeb = 0;
	
	con_b.slow_point = 0;
	con_b.ramp_buffer[0] = BASE_SPEED+5;

	
	for(i=1; i<20; i++)
	{
		
		if(Group_p.move_pulse < (con_b.slow_point*2))
		{
			return;
		}
		
		timeb = con_b.ramp_buffer[0] + (i + 1) * i / 	2;
		
		con_b.ramp_buffer[i] = timeb;
		con_b.slow_point += SLOW_BASIC_TIME33 / ((float)(timeb + 1) * (float)501 / (float)48000);
		con_b.Ramp_id = i;
	}
	
	//con_b.rampp_time = SLOW_BASIC_TIME33 * (con_b.Ramp_id-1);
	
//tring_Printf(USART_2,"DD",2);
//tring_Printf(USART_2,(uint8_t * )&con_b.slow_point,4);
//	return 1;
	
}

void Group_find_Origin_slow_check(MOTOR_TYPE motor_t)
{	
	if(motor_t == MOTOR_HORITAL)
	{
		if(Group_p.check_dir)return;
		if(find_pata.HHfind_Apoint == 0)return;

		if(motorHH_p.DIR == A_TO_B)
		{
			if(motorHH_p.pulse_count >= Group_p.origin_pulse - find_pata.slow_pulse)
			{
				find_pata.find_abpoint = SLOW_STOP;
			}
		}
		else
		{
			if(motorHH_p.pulse_count <= Group_p.origin_pulse + find_pata.slow_pulse)
			{
				find_pata.find_abpoint = SLOW_STOP;
			}
		}
	}
}

void Group_loop_check(void)
{
	if(mode_backup != GROUP_PHOTO)return;
		Group_find_Origin_checkStop(MOTOR_HORITAL);
		Group_find_Origin_slow_check(MOTOR_HORITAL);
		Group_mode_move_compara();
		Group_FindABpoint_pluse_check(MOTOR_HORITAL);
		Group_FindABpoint_slow_check(MOTOR_HORITAL);
}

