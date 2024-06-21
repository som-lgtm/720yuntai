#include "common.h"
#include "motor.h"
#include "GPIO.h"
#include "string.h"


uint32_t standard_H_pulse[STAND_IDMAX]={0};// ������70�ȿ�ʼ��
uint32_t standard_V_pulse[STAND_IDMAX]={0};// ������70�ȿ�ʼ��
uint8_t standard_H_frames[STAND_IDMAX]={0};// ������70�ȿ�ʼ��
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
/*	standard_H_frames[0]=8; //ˮƽ0������
	standard_H_frames[1]=6; //����35������
	standard_H_frames[2]=3;//����60������
	standard_H_frames[3]=6;//����35������
	standard_H_frames[4]=3;//����70������
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
		if(move_begin_backup)// ��ͣ�������
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
		else // �״�����0�ȿ�ʼˮƽ����
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
// ˮƽ����������
void Horizontal_pulse_check(void)
{
	if(mode_backup != STANDAR_MODE && mode_backup != WIDE_ANGLE_MODE)return;
	if(move_begin != 2)return;
	if(find_pata.HHfind_Apoint)return;
	con_b.Dynamic_pulse += 1;
	if(con_b.Dynamic_pulse >= standard_H_pulse[con_b.section_id]+con_b.compensation) // ˮƽ���ߵ���λ�ú�ͣ��
	{
		motorHH_stop();
		con_b.Dynamic_pulse = 0;
		move_begin = 3;
		p_move_time = 0;
	}
}

// ��������������
void Vertical_pulse_check(void)
{
	if(mode_backup != STANDAR_MODE && mode_backup != WIDE_ANGLE_MODE)return;
	//if(move_begin != 1)return;
	if(find_pataVV.HHfind_Apoint)return;
	con_b.VVDynamic_pulse += 1;
	if(con_b.VVDynamic_pulse >= standard_V_pulse[con_b.VVsection_id]) //�����ᵽ��λ�ú���ˮƽ�Ὺʼ
	{
		motorVV_stop();
		motorVV_direction_change(B_TO_A);
		con_b.VVDynamic_pulse = 0;
		con_b.VVgo_back = 0;
//		Horizontal_start();
	}
}

// ˮƽ���������ͣʱ�����˺���
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

// �������������ͣʱ�����˺���
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

// �ֶ����ܴ򿪺󣬵ȴ�������������
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

//���������Ҫ���㴦��������
void standard_mode_main(void)
{
	BLE_shut_stop();
	if(mode_backup != STANDAR_MODE & mode_backup != WIDE_ANGLE_MODE)return;
	if(move_begin == 3)
	{
		if(p_move_time >= con_b.interval) // ͣ0.5��󿪿�������
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
	else if(move_begin == 4) //�������ŵ�ʱ��
	{
		//BLE_shut_stop();
		if(p_move_time >= con_b.exposure) // jʱ�䵽��ؿ���
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
		if(p_move_time >= con_b.sys_stop_time) // �����պ�ͣ0.5��������һ����������
		{
			if(p_amount < con_b.taltol_amount) // ��26��
			{
				if(con_b.p_amount_back >= (standard_H_frames[con_b.section_id] +1)) //һ�������Ƕ�����������ϣ�����һ�������Ƕ�
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
			else //�����������
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
	if(con_b.go_back)return; // ���ڻ�ԭ�㲻�ɲ���
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
//��׼��ͷ���������
1��ˮƽ  0��һȦ��8�ţ�45��1��:��V����0�ǽ�(ԭ��)��ʼˮƽ����8��(45��һ��)
4 ����36��һȦ��6�ţ�60��1�� : ����35�����꣬V������70��ͣ������35��,ˮƽ����6��
5 ����71��һȦ��3�ţ�120��1�� ������35�����꣬V������35�Ǻ�ͣ������70�ȣ�ˮƽ����3�� 
2������36��һȦ��6�ţ�60��1�� ������60�����꣬V������25�Ǻ�ͣ�ڸ���35�ȣ�ˮƽ����6��
3������51��һȦ��3�ţ�120��1�ţ�ԭ������V������60�Ⱥ�ˮƽ����3�š�
һ������26����Ƭ 

               */
