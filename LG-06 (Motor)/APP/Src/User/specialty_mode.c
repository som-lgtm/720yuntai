

#include "common.h"
#include "uart.h"
#include "stdio.h" //mency 的头文件
#include "string.h" // memset的头文件
#include "stdlib.h" //abs的头文件
#include "motor.h"
#include "gpio.h"
#include "RF24L01.h"

#define RAMP_MAX 170
//MODE_PARA spe_para[8]={0};
uint8_t Sync_sendt=0;

uint8_t mid_ct = 0;
uint8_t run_id = 0;
uint8_t run_idVV = 0;
uint8_t sp_start= 0;
uint8_t delay_cout = 0;
uint8_t H_goBack = 0;
uint8_t V_goBack = 0;
uint8_t H_slow_tag = 0; // 慢启停标志记
uint8_t V_slow_tag = 0; // 慢启停标志记
uint8_t Alon_sendt = 0;
uint16_t H_frame_pulse[PARA_MAX_ID]={0};
uint32_t V_frame_pulse[PARA_MAX_ID]={0};

SPEDCIALTY_MOVE_PARA sp_move_par[PARA_MAX_ID]={0};

void specialty_mode_para_clear(void)
{
	memset((void *)&glob_para.spe_para, 0, sizeof(glob_para.spe_para));
	glob_para.mode_id = 0;
}

void SpVVslider_upORdown(uint8_t p_type)
{
	con_b.p_upDown = p_type;
}

//慢启停的间隔时间
void SpVVset_slow_time_data(void)
{
	con_b.slw_time = SLOW_BASIC_TIME00 ;//SLOW_THRESHOLD - dynamic_speed + 2;
}

//水平轴启动
void SpHHorizontal_start(void)
{
	//dynamic_speed = con_b.Ramp_id;
	//motorHH_speed_set(con_b.ramp_buffer[con_b.Ramp_id]);
	//slider_upORdown(SLOW_START);
	//set_slow_time_data();
	H_slow_tag = 1; // 慢启停标志记
	dynamic_speed = 7;
	motorHH_speed_set(con_b.ramp_buffer[dynamic_speed]);
	slider_upORdown(SLOW_START);
	set_slow_time_data();
	motorHH_start();
	
	//Horizontal_start();
}

void SpVVertical_start(void)
{
	//dynamic_speed = con_b.Ramp_id;
	//motorVV_speed_set(con_b.ramp_buffer[con_b.Ramp_id]);
	//slider_upORdown(SLOW_START);
	//set_slow_time_data();
	V_slow_tag = 1;
	con_b.dynamic = 7;
	motorVV_speed_set(con_b.Vramp_buffer[con_b.dynamic]);
	SpVVslider_upORdown(SLOW_START);
	SpVVset_slow_time_data();
	motorVV_start();
	//Vertical_start();
}

void sp_para_init(void)
{
	uint8_t i;
	for(i=0; i<MODE_ID_MAX; i++)
	{
		glob_para.spe_para[i].Param[0].lev_angle = 360;
		if(glob_para.spe_para[i].para_id>=PARA_MAX_ID)glob_para.spe_para[i].para_id=0;
	}
	
	if(glob_para.mode_id>=MODE_ID_MAX)glob_para.mode_id=0;
	mid_ct = glob_para.mode_id;
	if(mid_ct >=MODE_ID_MAX)mid_ct=MODE_ID_MAX-1;
}

void Sp_para_start_init(void)
{
	uint8_t i;
	uint16_t temps=0;
	if(mid_ct>=MODE_ID_MAX)mid_ct =MODE_ID_MAX-1;
	if(glob_para.spe_para[mid_ct].para_id>=PARA_MAX_ID)glob_para.spe_para[mid_ct].para_id=PARA_MAX_ID-1;
	for(i=0; i< glob_para.spe_para[mid_ct].para_id; i++)
	{
		temps += glob_para.spe_para[mid_ct].Param[i+1].amout;
		specialty_parameter_calculate(i+1);
	}
	
	glob_para.spe_para[mid_ct].total_aount = temps;
	con_b.taltol_amount = temps;

	con_b.shut_time = (uint16_t)glob_para.spe_para[mid_ct].Param[run_id].exposure * 1000;
	con_b.interval_t = (uint16_t)glob_para.spe_para[mid_ct].Param[run_id].interval * 1000;
	con_b.sys_stop = (uint16_t)glob_para.spe_para[mid_ct].Param[run_id].sys_stop * 1000;
	//con_b.interval_t = con_b.interval_t - con_b.shut_time;
	
}

