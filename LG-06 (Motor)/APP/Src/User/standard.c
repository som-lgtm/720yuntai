#include "common.h"
#include "motor.h"
#include "GPIO.h"
#include "string.h"


uint32_t standard_H_pulse[STAND_IDMAX]={0};// 从仰角70度开始拍
uint32_t standard_V_pulse[STAND_IDMAX]={0};// 从仰角70度开始拍
uint8_t standard_H_frames[STAND_IDMAX]={0};// 从仰角70度开始拍
uint8_t ble_shut_tag = 0;
uint16_t ble_shut_time = 0;
STANDARD_SLOW standard_p={0};

void ble_shut_time_count(void)
{
	if(ble_shut_time)ble_shut_time--;
	if(standard_p.HH_Ttime)standard_p.HH_Ttime--;
	if(standard_p.VV_Time)standard_p.VV_Time--;
}

void Horizontal_start(void)
{
	//dynamic_speed = con_b.Ramp_id;
	//motorHH_speed_set(con_b.ramp_buffer[con_b.Ramp_id]);
	//slider_upORdown(SLOW_START);
	//set_slow_time_data();
	
	standard_p.HH_speed = 10;
	motorHH_speed_set(con_b.ramp_buffer[standard_p.HH_speed]);
	//slider_upORdown(SLOW_START);
	standard_p.HH_UpRoDown = SLOW_START;
	set_slow_time_data();
	motorHH_start();
}

void Vertical_start(void)
{
	//dynamic_speed = con_b.Ramp_id;
	//motorVV_speed_set(con_b.ramp_buffer[con_b.Ramp_id]);
	//slider_upORdown(SLOW_START);
	//set_slow_time_data();
	standard_p.VV_speed = 6;
	motorVV_speed_set(con_b.ramp_buffer[standard_p.VV_speed]);
	//slider_upORdown(SLOW_START);
	standard_p.VV_UpRoDown = SLOW_START;
	//set_slow_time_data();
	standard_p.VV_Time = SLOW_BASIC_TIME00;
	motorVV_start();
}

void standard_mode_para_init(void)
{
/*	standard_H_frames[0]=8; //水平0度张数
	standard_H_frames[1]=6; //府下35度张数
	standard_H_frames[2]=3;//府下60度张数
	standard_H_frames[3]=6;//仰上35度张数
	standard_H_frames[4]=3;//仰上70度张数
	*/
	con_b.section_id=0;
	con_b.VVsection_id = 0;
	con_b.p_amount_back = 0;
	move_begin = 0;
	move_begin_backup = 0;
	con_b.go_back = 0;
	p_amount = 0;
	m_start = 0;
}

void standard_mode_start(void)
{	
	uint8_t delay_t = 0;
	
	if(m_start)
	{
		if(move_begin_backup)// 暂停后的重启
		{
			move_begin = move_begin_backup;
			if(move_begin_backup == 1)Vertical_start();
			else if(move_begin_backup == 2)
			{
				if(con_b.VVgo_back)Vertical_start();
				Horizontal_start();
			}
			//else if(move_begin_backup == 6)
		}
		else // 首次启动0度开始水平拍摄
		{
		//	con_b.time_down_count=con_b.time_down;
			p_amount = 0;
			p_move_time = 0;
			standard_mode_param_display();
			if(mode_backup == STANDAR_MODE)
			{
				delay_t = con_b.delay_time_s;
			}
			else if(mode_backup == WIDE_ANGLE_MODE)
			{
				delay_t = con_b.wid_time_s;
			}
			
			if(delay_t)
			{
				move_begin = 6;
				con_b.time_down_count = delay_t;
			}
			else
			{
				//motorHH_p.DIR = B_TO_A;
				//motorHH_direction_change(motorHH_p.DIR);
				BLE_shut_start();
				
			//	Horizontal_start();
				move_begin = 4;
				p_move_time = 0;
			}
		}
	}
	else
	{
		motorVV_stop();
		motorHH_stop();
		move_begin_backup = move_begin;
		move_begin = 0;
	}
}