void get_countdown_time(void)
{
	uint8_t i;
	uint32_t h_time=0;
	uint32_t v_time=0;
	//con_b.exposure = 500;
	//uint8_t standard_H_angle[5]={120, 60, 45, 60, 120}; // ������0�ȿ�ʼ��
	uint8_t standard_V_frame[5]={8,   6,  3,   6,   3};//����
	uint8_t standard_H_angle[5]={45, 60, 120, 60, 120}; // ������0�ȿ�ʼ��
	uint8_t standard_V_angle[5]={0 , 36,  35, 107, 15};// ������0�ȿ�ʼ��
	memcpy(standard_H_frames, standard_V_frame, 5); // load data to DMA send buffer
	con_b.fix_stop_time = con_b.interval;
	con_b.sys_stop_time = 200;
	con_b.taltol_amount = 0;
	for(i=0; i<5; i++){ //���Ƕȵ�ʱ���ܺ�
	
		standard_H_pulse[i] = PULSE_SUM(standard_H_angle[i]); //ˮƽ��Ƕȶ�Ӧ��������
		standard_V_pulse[i] = PULSE_SUM(standard_V_angle[i]); //������Ƕȶ�Ӧ��������
		con_b.remainder_p[i] = PULSE_SUM(standard_H_angle[i]);
		h_time += calculate_fast_speed_and_ramp_time(standard_V_pulse[i], speed_calculat(99));
//		String_Printf(USART_2, "AAAA", 4);
		
//		String_Printf(USART_2, (uint8_t *)&standard_H_pulse[i], 4);
//		String_Printf(USART_2, (uint8_t *)&v_time, 4);
		// ÿһ�ŵ�ʱ��= ����ʱ��+ 0.5����ͣʱ��+ ����0.5��+ �����յ���ͣʱ��0.2��
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

//��ʱʱ��ĵ���
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

//  ���������������ݲ���
void Standard_Get_data_from_controller(uint8_t *fifob)
{
	if(fifob[1] == 0x00) // ����
	{
		if(fifob[4] == 0x01) //delay time
		{
			con_b.delay_time_s = fifob[5];
			con_b.time_down_count = con_b.delay_time_s;
			standard_mode_param_display();
		}
		else if(fifob[4] == 0x02) //start or stop
		{
			if(con_b.go_back)return; // ���ڻ�ԭ�㲻�ɲ���
			m_start = fifob[5];
			standard_mode_start();
		}
		else if(fifob[4] == 0x03) // exposure time ���ŵ㰴̨���ĵȴ�ʱ��
		{
			//if(m_start)return;
			
			con_b.exposure = (uint16_t)fifob[5] | (uint16_t)fifob[6]<<8;
		//	get_countdown_time();
		}
		else if(fifob[4] == 0x04) // auto or manul
		{			
			con_b.std_manul = fifob[5];
		}
		else if(fifob[4] == 0x05) //��ͣ�ٶ�
		{
			con_b.interval = (uint16_t)fifob[5] | (uint16_t)fifob[6]<<8;
		}
	}
	else if(fifob[1] == 0x06) // �����������Ļ�ȡͬ�������������Ҫ�������в���
	{
		if(fifob[3] == 0x05) // �����׼ģʽ
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
		else if(fifob[3] == 0x06) // ������ģʽ
		{
			
		}
	}
}

//��׼ģʽ����ͣ������
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

//  ʱ�䵽���·ſ��Ŷ�������300MS̨����ż�
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

// ȫ������ģʽ�µ����岹��
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

// ����������ԭ�����ʾ��������
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

// ����ȫ��ģʽҪ�Զ���A ��
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

// ȫ��ģʽ�Զ���A �㵽����ֹͣ
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