// 删除专业模式下的模式
void Delete_mode_para(uint8_t types, uint8_t idx)
{
	uint8_t i = 0;
	if(mode_backup == PREINSTALL_MODE)
	{
		if(types == 0x0c) // 删除模式
		{
			if(glob_para.mode_id>=MODE_ID_MAX)glob_para.mode_id=MODE_ID_MAX-1;
			for(i=idx; i< glob_para.mode_id; i++)
			{
				memcpy((void *)&glob_para.spe_para[i].set_tag, (void *)&glob_para.spe_para[i+1].set_tag, sizeof(glob_para.spe_para[i])); // shift the data
			}
			
			memset((void *)&glob_para.spe_para[i].set_tag, 0, sizeof(glob_para.spe_para[i]));
			if(glob_para.mode_id)glob_para.mode_id -=1;
		}
		else if(types == 0x0d) // 删除参数
		{
			if(mid_ct>=MODE_ID_MAX)mid_ct =MODE_ID_MAX-1;
			if(glob_para.spe_para[mid_ct].para_id>=PARA_MAX_ID)glob_para.spe_para[mid_ct].para_id=PARA_MAX_ID-1;
			for(i=idx; i< glob_para.spe_para[mid_ct].para_id; i++)
			{
				memcpy((void *)&glob_para.spe_para[mid_ct].Param[i].lev_angle, (void *)&glob_para.spe_para[mid_ct].Param[i+1].lev_angle, sizeof(glob_para.spe_para[mid_ct].Param[i])); // shift the data
			}
			memset((void *)&glob_para.spe_para[mid_ct].Param[glob_para.spe_para[mid_ct].para_id].lev_angle, 0, sizeof(glob_para.spe_para[mid_ct].Param[glob_para.spe_para[mid_ct].para_id]));
			if(glob_para.spe_para[mid_ct].para_id)glob_para.spe_para[mid_ct].para_id -= 1;
		}
	}
}


void specialty_parameter_calculate(uint8_t idxs)
{
	uint16_t m_angle = 0;
//	uint16_t frame_ange = 0;
	if(mid_ct>=MODE_ID_MAX)mid_ct =MODE_ID_MAX-1;

	if(glob_para.spe_para[mid_ct].Param[idxs].amout == 0)return;
	
// H轴的角度换算出对应张数下的角度脉冲数
	m_angle = glob_para.spe_para[mid_ct].Param[idxs].lev_angle;
	con_b.remainder_p[idxs] = PULSE_SUM(m_angle) / (double)(glob_para.spe_para[mid_ct].Param[idxs].amout);
//	H_frame_pulse[idxs] = con_b.remainder_p[idxs];
	sp_move_par[idxs].lev_pulse = con_b.remainder_p[idxs];	
}

//水平抽原点检测，不在原点就要回到原点后开始下一个参数拍摄, V 轴检测当前起点
uint8_t HHmotor_return_to_theOrigin_start(void)
{
	uint8_t temp = 0;
	
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
		SpHHorizontal_start();
		con_b.HHgo_back = 0xff;
		temp = 1;
	}
	else
	{
		con_b.HHgo_back = 0;
	}

	return temp;
}

//启动的开快门开始拍摄
void leve_turn_on_shutter(void)
{
	//SHUTTER_ON;
	//BLE_shut_start();
	//time_delay_ms(100);
	con_b.begin = 3;
	p_move_time = 0;
}

// 专业模式启动前的迟时开始倒计时到达后的启动处理涵数
void Specialty_mode_downcount_begin(void)
{
	if(delay_cout)
	{
		delay_cout = 0;
		if(sp_move_par[run_idVV].ele_pulse != ORIGIN_POINT && sp_move_par[run_idVV].ele_pulse != motorVV_p.pulse_count)
		{
			con_b.begin = 7;
		}
		else
		{
			delay_cout = 0;
			leve_turn_on_shutter();
		}

	}
}