void Vertical_pulse_goBack(void)
{
//	if(page_id != STANDAR_MODE & page_id != WIDE_ANGLE_MODE)return;
	if(con_b.go_back == 0)return;
	if(motorVV_p.pulse_count == ORIGIN_POINT)
	{
		motorVV_stop();
		con_b.go_back = 0;
		//String_Printf(USART_2, (uint8_t *)&glob_para.pulse_cout11, 4);
	}
}
// 水平轴电机脉冲检测
void Horizontal_pulse_check(void)
{
	if(mode_backup != STANDAR_MODE && mode_backup != WIDE_ANGLE_MODE)return;
	if(move_begin != 2)return;
	if(find_pata.HHfind_Apoint)return;
	con_b.Dynamic_pulse += 1;
	if(con_b.Dynamic_pulse >= standard_H_pulse[con_b.section_id]+con_b.compensation) // 水平行走到达位置后停下
	{
		motorHH_stop();
		con_b.Dynamic_pulse = 0;
		move_begin = 3;
		p_move_time = 0;
	}
}

// 府仰轴电机脉冲检测
void Vertical_pulse_check(void)
{
	if(mode_backup != STANDAR_MODE && mode_backup != WIDE_ANGLE_MODE)return;
	//if(move_begin != 1)return;
	if(find_pataVV.HHfind_Apoint)return;
	con_b.VVDynamic_pulse += 1;
	if(con_b.VVDynamic_pulse >= standard_V_pulse[con_b.VVsection_id]) //府仰轴到达位置后由水平轴开始
	{
		motorVV_stop();
		motorVV_direction_change(B_TO_A);
		con_b.VVDynamic_pulse = 0;
		con_b.VVgo_back = 0;
//		Horizontal_start();
	}
}

// 水平电机到达慢停时触发此涵数
void Horizontal_slow_check(void)
{
	if(mode_backup != STANDAR_MODE && mode_backup != WIDE_ANGLE_MODE )return;
	//if(move_begin != 2)return;
	if(con_b.Dynamic_pulse == (standard_H_pulse[con_b.section_id] - con_b.slow_point))
	{
	//	slider_upORdown(SLOW_STOP);
		standard_p.HH_UpRoDown = SLOW_STOP;
	//	set_slow_time_data();
	
	}
}

// 府仰电机到达慢停时触发此涵数
void Vertical_slow_check(void)
{
	if(mode_backup != STANDAR_MODE && mode_backup != WIDE_ANGLE_MODE)return;
	if(find_pataVV.HHfind_Apoint)return;
	//if(move_begin != 1)return;
	if(con_b.VVDynamic_pulse == (standard_V_pulse[con_b.VVsection_id] - con_b.slow_point))
	{
		//String_Printf(USART_2, (uint8_t *)&con_b.Dynamic_pulse, 4);
		//slider_upORdown(SLOW_STOP);
		
		standard_p.VV_UpRoDown = SLOW_STOP;
	}
}

// 手动功能打开后，等待按键触发快门
void Manul_Turn_the_shuter(void)
{
	if(mode_backup == STANDAR_MODE)
	{
		if(move_begin != 2 && move_begin != 1)
		{
			BLE_shut_start();
		}
		
		if(con_b.std_manul == 0xff)
		{
			if(move_begin == 3)
			{
			//	SHUTTER_ON;
			//	BLE_SHUT_OFF;
				p_move_time = 0;
				move_begin = 4;
			}
		}
	}
	else if(mode_backup == WIDE_ANGLE_MODE)
	{
		if(move_begin != 2 && move_begin != 1)
		{
			BLE_shut_start();
		}
		
		if(con_b.wid_manul == 0xff)
		{
			if(move_begin == 3)
			{
			//	SHUTTER_ON;
			//	BLE_SHUT_OFF;
				p_move_time = 0;
				move_begin = 4;
			}
		}
	}
	else if(mode_backup == PREINSTALL_MODE)
	{
		Spe_manul_Shuting();
	}
	else
	{
		BLE_shut_start();
	}
}

