
#include "common.h"
#include "motor.h"
#include "string.h"


void wide_mode_para_init(void)
{
	standard_H_frames[0]=4; //俯角47度一圈拍4张。90度1张
	standard_H_frames[1]=4; // 仰角12度一圈拍5张，72度1张
	standard_H_frames[2]=4; //仰角60度一圈拍3张，120度1张

}

void wide_mode_start(void)
{	
	if(m_start)
	{
		if(move_begin_backup)// 暂停后的重启
		{
			move_begin = move_begin_backup;
			if(move_begin_backup == 1)Vertical_start();
			else if(move_begin_backup == 2)Horizontal_start();
		}
		else // 首次启动0度开始水平拍摄
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
	//if(con_b.go_back)return; // 正在回原点不可操作
	//m_start = ~m_start;
	//wide_mode_start();
	standard_mode_start();
}

/*

广角镜头拍摄参数：
1：仰角0度一圈拍5张，72度1张
3：仰角55度一圈拍4张，90度1张
2：俯角34度一圈拍3张。120度1张
一共拍摄12张照片                   */
void wide_get_countdown_time(void)
{
	uint8_t i;
	uint32_t h_time=0;
	uint32_t v_time=0;
	uint8_t wide_h_frame[3]={5,   4,  3}; // 张数
	uint8_t wide_h_angle[3]={72, 90, 120}; // 多少度为1张；俯仰轴向下55度时，水平轴90度拍一张
	uint8_t wide_v_angle[3]={0,  55, 89}; // 从俯角0度开始拍摄，后向下转55度拍，再向上转95角拍
	
	memcpy(standard_H_frames, wide_h_frame, 3); // load data to DMA send buffer
	con_b.taltol_amount = 0;
	con_b.fix_stop_time = con_b.interval;
	con_b.sys_stop_time = 200;
	
	for(i=0; i<3; i++){ //俯角度的时间总和
		h_time += calculate_fast_speed_and_ramp_time(PULSE_SUM(wide_v_angle[i]), speed_calculat(STANDARD_SPEED));

		// 每一张的时间= 行走时间+ 1秒暂停时间+ 拍照0.5秒+ 拍完照的暂停时间0.5秒
		v_time += (calculate_fast_speed_and_ramp_time(PULSE_SUM(wide_h_angle[i]), speed_calculat(STANDARD_SPEED))\
					+ con_b.fix_stop_time + con_b.exposure + con_b.sys_stop_time) * standard_H_frames[i];
		con_b.taltol_amount += standard_H_frames[i];
		standard_H_pulse[i] = PULSE_SUM(wide_h_angle[i]); //水平轴角度对应的脉冲数
		standard_V_pulse[i] = PULSE_SUM(wide_v_angle[i]); //府仰轴角度对应的脉冲数
		con_b.remainder_p[i] = PULSE_SUM(wide_h_angle[i]);
	}

	con_b.time_down = (h_time + v_time) / 1000;
//	con_b.time_down_count = con_b.time_down;
}


//  控制器发来的数据参数
void wideAngle_Get_data_from_controller(uint8_t *fifob)
{
	if(fifob[1] == 0x00) // 参数
	{
		if(fifob[4] == 0x01) //delay time
		{
			con_b.wid_time_s = fifob[5];
			con_b.time_down_count = con_b.wid_time_s;
			standard_mode_param_display();
		}
		else if(fifob[4] == 0x02) //start or stop
		{
			if(con_b.go_back)return; // 正在回原点不可操作
			m_start = fifob[5];
			wide_angle_mode_start();
		}
		else if(fifob[4] == 0x03) // exposure time 快门点按台起后的等待时间
		{
		//	if(m_start)return;
			
			con_b.exposure = (uint16_t)fifob[5] | (uint16_t)fifob[6]<<8;
			//wide_get_countdown_time();
		}
		else if(fifob[4] == 0x04) // auto or manul
		{			
			con_b.wid_manul = fifob[5];
		}
		else if(fifob[4] == 0x05) //缓停速度
		{
			con_b.interval = (uint16_t)fifob[5] | (uint16_t)fifob[6]<<8;
		}
	}
	else if(fifob[1] == 0x06) // 控制器来发的获取同步参数，电机端要返回所有参数
	{
		if(fifob[3] == 0x06) // 进入广角模式
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