void Specialty_mode_Start(void)
{
	uint8_t i=0;
	if(sp_start)
	{
		if(con_b.begin_back)
		{
			con_b.begin = con_b.begin_back;
			if(con_b.begin == 1)
			{
				if(con_b.HHgo_back)
				{
					SpHHorizontal_start();
				}
			}
			if(con_b.begin == 2)SpHHorizontal_start(); //H 轴移动
			if(con_b.begin == 6)con_b.begin_back=0; //还未开始
			else if(con_b.begin == 7)SpVVertical_start();
		}
		else
		{
			p_amount = 0;
			con_b.p_amount_back = 0;
			run_id = 1;
			run_idVV = 1;
			Sp_para_start_init();
			//SpVVRamp_Speed_Load(speed_calculat(STANDARD_SPEED));
			for(i=BASE_SPEED; i < RAMP_MAX; i++)
			{
				if(SpecialtyHH_Ramp_Speed_Load(i, run_id))
				{
					break;				
				}
			}
			/*for(i=0; i<glob_para.spe_para[mid_ct].para_id; i++)
			{
				specialty_parameter_calculate(i+1);
			}*/
		//	String_Printf(USART_2,"FFFF",4);
		//	String_Printf(USART_2,(uint8_t *)&motorHH_p.pulse_count,4);
		//	String_Printf(USART_2,(uint8_t *)&V_frame_pulse[run_id],4);
		//	String_Printf(USART_2,(uint8_t *)&con_b.go_back,1);
			if(mid_ct>=MODE_ID_MAX)mid_ct =MODE_ID_MAX-1;
			if(glob_para.spe_para[mid_ct].delay_t && delay_cout==0) // 有延迟时需要等待
			{
				con_b.time_down_count = glob_para.spe_para[mid_ct].delay_t;
				delay_cout = 1;
				con_b.begin = 6;
				//con_b.begin_back = con_b.begin;
			}
			else
			{
				/*	if(sp_move_par[run_idVV].ele_pulse != ORIGIN_POINT && sp_move_par[run_idVV].ele_pulse != motorVV_p.pulse_count)
					{
						con_b.begin = 7;
						motorVV_p.DIR = sp_move_par[run_idVV].ele_dir;
						//con_b.begin = 1;
						//V_goBack = 1;
						motorVV_direction_change(motorVV_p.DIR);
						SpVVertical_start();
						//con_b.begin_back = con_b.begin;
						delay_cout = 0;
					}
					else*/
					{
						delay_cout = 0;
						leve_turn_on_shutter();
						Red_led_tack();
						//con_b.begin = 3;
					}

			}
		}
	}
	else
	{
		motorVV_stop();
		motorHH_stop();
		con_b.begin_back = con_b.begin;
		con_b.begin = 0;
	}
}

void Specialty_Get_data_from_controller(uint8_t *fifob)
{
	uint8_t idx = 0;

	idx = fifob[7];
	
	if(mid_ct>=MODE_ID_MAX)mid_ct =MODE_ID_MAX-1;
	if(fifob[4] == 0x01)// 角度
	{
		glob_para.spe_para[mid_ct].Param[idx].lev_angle = (uint16_t)fifob[5] | (uint16_t)fifob[6]<<8;
		specialty_parameter_calculate(idx);
		/*if(spe_para[mid_ct].Param[idx].ele_angle < 0)
		{
			String_Printf(USART_2,"FFFF",4);
		}*/
	}
	else if(fifob[4] == 0x02) //水平角度
	{
		glob_para.spe_para[mid_ct].Param[idx].shut_times = (uint16_t)fifob[5] | (uint16_t)fifob[6]<<8;
	}
	else if(fifob[4] == 0x03) //张数
	{
		glob_para.spe_para[mid_ct].Param[idx].amout = fifob[5];
		specialty_parameter_calculate(idx);
	}
	else if(fifob[4] == 0x04) //曝光时间
	{
		glob_para.spe_para[mid_ct].Param[idx].exposure = fifob[5];
		if_write_flash();
		//String_Printf(USART_2,"EEEE",4);
	//	String_Printf(USART_2,(uint8_t *)&glob_para.spe_para[mid_ct].Param[idx].exposure,2);
	}
	else if(fifob[4] == 0x05) //间隔时间
	{
		glob_para.spe_para[mid_ct].Param[idx].interval = fifob[5];
		if_write_flash();
		//write_flash_active();
	}
	else if(fifob[4] == 0x06) //定时时间，即延迟开始的时间
	{
		glob_para.spe_para[mid_ct].delay_t = fifob[5];
		con_b.time_down_count = glob_para.spe_para[mid_ct].delay_t;
		//standard_mode_param_display();
	}
	else if(fifob[4] == 0x07) //手动功能
	{
		glob_para.spe_para[mid_ct].manul = fifob[5];
	}
	else if(fifob[4] == 0x08) //启/停
	{
		sp_start = fifob[5];
		Specialty_mode_Start();
		
		write_flash_active();
	}
	else if(fifob[4] == 0x09) //添加模式ID
	{
		if(mid_ct != fifob[5])
		{
			spe_mode_clear();
			motorVV_stop();
			motorHH_stop();
			mid_ct = fifob[5];
			if(mid_ct>=MODE_ID_MAX)mid_ct=MODE_ID_MAX-1;
			Panorama_mode_find_Apoint();
		}
		
		if(glob_para.mode_id < mid_ct)glob_para.mode_id +=1;
	}
	else if(fifob[4] == 0x0A) //添加参数ID
	{
		//para_id_c = fifob[6];
		glob_para.spe_para[mid_ct].para_id = fifob[6];
		if(glob_para.spe_para[mid_ct].para_id>=PARA_MAX_ID)glob_para.spe_para[mid_ct].para_id=PARA_MAX_ID-1;
		glob_para.spe_para[mid_ct].Param[fifob[7]].set_flag = 1;
	//	String_Printf(USART_2,&glob_para.spe_para[mid_ct].para_id,1);
		if_write_flash();
		//write_flash_active();
	}
	else if(fifob[4] == 0x0B) //取消参数设置, 需要返回所有参数，用于同步数据
	{
		Cancel_the_para_set_return(fifob[5]);
	}
	else if(fifob[4] == 0x0C) // 删除模式
	{
		Delete_mode_para(fifob[4], fifob[5]);
		if_write_flash();
		//write_flash_active();
	}
	else if(fifob[4] == 0x0D) //删除对应模式下的参数
	{
		Delete_mode_para(fifob[4], fifob[5]);
		if_write_flash();
		//write_flash_active();
	}
	else if(fifob[4] == 0x0e) // 延时时间，快门关闭后的等待时间
	{
		glob_para.spe_para[mid_ct].Param[idx].sys_stop = fifob[5];
		if_write_flash();
	}
	else if(fifob[4] == 0x0f) // 速度档位
	{
		glob_para.spe_para[mid_ct].speed_range = fifob[5];
		if_write_flash();
	}
	else if(fifob[4] == 0x10) // 方向
	{
		if(fifob[5] == 1) //left
		{
			
		}
		else if(fifob[5] == 2) //right
		{
			
		}
	}
}