//启动后的主要拍摄处理主涵数
void standard_mode_main(void)
{
	BLE_shut_stop();
	if(mode_backup != STANDAR_MODE & mode_backup != WIDE_ANGLE_MODE)return;
	if(move_begin == 3)
	{
		if(p_move_time >= con_b.interval) // 停0.5秒后开快门拍照
		{
			if(mode_backup == STANDAR_MODE)
			{
				if(con_b.std_manul == 0)
				{
					SHUTTER_ON;
					BLE_shut_start();
					Red_led_tack();
					p_move_time = 0;
					move_begin = 4;
				}
			}
			else if(mode_backup == WIDE_ANGLE_MODE)
			{
				if(con_b.wid_manul == 0)
				{
					SHUTTER_ON;
					BLE_shut_start();
					Red_led_tack();
					p_move_time = 0;
					move_begin = 4;
				}
			}
		}
	}
	else if(move_begin == 4) //开启快门的时间
	{
		//BLE_shut_stop();
		if(p_move_time >= con_b.exposure) // j时间到后关快门
		{
			SHUTTER_OFF;
			//Panorama_mode_compensation();
			p_amount += 1;
			con_b.p_amount_back += 1;
			standard_mode_param_display();
			move_begin = 5;
			p_move_time = 0;
		}
	}
	else if(move_begin == 5)
	{
		if(p_move_time >= con_b.sys_stop_time) // 拍完照后停0.5秒后进入下一张拍照流程
		{
			if(p_amount < con_b.taltol_amount) // 共26张
			{
				if(con_b.p_amount_back >= (standard_H_frames[con_b.section_id] +1)) //一个府仰角度张数拍摄完毕，到下一个府仰角度
				{
					con_b.p_amount_back = 1;
					if(mode_backup == STANDAR_MODE)
					{
						if(con_b.section_id < 5)con_b.section_id +=1;
					}
					else if(mode_backup == WIDE_ANGLE_MODE)
					{
						if(con_b.section_id < 3)con_b.section_id +=1;
					}
					
					//BLE_shut_start();
					//move_begin = 4;
					motorHH_p.DIR = A_TO_B;
					motorHH_direction_change(motorHH_p.DIR);
					Horizontal_start();
					move_begin = 2;
					//move_begin = 1;
				}
				else 
				{
					if(con_b.p_amount_back >= standard_H_frames[con_b.section_id])
					{
						if(mode_backup == STANDAR_MODE)
						{
							if(con_b.VVsection_id < 5)con_b.VVsection_id +=1;
							if(con_b.VVsection_id > 2){
								motorVV_p.DIR = B_TO_A;
							}
							else{
								motorVV_p.DIR = A_TO_B;
							}
						}
						else if(mode_backup == WIDE_ANGLE_MODE)
						{
							if(con_b.VVsection_id < 3)con_b.VVsection_id +=1;
							if(con_b.VVsection_id==1)
							{
								motorVV_p.DIR = A_TO_B;
							}
							else if(con_b.VVsection_id == 2)
							{
								motorVV_p.DIR = B_TO_A;
							}
						}
						con_b.VVgo_back = 1;
						motorVV_direction_change(motorVV_p.DIR);
						Vertical_start();
						move_begin = 1;
					}
					
					motorHH_p.DIR = A_TO_B;
					motorHH_direction_change(motorHH_p.DIR);
					Horizontal_start();
					move_begin = 2;
					//String_Printf(USART_2, (uint8_t *)&glob_para.pulse_cout11, 4);
				}
			}
			else //整个拍摄完成
			{
				move_begin = 0;
				move_begin_backup = 0;
				m_start = 0;
				con_b.p_amount_back = 0;
				con_b.section_id=0;
				con_b.VVsection_id=0;
				con_b.time_down_count = 0;
				standard_mode_param_display();
				//con_b.go_back = 0xff;
				//motorVV_p.DIR = ~motorVV_p.DIR;
				//motorVV_direction_change(motorVV_p.DIR);
				//Vertical_start();
				find_pataVV.slow_pulse = 0;
				Panorama_mode_find_Apoint();
			}
			
			p_move_time = 0;
		}
	
	}
}


void standard_mode_OK(uint8_t types)
{
	if(con_b.go_back)return; // 正在回原点不可操作
	m_start = types;
	standard_mode_start();
}


void standard_mode_param_display(void)
{
	uint8_t bufers[12]={0};

	bufers[0] = 0x09;
	bufers[1] = 0x04;
	bufers[2] = 0x0a;
	bufers[3] = 0x05;
	
	bufers[4] = p_amount;
	bufers[5] = p_amount>>8;
	bufers[6] = con_b.time_down_count;
	bufers[7] = con_b.time_down_count>>8;
	if(mode_backup == PREINSTALL_MODE)
	{
		bufers[8] = sp_start;
	}
	else
	{
		bufers[8] = m_start;
	}
	bufers[9] = con_b.time_down_count;
	bufers[10] = con_b.time_down_count;
	bufers[9] = check_sum_add(9, bufers);
	Package_dataBufer(10, bufers);
}

