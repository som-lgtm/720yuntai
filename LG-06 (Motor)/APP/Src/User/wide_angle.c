
#include "common.h"
#include "motor.h"
#include "string.h"


void wide_mode_para_init(void)
{
	standard_H_frames[0]=4; //����47��һȦ��4�š�90��1��
	standard_H_frames[1]=4; // ����12��һȦ��5�ţ�72��1��
	standard_H_frames[2]=4; //����60��һȦ��3�ţ�120��1��

}

void wide_mode_start(void)
{	
	if(m_start)
	{
		if(move_begin_backup)// ��ͣ�������
		{
			move_begin = move_begin_backup;
			if(move_begin_backup == 1)Vertical_start();
			else if(move_begin_backup == 2)Horizontal_start();
		}
		else // �״�����0�ȿ�ʼˮƽ����
		{
			//con_b.time_down_count=con_b.time_down;
			p_amount = 0;
			p_move_time = 0;
			/*glob_para.orbital_dir11 = B_TO_A;
			motor11_direction_change(B_TO_A);
			Vertical_start();
			standard_mode_param_display(5);
			standard_mode_param_display(4);
			move_begin = 1;*/
		//	motorHH_p.DIR = B_TO_A;
		//	motorHH_direction_change(motorHH_p.DIR);
			//Horizontal_start();
			motorVV_p.DIR = B_TO_A;
			motorVV_direction_change(motorVV_p.DIR);
			Vertical_start();
			move_begin = 1;
			p_move_time = 0;
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

void wide_angle_mode_start(void)
{
	//if(con_b.go_back)return; // ���ڻ�ԭ�㲻�ɲ���
	//m_start = ~m_start;
	//wide_mode_start();
	standard_mode_start();
}

/*

��Ǿ�ͷ���������
1������0��һȦ��5�ţ�72��1��
3������55��һȦ��4�ţ�90��1��
2������34��һȦ��3�š�120��1��
һ������12����Ƭ                   */
void wide_get_countdown_time(void)
{
	uint8_t i;
	uint32_t h_time=0;
	uint32_t v_time=0;
	uint8_t wide_h_frame[3]={5,   4,  3}; // ����
	uint8_t wide_h_angle[3]={72, 90, 120}; // ���ٶ�Ϊ1�ţ�����������55��ʱ��ˮƽ��90����һ��
	uint8_t wide_v_angle[3]={0,  55, 89}; // �Ӹ���0�ȿ�ʼ���㣬������ת55���ģ�������ת95����
	
	memcpy(standard_H_frames, wide_h_frame, 3); // load data to DMA send buffer
	con_b.taltol_amount = 0;
	con_b.fix_stop_time = con_b.interval;
	con_b.sys_stop_time = 200;
	
	for(i=0; i<3; i++){ //���Ƕȵ�ʱ���ܺ�
		h_time += calculate_fast_speed_and_ramp_time(PULSE_SUM(wide_v_angle[i]), speed_calculat(STANDARD_SPEED));

		// ÿһ�ŵ�ʱ��= ����ʱ��+ 1����ͣʱ��+ ����0.5��+ �����յ���ͣʱ��0.5��
		v_time += (calculate_fast_speed_and_ramp_time(PULSE_SUM(wide_h_angle[i]), speed_calculat(STANDARD_SPEED))\
					+ con_b.fix_stop_time + con_b.exposure + con_b.sys_stop_time) * standard_H_frames[i];
		con_b.taltol_amount += standard_H_frames[i];
		standard_H_pulse[i] = PULSE_SUM(wide_h_angle[i]); //ˮƽ��Ƕȶ�Ӧ��������
		standard_V_pulse[i] = PULSE_SUM(wide_v_angle[i]); //������Ƕȶ�Ӧ��������
		con_b.remainder_p[i] = PULSE_SUM(wide_h_angle[i]);
	}

	con_b.time_down = (h_time + v_time) / 1000;
//	con_b.time_down_count = con_b.time_down;
}


//  ���������������ݲ���
void wideAngle_Get_data_from_controller(uint8_t *fifob)
{
	if(fifob[1] == 0x00) // ����
	{
		if(fifob[4] == 0x01) //delay time
		{
			con_b.wid_time_s = fifob[5];
			con_b.time_down_count = con_b.wid_time_s;
			standard_mode_param_display();
		}
		else if(fifob[4] == 0x02) //start or stop
		{
			if(con_b.go_back)return; // ���ڻ�ԭ�㲻�ɲ���
			m_start = fifob[5];
			wide_angle_mode_start();
		}
		else if(fifob[4] == 0x03) // exposure time ���ŵ㰴̨���ĵȴ�ʱ��
		{
		//	if(m_start)return;
			
			con_b.exposure = (uint16_t)fifob[5] | (uint16_t)fifob[6]<<8;
			//wide_get_countdown_time();
		}
		else if(fifob[4] == 0x04) // auto or manul
		{			
			con_b.wid_manul = fifob[5];
		}
		else if(fifob[4] == 0x05) //��ͣ�ٶ�
		{
			con_b.interval = (uint16_t)fifob[5] | (uint16_t)fifob[6]<<8;
		}
	}
	else if(fifob[1] == 0x06) // �����������Ļ�ȡͬ�������������Ҫ�������в���
	{
		if(fifob[3] == 0x06) // ������ģʽ
		{
			if(mode_backup != WIDE_ANGLE_MODE)
			{
				mode_backup = WIDE_ANGLE_MODE;
				wide_mode_para_init();
				wide_get_countdown_time();
				Send_connect_data_to_controller();
				con_b.time_down_count = con_b.wid_time_s;
				standard_mode_param_display(); //countdown display for the controller
			}
		}
	}
}