void Specialty_modeID_sendt(void)
{
//	uint8_t i=0;
	uint8_t buffer[10] = {0};
	
	buffer[0] = 9;
	buffer[1] = 0;
	buffer[2] = 9;
	buffer[3] = 8;
	
	buffer[4] = glob_para.mode_id;
	buffer[5] = mid_ct;
	buffer[6] = run_id;
	buffer[7] = sp_start;
	buffer[8] = check_sum_add(8, buffer);
	
	Package_dataBufer(9,buffer);
}

// 发送专业模式下的所有模式和所有参数给控制器显示
void Specialty_Send_data(uint8_t m_id)
{
	uint8_t buffer[20] = {0};
	uint8_t j=0;
	
	buffer[0] = 9;
	buffer[1] = 0;
	buffer[2] = 9;
	buffer[3] = 7;
	
	if(glob_para.spe_para[mid_ct].para_id>=PARA_MAX_ID)glob_para.spe_para[mid_ct].para_id=PARA_MAX_ID-1;
		for(j=0; j<glob_para.spe_para[m_id].para_id; j++)
		{
		//	if(glob_para.spe_para[m_id].Param[j+1].set_flag)
			{
				buffer[4] = (uint16_t)glob_para.spe_para[m_id].Param[j+1].lev_angle;
				buffer[5] = (uint16_t)glob_para.spe_para[m_id].Param[j+1].lev_angle>>8;
				buffer[6] = (uint16_t)glob_para.spe_para[m_id].Param[j+1].shut_times;
				buffer[7] = 0;
				buffer[8] = glob_para.spe_para[m_id].Param[j+1].amout;
				buffer[9] = glob_para.spe_para[m_id].Param[j+1].exposure;
				buffer[10] = glob_para.spe_para[m_id].Param[j+1].interval;
				buffer[11] = glob_para.spe_para[m_id].delay_t;
				buffer[12] = glob_para.spe_para[m_id].manul;
				buffer[13] = sp_start;
				buffer[14] = j+1;
				buffer[15] = m_id;
				buffer[16] = (uint16_t)glob_para.spe_para[m_id].Param[j+1].sys_stop;
				buffer[17] = glob_para.spe_para[m_id].speed_range;
				buffer[18] = check_sum_add(18, buffer);
				Package_dataBufer(19,buffer);
				//memcpy(&Usart11_shadow[j].usart11_shadow_buffer[0], buffer, 19); // load data to DMA send buffer
				//Usart11_shadow[j].shadow_send_size = 19;
			//	Usart11_Interval_set();
			}
		}
		
		Alon_sendt = glob_para.spe_para[m_id].para_id;

}