/*
//标准镜头拍摄参数：
1：水平  0度一圈拍8张，45度1张:先V轴电机0角角(原点)开始水平轴拍8张(45角一张)
4 仰角36度一圈拍6张，60度1张 : 府角35度拍完，V轴上移70后停在仰角35度,水平轴拍6张
5 仰角71度一圈拍3张，120度1张 ：仰角35度拍完，V轴上移35角后停在仰角70度，水平轴拍3张 
2：俯角36度一圈拍6张，60度1张 ：府角60度拍完，V轴上移25角后停在府角35度，水平轴拍6张
3：俯角51度一圈拍3张，120度1张：原点拍完V轴下移60度后，水平轴拍3张。
一共拍摄26张照片 

               */
void get_countdown_time(void)
{
	uint8_t i;
	uint32_t h_time=0;
	uint32_t v_time=0;
	//con_b.exposure = 500;
	//uint8_t standard_H_angle[5]={120, 60, 45, 60, 120}; // 从仰角0度开始拍
	uint8_t standard_V_frame[5]={8,   6,  3,   6,   3};//张数
	uint8_t standard_H_angle[5]={45, 60, 120, 60, 120}; // 从仰角0度开始拍
	uint8_t standard_V_angle[5]={0 , 36,  35, 107, 15};// 从仰角0度开始拍
	memcpy(standard_H_frames, standard_V_frame, 5); // load data to DMA send buffer
	con_b.fix_stop_time = con_b.interval;
	con_b.sys_stop_time = 200;
	con_b.taltol_amount = 0;
	for(i=0; i<5; i++){ //俯角度的时间总和
	
		standard_H_pulse[i] = PULSE_SUM(standard_H_angle[i]); //水平轴角度对应的脉冲数
		standard_V_pulse[i] = PULSE_SUM(standard_V_angle[i]); //府仰轴角度对应的脉冲数
		con_b.remainder_p[i] = PULSE_SUM(standard_H_angle[i]);
		h_time += calculate_fast_speed_and_ramp_time(standard_V_pulse[i], speed_calculat(99));
//		String_Printf(USART_2, "AAAA", 4);
		
//		String_Printf(USART_2, (uint8_t *)&standard_H_pulse[i], 4);
//		String_Printf(USART_2, (uint8_t *)&v_time, 4);
		// 每一张的时间= 行走时间+ 0.5秒暂停时间+ 拍照0.5秒+ 拍完照的暂停时间0.2秒
		v_time += (calculate_fast_speed_and_ramp_time(standard_H_pulse[i], speed_calculat(99))\
					+ con_b.fix_stop_time + con_b.exposure + con_b.sys_stop_time) * standard_H_frames[i];
//		String_Printf(USART_2, "BBBB", 4);
		
//		String_Printf(USART_2, (uint8_t *)&v_time, 4);
		con_b.taltol_amount += standard_H_frames[i];

	}

//	String_Printf(USART_2, "AAAA", 4);

//	String_Printf(USART_2, (uint8_t *)&h_time, 4);
//	String_Printf(USART_2, (uint8_t *)&v_time, 4);
//	con_b.time_down = (h_time + v_time) / 1000;
//	con_b.time_down_count = con_b.time_down;
	//String_Printf(USART_2, (uint8_t *)&standard_V_pulse[0], 4);
}