// 参数设置取消时的数据返回用于同步参数
void Cancel_the_para_set_return(uint8_t indx)
{
	uint8_t buffer[20] = {0};

	if(indx>=PARA_MAX_ID)indx=PARA_MAX_ID-1;
	if(mid_ct>=MODE_ID_MAX)mid_ct =MODE_ID_MAX-1;
	
	buffer[0] = 9;
	buffer[1] = 0;
	buffer[2] = 9;
	buffer[3] = 7;
	buffer[4] = (uint16_t)glob_para.spe_para[mid_ct].Param[indx].lev_angle;
	buffer[5] = (uint16_t)glob_para.spe_para[mid_ct].Param[indx].lev_angle>>8;
	buffer[6] = (uint16_t)glob_para.spe_para[mid_ct].Param[indx].shut_times;
	buffer[7] = 0;
	buffer[8] = glob_para.spe_para[mid_ct].Param[indx].amout;
	buffer[9] = glob_para.spe_para[mid_ct].Param[indx].exposure;
	buffer[10] = glob_para.spe_para[mid_ct].Param[indx].interval;
	buffer[11] = glob_para.spe_para[mid_ct].delay_t;
	buffer[12] = glob_para.spe_para[mid_ct].manul;
	buffer[13] = sp_start;
	buffer[14] = indx;
	buffer[15] = mid_ct;
	buffer[16] = glob_para.spe_para[mid_ct].Param[indx].sys_stop;
	buffer[17] = glob_para.spe_para[mid_ct].speed_range;
	buffer[18] = check_sum_add(18, buffer);
	Package_dataBufer(19, buffer);
}

//发张数计数给遥控器显示
void specialty_mode_param_display(void)
{
	uint8_t bufers[13]={0};

	bufers[0] = 0x09;
	bufers[1] = 0x04;
	bufers[2] = 0x0a;
	bufers[3] = 0x05;
	
	bufers[4] = con_b.p_amount_back; //p_amount;
	bufers[5] = con_b.p_amount_back>>8; //p_amount>>8;
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
	bufers[9] = run_id;
	bufers[10] = con_b.begin;
	bufers[11] = check_sum_add(11, bufers);
	Package_dataBufer(12, bufers);
}

// 
void specialty_mode_main(void)
{
	uint8_t i;
	if(mode_backup != PREINSTALL_MODE)return;
	if(mid_ct>=MODE_ID_MAX)mid_ct =MODE_ID_MAX-1;
	if(con_b.begin == 3)
	{
		if(p_move_time >= con_b.interval_t)
		{
			if(glob_para.spe_para[mid_ct].manul == 0)
			{
				SHUTTER_ON;
				BLE_shut_start();
				Red_led_tack();
				p_move_time = 0;
				con_b.begin = 4;
			}
		}
	}
	else if(con_b.begin == 4)
	{
		if(p_move_time >= con_b.shut_time) // 快门时间到
		{
			SHUTTER_OFF;
			con_b.shut_time_b += 1;
			if(con_b.shut_time_b >= glob_para.spe_para[mid_ct].Param[run_id].shut_times)
			{
				con_b.shut_time_b = 0;
				p_amount += 1;
				con_b.p_amount_back += 1;
				if(Sync_sendt==0 && Alon_sendt==0)specialty_mode_param_display();
				con_b.begin = 5;
			}
			else
			{
				//con_b.shut_time_b += 1;
				con_b.begin = 3;
			}
			
			p_move_time = 0;
			//specialty_mode_compensation();
		}
	}
	else if(con_b.begin == 5)
	{
		if(p_move_time >= con_b.sys_stop) // 拍完照后停0.5秒后进入下一张拍照流程
		{
			if(p_amount < con_b.taltol_amount) // 
			{
				if(con_b.p_amount_back >= glob_para.spe_para[mid_ct].Param[run_id].amout) //一个府仰角度张数拍摄完毕，到下一个府仰角度
				{
					con_b.p_amount_back = 0;
					con_b.begin_back = 3; 
					con_b.begin = 0;
					p_move_time = 0;
					if(run_id < glob_para.spe_para[mid_ct].para_id)run_id +=1;
					///////////////////////////////////////////
				//	if(run_id < glob_para.spe_para[mid_ct].para_id)run_id +=1;
				//	con_b.p_amount_back = 1;
					for(i=BASE_SPEED; i < RAMP_MAX; i++)
					{
						if(SpecialtyHH_Ramp_Speed_Load(i, run_id))
						{
							break;				
						}
					}
					
					if(HHmotor_return_to_theOrigin_start()==0)
					{
					//	if(con_b.VVgo_back==0)
						{
							leve_turn_on_shutter();
							//con_b.begin_back = con_b.begin;
							//con_b.begin = 0;
							specialty_mode_param_display();
						}
					}
					con_b.shut_time = (uint16_t)glob_para.spe_para[mid_ct].Param[run_id].exposure * 1000;
					con_b.interval_t = (uint16_t)glob_para.spe_para[mid_ct].Param[run_id].interval * 1000;
					con_b.sys_stop = (uint16_t)glob_para.spe_para[mid_ct].Param[run_id].sys_stop * 1000;
					//con_b.interval_t = con_b.interval_t - con_b.shut_time;
					//HHmotor_return_to_theOrigin_start();
					//con_b.begin = 6;
				}
				else 
				{
					motorHH_p.DIR = A_TO_B;
					motorHH_direction_change(motorHH_p.DIR);
					SpHHorizontal_start();
					con_b.begin = 2;
					//String_Printf(USART_2, (uint8_t *)&glob_para.pulse_cout11, 4);
				}
			}
			else //整个拍摄完成
			{
				con_b.begin = 0;
				con_b.begin_back = 0;
				sp_start = 0;
				run_id =1;
				run_idVV = 0;
				con_b.time_down_count = 0;
				p_amount = 0;
			//	standard_mode_param_display();
				//con_b.go_back = 0xff;
				//con_b.HHgo_back = 0xff;
				//HHmotor_return_to_theOrigin_start();
				Panorama_mode_find_Apoint();
			//	if(find_pata.HHfind_Apoint ==0)
				{
					specialty_mode_param_display();
				}
				con_b.p_amount_back = 0;
			}
			
			p_move_time = 0;
		}
	}
}