//定时时间的倒计
void standard_countdown_time_display(void)
{
	static uint16_t ms_times = 0;

	if(mode_backup != STANDAR_MODE && mode_backup != WIDE_ANGLE_MODE && mode_backup != PREINSTALL_MODE)return;
	if(mode_backup == STANDAR_MODE  || mode_backup == WIDE_ANGLE_MODE)if(move_begin != 6)return;
	if(mode_backup == PREINSTALL_MODE)if(con_b.begin != 6)return;
	//if(mode_backup == STANDAR_MODE)if(con_b.time_down_count == 0)return;
	//if(mode_backup == WIDE_ANGLE_MODE)if(con_b.time_down_count == 0)return;
	ms_times +=1;
	if(ms_times >= 1000)
	{
		ms_times = 0;
		if(con_b.time_down_count > 0)con_b.time_down_count -= 1;
		standard_mode_param_display();
		if(con_b.time_down_count == 0)
		{
			if(mode_backup == PREINSTALL_MODE)
			{
				Specialty_mode_Start();
			//	Specialty_mode_downcount_begin();
			}
			else
			{
			/*	motorHH_p.DIR = A_TO_B;
				motorHH_direction_change(motorHH_p.DIR);
				Horizontal_start();
				move_begin = 2;
				p_move_time = 0;*/
					BLE_shut_start();
					
				//	Horizontal_start();
					move_begin = 4;
					p_move_time = 0;
			}
		}
	}
}

//  控制器发来的数据参数
void Standard_Get_data_from_controller(uint8_t *fifob)
{
	if(fifob[1] == 0x00) // 参数
	{
		if(fifob[4] == 0x01) //delay time
		{
			con_b.delay_time_s = fifob[5];
			con_b.time_down_count = con_b.delay_time_s;
			standard_mode_param_display();
		}
		else if(fifob[4] == 0x02) //start or stop
		{
			if(con_b.go_back)return; // 正在回原点不可操作
			m_start = fifob[5];
			standard_mode_start();
		}
		else if(fifob[4] == 0x03) // exposure time 快门点按台起后的等待时间
		{
			//if(m_start)return;
			
			con_b.exposure = (uint16_t)fifob[5] | (uint16_t)fifob[6]<<8;
		//	get_countdown_time();
		}
		else if(fifob[4] == 0x04) // auto or manul
		{			
			con_b.std_manul = fifob[5];
		}
		else if(fifob[4] == 0x05) //缓停速度
		{
			con_b.interval = (uint16_t)fifob[5] | (uint16_t)fifob[6]<<8;
		}
	}
	else if(fifob[1] == 0x06) // 控制器来发的获取同步参数，电机端要返回所有参数
	{
		if(fifob[3] == 0x05) // 进入标准模式
		{
			if(mode_backup != STANDAR_MODE)
			{
				mode_backup = STANDAR_MODE;
				standard_mode_para_init();
				get_countdown_time();
				Send_connect_data_to_controller();
				con_b.time_down_count = con_b.delay_time_s;
				standard_mode_param_display(); //countdown display for the controller
			}
		}
		else if(fifob[3] == 0x06) // 进入广角模式
		{
			
		}
	}
}