// V 轴到点停止转动处理涵数
void Speialty_Vertical_pulse_check(void)
{
//	uint8_t tempss=0;
	if(mode_backup != PREINSTALL_MODE)return;
	if(find_pataVV.HHfind_Apoint)return;
	//con_b.VDynamic_pulse += 1;
	//if(con_b.VDynamic_pulse >= V_frame_pulse[run_id]) //府仰轴到达位置后由水平轴开始
	if(motorVV_p.pulse_count == sp_move_par[run_idVV].ele_pulse/*+compensation*/)
	{
		motorVV_stop();
		con_b.VDynamic_pulse = 0;
		con_b.p_upDown = 0;
		V_slow_tag = 0;
		if(con_b.begin == 7)
		{
			leve_turn_on_shutter();
		}

		if(con_b.VVgo_back && con_b.HHgo_back==0)
		{
			leve_turn_on_shutter();
		}
		con_b.VVgo_back = 0;
	}
 }


// H  轴到点停止转动处理涵数
void Speialty_Horizontal_pulse_check(void)
{
	//if(mode_backup != PREINSTALL_MODE)return;
	if(con_b.begin != 2)return;
	if(con_b.HHgo_back)return;
	
	con_b.Dynamic_pulse += 1;
	if(con_b.Dynamic_pulse >= sp_move_par[run_id].lev_pulse+con_b.compensation) //
	{
		motorHH_stop();
		motorHH_p.DIR = A_TO_B;
		motorHH_direction_change(motorHH_p.DIR);
		con_b.Dynamic_pulse = 0;
		
		con_b.begin = 3;
		p_move_time = 0;
	//	V_goBack = 2;
		//Horizontal_start();
		//con_b.Dynamic_pulse = 0;
		//con_b.begin = 2;
		//p_move_time = 0;
	}
}

//专业模式
void SpHHmotor_slowly_startedORstop(void)
{
	if(mode_backup != PREINSTALL_MODE)return;
	//if(H_slow_tag == 0)return;
	if(p_upORdown==0)return;
	//if(p_upORdown == check_speed_tag)return;
	if(slow_time)return;

	if(p_upORdown == SLOW_STOP)
	{
		//motor_speed_set(dynamic_speed);
		
		if(dynamic_speed < con_b.Ramp_id)
		{
			dynamic_speed++;
			motorHH_speed_set(con_b.ramp_buffer[dynamic_speed]);
		//	motorVV_speed_set(con_b.Vramp_buffer[dynamic_speed]);
		}
		else 
		{
			p_upORdown = 0;
			
		//String_Printf(USART_2, (uint8_t *)&glob_para.pulse_cout, 4);
		//	time_delay_us(10000);
			//String_Printf(USART_2, (uint8_t *)&glob_para.pulse_cout11, 4);
			
		}
	}
	else if(p_upORdown == SLOW_START)
	{
		//motor_speed_set(dynamic_speed);
		
		if(dynamic_speed > 0)
		{
			dynamic_speed--;
			motorHH_speed_set(con_b.ramp_buffer[dynamic_speed]);
			//motorVV_speed_set(con_b.ramp_buffer[dynamic_speed]);
		//	String_Printf(USART_2, (uint8_t *)&con_b.ramp_buffer[dynamic_speed], 2);
		}
		else 
		{
			p_upORdown = 0;
		//	String_Printf(USART_2, (uint8_t *)&con_b.Dynamic_pulse, 4);
			//String_Printf(USART_2, (uint8_t *)&con_b.slow_point, 4);
		}
	}
	
	set_slow_time_data();
}