//标准模式缓启停处理涵数
void SDmotor_slowly_startedORstop(MOTOR_TYPE motor_t)
{
//	if(ramp_flag == 0)return;
	if(mode_backup != STANDAR_MODE && mode_backup != WIDE_ANGLE_MODE)return;
	if(motor_t == MOTOR_HORITAL)
	{
		if(standard_p.HH_UpRoDown==0)return;
		//if(p_upORdown == check_speed_tag)return;
		if(standard_p.HH_Ttime)return;

		if(standard_p.HH_UpRoDown == SLOW_STOP)
		{
			//motor_speed_set(dynamic_speed);
			
			if(standard_p.HH_speed < con_b.Ramp_id)
			{
				standard_p.HH_speed++;
				motorHH_speed_set(con_b.ramp_buffer[standard_p.HH_speed]);
				//motorVV_speed_set(con_b.ramp_buffer[standard_p.HH_speed]);
			}
			else 
			{
				standard_p.HH_UpRoDown = 0;
				
			//String_Printf(USART_2, (uint8_t *)&glob_para.pulse_cout, 4);
			//	time_delay_us(10000);
				//String_Printf(USART_2, (uint8_t *)&glob_para.pulse_cout11, 4);
				
			}
		}
		else if(standard_p.HH_UpRoDown == SLOW_START)
		{
			//motor_speed_set(dynamic_speed);
			
			if(standard_p.HH_speed > 0)
			{
				standard_p.HH_speed--;
				motorHH_speed_set(con_b.ramp_buffer[standard_p.HH_speed]);
				//motorVV_speed_set(con_b.ramp_buffer[standard_p.HH_speed]);
				//String_Printf(USART_2, (uint8_t *)&con_b.ramp_buffer[dynamic_speed], 2);
			}
			else 
			{
				standard_p.HH_UpRoDown = 0;
			//	String_Printf(USART_2, (uint8_t *)&con_b.Dynamic_pulse, 4);
				//String_Printf(USART_2, (uint8_t *)&con_b.slow_point, 4);
			}
		}
		
		set_slow_time_data();
	}
	else
	{
		if(standard_p.VV_UpRoDown==0)return;
		//if(p_upORdown == check_speed_tag)return;
		if(standard_p.VV_Time)return;

		if(standard_p.VV_UpRoDown == SLOW_STOP)
		{
			//motor_speed_set(dynamic_speed);
			
			if(standard_p.VV_speed < con_b.Ramp_id)
			{
				standard_p.VV_speed++;
				//motorHH_speed_set(con_b.ramp_buffer[standard_p.HH_speed]);
				motorVV_speed_set(con_b.ramp_buffer[standard_p.VV_speed]);
			}
			else 
			{
				standard_p.VV_UpRoDown = 0;
				
			//String_Printf(USART_2, (uint8_t *)&glob_para.pulse_cout, 4);
			//	time_delay_us(10000);
				//String_Printf(USART_2, (uint8_t *)&glob_para.pulse_cout11, 4);
				
			}
		}
		else if(standard_p.VV_UpRoDown == SLOW_START)
		{
			//motor_speed_set(dynamic_speed);
			
			if(standard_p.VV_speed > 0)
			{
				standard_p.VV_speed--;
				//motorHH_speed_set(con_b.ramp_buffer[standard_p.HH_speed]);
				motorVV_speed_set(con_b.ramp_buffer[standard_p.VV_speed]);
				//String_Printf(USART_2, (uint8_t *)&con_b.ramp_buffer[dynamic_speed], 2);
			}
			else 
			{
				standard_p.VV_UpRoDown = 0;
			//	String_Printf(USART_2, (uint8_t *)&con_b.Dynamic_pulse, 4);
				//String_Printf(USART_2, (uint8_t *)&con_b.slow_point, 4);
			}
		}
		
		standard_p.VV_Time = SLOW_BASIC_TIME44;
	}
}

//  时间到后事放快门动作，即300MS台起快门键
void BLE_shut_stop(void)
{
	if(ble_shut_tag)
	{
		if(ble_shut_time  <= 10)
		{
			ble_shut_tag = 0;
			BLE_SHUT_ON;
		}
	}
 }

void BLE_shut_start(void)
{
	if(ble_shut_tag == 0)
	{
		ble_shut_tag = 1;
		ble_shut_time = 300;
		BLE_SHUT_OFF;
	}
}

// 全景所有模式下的脉冲补尝
void Panorama_mode_compensation(void)
{
//	uint8_t idex=0;
	if(mode_backup != STANDAR_MODE && mode_backup != WIDE_ANGLE_MODE)return;
	
	con_b.diff += (con_b.remainder_p[con_b.section_id] - (double)standard_H_pulse[con_b.section_id]);
	
	if(con_b.diff > 1.0)
	{
		con_b.compensation = 1;
		con_b.diff -=1.0;
	}
	else
	{
		con_b.compensation = 0;
	}
}

// 发送正在找原点的显示给控制器
void Send_find_the_Apoint_tag(uint8_t types)
{
	uint8_t buffers[7]={0};
	buffers[0] = 0x09;
	buffers[1] = 0x01;
	buffers[2] = 0x09;
	buffers[3] = 0x02;
	buffers[4] = types;
	buffers[5] = mode_backup;
	buffers[6] = check_sum_add(6, buffers);
	Package_dataBufer(7, buffers);

}

// 进入全景模式要自动找A 点
void Panorama_mode_find_Apoint(void)
{	
/*	if(motorVV_p.pulse_count != ORIGIN_POINT)
	{
		if(motorVV_p.pulse_count < M_POINT_END)
		{
			motorVV_p.DIR = B_TO_A;
		}
		else if(motorVV_p.pulse_count > M_POINT_END)
		{
			motorVV_p.DIR = A_TO_B;
		}
			motorVV_direction_change(motorVV_p.DIR);

			set_abpoint_start_setting(MOTOR_Vertical);
	}
*/
	if(motorHH_p.pulse_count != ORIGIN_POINT)
	{
		if(motorHH_p.pulse_count < M_POINT_END)
		{
			motorHH_p.DIR = B_TO_A;
		}
		else if(motorHH_p.pulse_count > M_POINT_END)
		{
			motorHH_p.DIR = A_TO_B;
		}
			
			motorHH_direction_change(motorHH_p.DIR);

			set_abpoint_start_setting(MOTOR_HORITAL);
			Send_find_the_Apoint_tag(1);
	}

}

// 全景模式自动找A 点到达后的停止
void Panorama_find_end_point_checkStop(MOTOR_TYPE motor_t)
{
	//if(mode_backup != PREINSTALL_MODE)return;
	
	if(motor_t == MOTOR_HORITAL)
	{
		if(find_pata.HHfind_Apoint == 0)return;
		
		if(motorHH_p.DIR == A_TO_B)
		{
			/*if(find_pata.compensation)
			{
				if(motorHH_p.pulse_count == ORIGIN_POINT + find_pata.compensation)
				{
					motorHH_p.DIR = B_TO_A;
					find_pata.compensation = 0;
					motorHH_direction_change(motorHH_p.DIR);
				}
			}
			else*/
			{
				if(motorHH_p.pulse_count == ORIGIN_POINT)
				{
					motorHH_stop();
					find_pata.HHfind_Apoint = 0;
					find_pata.find_abpoint = 0;
					find_pata.slow_pulse = 0;
					//if(find_pataVV.HHfind_Apoint == 0)
					{
						Send_find_the_Apoint_tag(0);
					}
				}
			}
		}
		else if(motorHH_p.DIR == B_TO_A)
		{
			/*if(find_pata.compensation)
			{
				if(motorHH_p.pulse_count == B_POINT_END - find_pata.compensation)
				{
					motorHH_p.DIR = A_TO_B;
					find_pata.compensation = 0;
					motorHH_direction_change(motorHH_p.DIR);
				}
			}
			else*/
			{
				if(motorHH_p.pulse_count == ORIGIN_POINT)
				{
					motorHH_stop();
					find_pata.HHfind_Apoint = 0;
					find_pata.find_abpoint = 0;
					find_pata.slow_pulse = 0;
					//if(find_pataVV.HHfind_Apoint == 0)
					{
						Send_find_the_Apoint_tag(0);
					}
				}
			}
		}
			//Manu_mode_dir_check(motor_t);
	}
}

void Panorama_find_end_point_slowChcek(MOTOR_TYPE motor_t)
{
	if(mode_backup != PANORAMA_MAIN && mode_backup != STANDAR_MODE && mode_backup != WIDE_ANGLE_MODE && mode_backup != PREINSTALL_MODE)return;
	if(motor_t == MOTOR_HORITAL)
	{
		if(find_pata.HHfind_Apoint==0)return;
		
		if(motorHH_p.DIR == A_TO_B)
		{
			if(motorHH_p.pulse_count >= (B_POINT_END - find_pata.slow_pulse))
			{
				find_pata.find_abpoint = SLOW_STOP;
				//standard_p.HH_Ttime = 15;
			}
		}
		else
		{
			if(motorHH_p.pulse_count <= (ORIGIN_POINT + find_pata.slow_pulse))
			{
				find_pata.find_abpoint = SLOW_STOP;
				//standard_p.HH_Ttime = 15;
			}
		}
		 
	}
	else if(motor_t == MOTOR_Vertical)
	{
		if(find_pataVV.HHfind_Apoint==0)return;
		
		if(motorVV_p.DIR == A_TO_B)
		{
			if(motorVV_p.pulse_count >= (B_POINT_END - find_pataVV.slow_pulse))
			{
				find_pataVV.find_abpoint = SLOW_STOP;
			//	standard_p.VV_Time = 15;
			//String_Printf(USART_2,"AA",2);
			}
		}
		else
		{
			if(motorVV_p.pulse_count <= (ORIGIN_POINT + find_pataVV.slow_pulse))
			{
				find_pataVV.find_abpoint = SLOW_STOP;
				//standard_p.VV_Time = 15;
				//String_Printf(USART_2,"BB",2);
			}
		}
	}
}