void SpVVmotor_slowly_startedORstop(void)
{
	if(mode_backup != PREINSTALL_MODE)return;
	//if(V_slow_tag == 0)return;
	if(con_b.p_upDown==0)return;
	//if(p_upORdown == check_speed_tag)return;
	if(con_b.slw_time)return;

	if(con_b.p_upDown == SLOW_STOP)
	{
		//motor_speed_set(dynamic_speed);
		
		if(con_b.dynamic < con_b.VRamp_id)
		{
			con_b.dynamic++;
			motorVV_speed_set(con_b.Vramp_buffer[con_b.dynamic]);
		//	motorVV_speed_set(con_b.Vramp_buffer[dynamic_speed]);
		}
		else 
		{
			con_b.p_upDown = 0;
			V_slow_tag = 0;
		//String_Printf(USART_2, (uint8_t *)&glob_para.pulse_cout, 4);
		//	time_delay_us(10000);
			//String_Printf(USART_2, (uint8_t *)&glob_para.pulse_cout11, 4);
			
		}
	}
	else if(con_b.p_upDown == SLOW_START)
	{
		//motor_speed_set(dynamic_speed);
		
		if(con_b.dynamic > 0)
		{
			con_b.dynamic--;
			motorVV_speed_set(con_b.Vramp_buffer[con_b.dynamic]);
			//motorVV_speed_set(con_b.ramp_buffer[dynamic_speed]);
		//	String_Printf(USART_2, (uint8_t *)&con_b.ramp_buffer[dynamic_speed], 2);
		}
		else 
		{
			con_b.p_upDown = 0;
			V_slow_tag = 0;
		//	String_Printf(USART_2, (uint8_t *)&con_b.Dynamic_pulse, 4);
			//String_Printf(USART_2, (uint8_t *)&con_b.slow_point, 4);
		}
	}
	
	SpVVset_slow_time_data();
}

// 需要缓起缓停的区间速度缓冲区
// type: 1表示需要计算时间，0 表示不需要计算时间
void SpVVRamp_Speed_Load(uint32_t speed)
{
	uint8_t i;
	uint16_t timeb = 0;
	
	con_b.Vslow_point = 0;
	con_b.Vramp_buffer[0] = speed;
	for(i=1; i<15; i++)
	{
		timeb = speed + (i + 1) * i / 2;
		if(timeb > SLOW_THRESHOLD)
		{
			con_b.VRamp_id = i-1;
			break;
		}
		
		con_b.Vramp_buffer[i] = timeb;
		con_b.Vslow_point += SLOW_BASIC_TIME00 / ((float)(con_b.Vramp_buffer[i] + 1) * (float)501 / (float)48000);
		con_b.VRamp_id = i;
	}
	
	//con_b.slow_point +=160;
}

// 换参数组拍摄找完原点后的继续，此涵数处理找到原点后的开始正式拍摄 
void FindOrigin_over_to_Sart(void)
{
	if((H_goBack == 2 && V_goBack == 2)||(H_goBack == 2 && V_goBack == 0)||(H_goBack == 0 && V_goBack == 2))
	{
		H_goBack = 0;
		V_goBack = 0;
		if(sp_start == 0)return;
		if(mid_ct>=MODE_ID_MAX)mid_ct =MODE_ID_MAX-1;
		if(p_amount < glob_para.spe_para[mid_ct].total_aount)
		{
			String_Printf(USART_2,"FFFF",4);
			SHUTTER_ON;
			BLE_shut_start();
			con_b.begin = 5;
			p_move_time = 0;
			delay_cout = 0;
		}
	}
}

// 府仰电机到达慢停时触发此涵数
void SpVVertical_slow_check(void)
{
	if(mode_backup != PREINSTALL_MODE )return;
//	if(con_b.begin != 1)return;
	if(find_pataVV.HHfind_Apoint)return;
	
	//if(sp_move_par[run_idVV].ele_dir == A_TO_B)
	if(motorVV_p.DIR == A_TO_B)
	{
		if(motorVV_p.pulse_count == (sp_move_par[run_idVV].ele_pulse - con_b.Vslow_point))
		{
			SpVVslider_upORdown(SLOW_STOP);
			V_slow_tag = 1;
			SpVVset_slow_time_data();
		}
	}
	else if(motorVV_p.DIR == B_TO_A)
	{
		if(motorVV_p.pulse_count == (sp_move_par[run_idVV].ele_pulse + con_b.Vslow_point))
		{
			SpVVslider_upORdown(SLOW_STOP);
			V_slow_tag = 1;
			SpVVset_slow_time_data();
		}
	}
	
}

// 水平电机到达慢停时触发此涵数
void SpHHorizontal_slow_check(void)
{
	//if(mode_backup != PREINSTALL_MODE)return;
	//if(con_b.begin != 2)return;
	if(con_b.Dynamic_pulse == (sp_move_par[run_id].lev_pulse - con_b.slow_point))
	{
		slider_upORdown(SLOW_STOP);
		set_slow_time_data();
	}
}

void Horizontal_pulse_goBack(void)
{
	//if(mode_backup != PREINSTALL_MODE)return;
	if(con_b.HHgo_back == 0)return;

	if(motorHH_p.pulse_count == ORIGIN_POINT)
	{
		motorHH_stop();
		con_b.HHgo_back = 0;
	//	if(con_b.VVgo_back==0)
		{
		//	leve_turn_on_shutter();
			//con_b.begin_back = con_b.begin;
			//con_b.begin = 0;
			specialty_mode_param_display();
		}
		//String_Printf(USART_2, (uint8_t *)&glob_para.pulse_cout11, 4);
	}
	else
	{
		if(motorHH_p.pulse_count == (ORIGIN_POINT + con_b.slow_point))
		{
			if(motorHH_p.DIR == B_TO_A)
			{
				slider_upORdown(SLOW_STOP);
			//	String_Printf(USART_2,"FF",2);
			}
		}
		else if(motorHH_p.pulse_count == (B_POINT_END - con_b.slow_point))
		{
			if(motorHH_p.DIR == A_TO_B)
			{
				slider_upORdown(SLOW_STOP);
			//	String_Printf(USART_2,"EE",2);
			}
		}
	}
}

// 手动快门处理，即电动模式开时，手按快门键
void Spe_manul_Shuting(void)
{
	if(mid_ct>=MODE_ID_MAX)mid_ct =MODE_ID_MAX-1;
	if(glob_para.spe_para[mid_ct].manul)
	{
		SHUTTER_ON;
		BLE_shut_start();
		con_b.begin = 4;
		p_move_time = 0;
	}
	else
	{
		if(con_b.begin != 1 && con_b.begin != 2 && con_b.begin!=3)
		{
			BLE_shut_start();
		}
	}
}

// 专业模式下的脉冲补尝
void specialty_mode_compensation(void)
{
	if(mode_backup != PREINSTALL_MODE)return;
	
	con_b.diff += (con_b.remainder_p[run_id] - (double)H_frame_pulse[run_id]);
	
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

// 专业模式清零参数
void spe_mode_clear(void)
{
	con_b.begin = 0;
	con_b.begin_back = 0;
	sp_start = 0;
	con_b.p_amount_back = 0;
	run_id =0;
	run_idVV = 0;
	con_b.time_down_count = 0;
	delay_cout = 0;
	
	run_idVV = 0;
}

// 将专业模式里的所有参数发给遥控器显示
void Sync_data_for_controller(void)
{
//	uint8_t i=0;
	if(mode_backup != PREINSTALL_MODE)return;
	if(Sync_sendt == 0)return;
	if(Alon_sendt)return;

	Specialty_Send_data(Sync_sendt);
	if(Sync_sendt)Sync_sendt -=1;
	if(Sync_sendt==0)Specialty_modeID_sendt();
//	Alon_sendt = 1;
}

// 水平轴每张相片移动的RAMP
// 需要缓起缓停的区间速度缓冲区
uint8_t SpecialtyHH_Ramp_Speed_Load(uint32_t speed, uint8_t idx)
{
	uint8_t i;
	uint16_t timeb = 0;
	
	con_b.slow_point = 0;
	con_b.ramp_buffer[0] = speed;
	for(i=1; i<20; i++)
	{
		timeb = speed + (i + 1) * i / 2;
		if(timeb > RAMP_MAX)
		{
			con_b.Ramp_id = i-1;
			break;
		}
		
		con_b.ramp_buffer[i] = timeb;
		con_b.slow_point += SLOW_BASIC_TIME33 / ((float)(con_b.ramp_buffer[i] + 1) * (float)501 / (float)48000);
		con_b.Ramp_id = i;
	}
	
	con_b.rampp_time = SLOW_BASIC_TIME33 * (con_b.Ramp_id-1);

	if(sp_move_par[idx].lev_pulse < (con_b.slow_point*2))
	{
		return 0;
	}
	
//tring_Printf(USART_2,"DD",2);
//tring_Printf(USART_2,(uint8_t * )&con_b.slow_point,4);
	return 1;
	
}

void Specialty_loop_check(void)
{
	if(mode_backup != PREINSTALL_MODE)return;
	Panorama_find_end_point_checkStop(MOTOR_HORITAL);
	//Panorama_find_end_point_slowChcek(MOTOR_HORITAL);
	Horizontal_pulse_goBack();
	Speialty_Horizontal_pulse_check();
	SpHHorizontal_slow_check();
}

void Specialty_Key_start(void)
{
	if(mode_backup != PREINSTALL_MODE)return;
	if(sp_start)return;
	sp_start = 0xff;
	
	Specialty_mode_Start();
	Specialty_modeID_sendt();
}

void manual_goBack(void)
{
	if()
	motorHH_p.DIR = B_TO_A;
	motorHH_direction_change(motorHH_p.DIR);
	SpHHorizontal_start();
	con_b.begin = 2;
}

