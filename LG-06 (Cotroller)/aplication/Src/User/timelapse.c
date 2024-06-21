
#include "common.h"
#include "gpio.h"
#include "motor_dri.h"

uint8_t compensation = 0;
uint8_t move_begin_backup =0;
uint8_t move_begin = 0;
uint8_t downcount = 0;
uint8_t slider_dir_backup = 0;
uint8_t delay_time_set = 0;

uint16_t MS_count=0;

uint32_t move_distance =0;
uint32_t p_move_time  = 0;
uint32_t p_amount=0;
uint32_t move_time_bakeup=0;
uint32_t stop_time_bakeup = 0;
uint32_t sys_stop_time = 0;
uint32_t fast_time_bakeup=0;
uint32_t frames_time_back = 0;

float diff =0.0;
float remainder_p = 0.0;

void time_lapse_clear_para(void)
{
	move_distance = 0;
	compensation = 0;
	p_amount = 0;
	move_begin_backup = 0;
	p_move_time = 0;
	move_begin = 0;
	diff = 0;
	downcount = 0;
	delay_start = 0;
	
	shutter_action(SHUT_OFF);
}

void timelapse_downcount(void)
{
	if(move_begin>1)if(p_move_time<0xffffffff)p_move_time++;
	if(delay_begin_time)delay_begin_time--;
}

void Get_amount(void)
{
	con_b.shoot_amount = (glob_para.playback_s + glob_para.playback_m * 60 + glob_para.playback_h * 3600) * glob_para.fps;
}

//每张脉冲的补尝涵数
void compensation_for_timelapse(void)
{
	diff += (remainder_p - (float)move_time_bakeup);
	
	if(diff > 1.0)
	{
		compensation = 1;
		diff -=1.0;
	}
	else
	{
		compensation = 0;
	}
}
// draw mode and delay mode used this function
void delay_move_time_compara(void)
{
	if(move_begin == 1)
	{
		move_distance++;
		if( move_distance >= (move_time_bakeup + compensation))
		{
			//print_word_data(p_move_time);
			motor_stop();
			move_begin = 2;
			p_move_time = 0;
			move_distance=0;
		}
	}
}


void time_lapse_delay_1SBegin(void)
{
	if(delay_begin_time == 1)
	{
		delay_begin_time = 0;
		fisrt_time_begin();
	}
}

void delay_mode_main(void)
{
	if(mode_backup != DELAY_MODE)return;
	time_lapse_delay_1SBegin();

	if(delay_start==0)return;
	if(move_begin==0)return;
	
	if(move_begin == 2)
	{
		if(p_move_time >= sys_stop_time)
		{
			p_amount+=1;
			shutter_action(SHUT_ON);
			p_move_time = 0;
			move_begin = 3;
		}
	}
	else if(move_begin == 3)
	{
		if(p_move_time >= fast_time_bakeup)
		{
			compensation_for_timelapse();
			shutter_action(SHUT_OFF);
			
			if(p_amount >= con_b.shoot_amount)
			{					
				if(slider_dir_backup == glob_para.slider_dir)
				{
					glob_para.slider_dir = ~glob_para.slider_dir;
				}
				move_begin = 0;
				motor_stop();
				m_start = 0;
				delay_start = 0;
				//delay_send_pake(1);
				mcu_set_data_to_apk(0x06, p_amount);
				mcu_set_data_to_apk(0x09, 1);
				mcu_set_data_to_apk(0x09, 2);
				p_amount = 0;
				move_begin_backup = 0;
				//LED_ON;
				return;
			}
			p_move_time = 0;
			move_begin = 4;
		}
	}
	else if(move_begin == 4)
	{
		if(p_move_time >= stop_time_bakeup)
		{
			/*if(p_amount >= con_b.shoot_amount)
			{
				move_begin = 0;
				motor_stop();
				delay_start= 0;
				delay_send_pake(1);
				if(send_amount == 0)mcu_set_data_to_apk(0x04, p_amount);
				//p_amount = 0;
				move_begin_backup = 0;
				//LED_ON;
				return;
			}
			else*/
			{
				p_move_time = 0;
				
				motor_speed_adjust(DELAY_M_SPEED);

					move_begin = 1;
					motor_start();
			}
		}
	}
}

void shutter_action(uint8_t status)
{
	if(status == SHUT_ON)
	{
		SHUTTER_ON;
		LED_ON;
	}
	else if(status == SHUT_OFF)
	{
		SHUTTER_OFF;
		LED_OFF;
	}
}

void fisrt_time_begin(void)
{
//	if(check_start_point != 0xff)return;
	if(/*(page_id == DELAY_SET) || */(mode_backup == DELAY_MODE))
	{
	
		downcount = 1;
		slider_dir_backup = glob_para.slider_dir;
		motor_speed_adjust(DELAY_M_SPEED);
		p_amount=0;
	//	if((glob_para.delay_time_h == 0) && (glob_para.delay_time_m == 0) && (glob_para.delay_time_s == 0))
		{
			delay_time_set = 0;
			shutter_action(SHUT_ON);
			p_amount+=1;
			mcu_set_data_to_apk(0x06, p_amount);
			p_move_time = 0;
			move_begin = 3;
		}
	/*	else
		{
			if(move_begin_backup)
			{
				move_begin = 5;
				//count_begin = 1;
			}
			else
			{
				//if(delay_time_set == 0)
				{
					delay_time_set = 1;
					if(move_begin_backup == 0)get_count_down_data();
					//p_move_time = (glob_para.delay_time_h * 3600 + glob_para.delay_time_m * 60) * 1000;
					move_begin = 5;
					
					//count_begin = 1;
				}
			}
		}*/
		
	}
}

void get_count_down_data(void)
{
	uint32_t p_pix_time =0;
	/*if(delay_time_set) // 当延时有设置时间时先倒计延后时间再显示拍摄时长
	{
		//p_pix_time = glob_para.delay_time_h * 3600 + glob_para.delay_time_m * 60;

		hours = glob_para.delay_time_h;
		minutes = glob_para.delay_time_m;
		seconds = glob_para.delay_time_s;
	}
	else // 显示拍摄时长*/
	Get_amount();
	{
		if(con_b.shoot_amount)
		{
			p_pix_time = (con_b.shoot_amount-1)*frames_time + expouse_time;
		}
	
		con_b.tatol_time_h = p_pix_time/60/60;
		con_b.tatol_time_m = p_pix_time / 60 - con_b.tatol_time_h *60;
		con_b.tatol_time_s  = p_pix_time- con_b.tatol_time_m*60 - con_b.tatol_time_h * 60 * 60;
		
		hours = con_b.tatol_time_h;
		minutes = con_b.tatol_time_m;
		seconds = con_b.tatol_time_s;
	}
	
}

uint32_t move_time_calculate(void)
{
	double pluse_basic_time=0; // the time of each pulse
	uint32_t move_time_p=0; // 
	uint32_t lenth_pulse = 0;
	uint32_t pix_num=0;
	
	lenth_pulse = Get_ab_lenght();
	if(lenth_pulse == 0)return 0;
	
	if(con_b.shoot_amount)pix_num = con_b.shoot_amount-1;
//	if(page_status == PAGE_FOUR)
	{
		//pluse_basic_time = (double)( speed_table[DELAY_M_SPEED-1] + 1) * (double)501 / (double)48000; //ms unit
		pluse_basic_time = (double)(speed_calculat(DELAY_M_SPEED) +1) * (double)501 / (double)48000; //ms unit

			if(lenth_pulse)
			{
				if(pix_num)
				{
					remainder_p = (float)lenth_pulse / (float)pix_num;
					move_time_bakeup = remainder_p;
					
					move_time_p = (remainder_p * pluse_basic_time);
				}
				else
				{
					move_time_p = 0;
					move_time_bakeup= 0;
					remainder_p = 0;
				}
			}
			else
			{
				move_time_p = 0;
				move_time_bakeup = 0;
				remainder_p = 0;
			}
	}
	
	return move_time_p;
}

// 启动时确认各参数：
// move_time_bakeup：滑轨移动的脉冲数
//stop_time_bakeup：曝光后的暂停时间让其完全关闭快门
//sys_stop_time：曝光前的暂停时间让其停稳后再开快门
// fast_time_bakeup：曝光的时间
void get_run_param(void)
{
	uint32_t move_time_p=0; // 
	uint32_t dynamic_frames=0;
	uint32_t static_frames=0;

	
	motor_speed_adjust(DELAY_M_SPEED);
	move_time_p =move_time_calculate();
	//fast_time_bakeup = glob_para.fast_time * 1000;
	//最后停止时间等于总停止时间(glob_para.stop_time即逐格时间)减去快门减固定1秒减电机移动时间
	fast_time_bakeup = expouse_time *1000; //glob_para.sl_sh_time_s + glob_para.sl_sh_time_m*60 + glob_para.sl_sh_time_h*60*60;
	//sys_stop_time = 100;
	//stop_time_bakeup = 100;

	dynamic_frames = move_time_p + 1200 + fast_time_bakeup;

	//dynamic_frames = move_time_p + 1500 + fast_time_bakeup; // 1500:曝光之前的1S+曝光之后的暂停0.5S
	static_frames = frames_time * 1000;
	
	if(static_frames <= dynamic_frames)
	{
		stop_time_bakeup = 200;
	}
	else 
	{
		stop_time_bakeup = static_frames - move_time_p - 1000 -fast_time_bakeup;
	}
	
	sys_stop_time = 1000;
	

	
}

void delay_confirm(void)
{
	if(con_b.shoot_amount == 0)return;
	
	if(delay_start) //轨道开始跑
	{
		write_flash_active();
		
		if(p_amount >= con_b.shoot_amount)
		{
			move_begin_backup = 0;
			p_amount=0;
			diff = 0;
			compensation = 0;
		}
		
		get_run_param();
		
		if(move_begin_backup)
		{
			downcount = 1;
			if(move_begin_backup == 1)
			{
					move_begin = move_begin_backup;
					motor_start();
			}
			else
			{
				move_begin = move_begin_backup;
			}
		}
		else
		{
			get_count_down_data();
			if(move_judge_ABpoint() == 0)
			{
				delay_begin_time = 0;
				fisrt_time_begin();
			}
		}
	}
	else //轨道停跑
	{
		move_begin_backup = 0;
		move_begin = 0;
		downcount = 0;
		p_amount = 0;
		SHUTTER_OFF;
		motor_stop();
	}
}

void timelapse_usart_get(uint8_t *sptb)
{
	if(mode_backup != DELAY_MODE)return;
	
	if(sptb[4] == 0x00) // fps
	{
		/*if(delay_start)
		{
			delay_send_pake(1);
			return;
		}*/
		time_lapse_clear_para();
		glob_para.fps = sptb[5];
		delay_para_compara();
		/*if(sptb[5] == 0x01)
		{
			glob_para.fps = 30;
		}
		else if(sptb[5] == 0x02)
		{
			glob_para.fps = 60;
		}
		else if(sptb[5] == 0x03)
		{
			glob_para.fps = 120;
		}*/

	//	max_amount_check();
	}
	else if(sptb[4] == 0x01) // playback time
	{
		/*if(delay_start)
		{
			delay_send_pake(1);
			return;
		}*/
		
		time_lapse_clear_para();
		glob_para.playback_s =  sptb[5];
		glob_para.playback_m = sptb[6];
		glob_para.playback_h = sptb[7];
		
		delay_para_compara();
	}
	else if(sptb[4] == 0x03) //frames time
	{
		/*if(delay_start)
		{
			delay_send_pake(1);
			return;
		}*/
		
		time_lapse_clear_para();
		glob_para.sl_f_time_s =  sptb[5];
		glob_para.sl_f_time_m = sptb[6];
		glob_para.sl_f_time_h = sptb[7];
		frames_time = glob_para.sl_f_time_s + glob_para.sl_f_time_m*60 + glob_para.sl_f_time_h*60*60;
	
		/*if(frames_time > 10799)
		{
			glob_para.sl_f_time_h = 2;
			glob_para.sl_f_time_m = 59;
			glob_para.sl_f_time_s = 59;
			frames_time = 10799;
			delay_send_pake(1);
		}*/
	
		delay_para_compara();
		frames_time_back = frames_time;
		//tatol_time_dis();
		if_write_flash();
	}
	else if(sptb[4] == 0x02) //shutter time
	{
		/*if(delay_start)
		{
			delay_send_pake(1);
			return;
		}*/
		time_lapse_clear_para();
		
		glob_para.sl_sh_time_s =  sptb[5];
		glob_para.sl_sh_time_m = sptb[6];
		glob_para.sl_sh_time_h = sptb[7];
		
		expouse_time = glob_para.sl_sh_time_s+glob_para.sl_sh_time_m*60+glob_para.sl_sh_time_h*3600;
		
		/*if(expouse_time > 7197)
		{
			glob_para.sl_sh_time_h = 1;
			glob_para.sl_sh_time_m = 59;
			glob_para.sl_sh_time_s = 57;
			expouse_time = 10797;
			delay_send_pake(1);
		}
		else */if(expouse_time < 1)
		{
			glob_para.sl_sh_time_h = 0;
			glob_para.sl_sh_time_m = 0;
			glob_para.sl_sh_time_s = 1;
			expouse_time = 1;
			delay_send_pake(1);
		}
		
		delay_para_compara();
		fast_time_bakeup = expouse_time * 1000;
		if_write_flash();
	}
	/*else if(sptb[4] == 0x03) //pirture numbers
	{
		if(delay_start)
		{
			delay_send_pake(1);
			return;
		}
		time_lapse_clear_para();
		glob_para.shoot_amount = sptb[5] | ( ((uint16_t)sptb[6])<<8 );
	
		max_amount_check();
		delay_para_compara(2);
		shoot_amount_back = glob_para.shoot_amount;
		//tatol_time_dis();
		if_write_flash();
	}*/
	else if(sptb[4] == 0x05) //direction
	{
		/*if(delay_start)
		{
			delay_send_pake(1);
			return;
		}*/
		
		glob_para.slider_dir = sptb[5];
		//TheDir_check();
		if(ab_point_change_dir()==0)
		{
			check_start_point = 0xff;
			motor_direction_change(glob_para.orbital_dir);
			slow_start_setting(100);
		}
		else
		{
			TheDir_check();
			//mcu_set_data_to_apk(0x09, 1);
			mcu_set_data_to_apk(0x09, 2);
		}
	}
	else if(sptb[4] == 0x04) //pause/play
	{
		if((glob_para.lenght) < 500)// a or b point error  , inter to reset
		{
			//ab_set_tag = 0;
			///glob_para.a_point = 0;
			////glob_para.b_point = 0;
			//glob_para.pulse_cout = 0;
			//mcu_set_data_to_apk(0x0b, 0xff);
			//m_start = 0;
			return;
		}
		
		write_flash_active();
		delay_start = sptb[5];
		delay_confirm();
	}
	else if(sptb[4] == 0x06) // auto ab point or customer ab point switch
	{
		if(sptb[5] == 0xff)
		{
			con_b.d_ab_mode = AUTO_ABPOINT;
		}
		else if(sptb[5] == 0)
		{
			con_b.d_ab_mode = CUSTOMER_AB;
		}
		if_write_flash();
		//Find_AOrB_point();
		delay_para_compara();
	}
}

uint8_t max_amount_check(void)
{
	uint16_t ative_amount = 0;
	uint16_t current_amount = 0;
	uint32_t ply_time = 0;

	ative_amount = Get_ab_lenght()/PIX_PULSE;
	current_amount = (glob_para.playback_h*3600 + glob_para.playback_m*60+glob_para.playback_s)*glob_para.fps;
	if(current_amount > ative_amount)
	{
		ply_time = ative_amount / glob_para.fps;
		current_amount = ative_amount;
		glob_para.playback_h = ply_time/60/60;
		glob_para.playback_m = ply_time / 60 - glob_para.playback_h *60;
		glob_para.playback_s = ply_time- glob_para.playback_m*60 - glob_para.playback_h * 3600;
		mcu_set_data_to_apk(7, 0);
		return 1;
	}

	con_b.shoot_amount = current_amount;
	return 0;
}

void delay_para_compara(void)
{
	//float one_frames=0;
	uint32_t temp =0;
	uint32_t pp_move_time = 0;
	uint32_t fast_times = 0;

	max_amount_check();
	
	fast_times = expouse_time * 1000;//(glob_para.sl_sh_time_s + glob_para.sl_sh_time_m*60 + glob_para.sl_sh_time_h*3600)*1000; // ms
	pp_move_time = move_time_calculate();
	
	temp = pp_move_time + 1200 + fast_times;
	
	
	if((frames_time * 1000) < temp )
	{
		pp_move_time = temp % 1000;
		
		if(pp_move_time)
		{
			frames_time = temp / 1000 + 1;
		}
		else{
			frames_time = temp / 1000;
		}
		
		glob_para.sl_f_time_h = frames_time/60/60;
		glob_para.sl_f_time_m = frames_time / 60 - glob_para.sl_f_time_h *60;
		glob_para.sl_f_time_s = frames_time- glob_para.sl_f_time_m*60 - glob_para.sl_f_time_h * 3600;
		mcu_set_data_to_apk(8, 0);
	}

	tatol_time_dis();

}

void tatol_time_dis(void)
{
/*	uint32_t p_pix_time=0;
	uint16_t p_hours=0;
	uint8_t p_minutes=0;
	uint8_t p_seconds=0;
//	uint8_t i;
	
	Get_amount();
	if(con_b.shoot_amount == 0)return;
	p_pix_time = (con_b.shoot_amount-1)*frames_time + expouse_time;

	p_hours = p_pix_time/60/60;
	p_minutes = p_pix_time / 60 - p_hours *60;
	p_seconds = p_pix_time- p_minutes*60 - p_hours * 3600;
	con_b.tatol_time_h = p_hours;
	con_b.tatol_time_m = p_minutes;
	con_b.tatol_time_s = p_seconds;
*/
	get_count_down_data();

	mcu_set_data_to_apk(0x05,0);
	
	//Usart22_Interval_set();
}

void delay_The_countdown(void)
{
	if(move_begin==0)return;
	if(downcount == 0)return;
	
	MS_count++;
	if(MS_count >= 1000)
	{
		MS_count = 0;
	//	if(delay_time_set == 0)
		{
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
			
			mcu_set_data_to_apk(0x06, p_amount);
			//if((hours == con_b.tatol_time_h) && (minutes == con_b.tatol_time_m) && (seconds == con_b.tatol_time_s))downcount = 0;
		}		
	}
}

void TimeLapse_Loop(void)
{
	uint32_t point_a = 0;
	uint32_t point_b = 0;
	
	if(mode_backup != DELAY_MODE)return;
	if(check_start_point)return;
	
	if(con_b.d_ab_mode == CUSTOMER_AB)
	{
		point_a = Point[mode_backup-2].A_point;
		point_b = Point[mode_backup-2].B_point;
	}
	else
	{
		point_a = glob_para.a_point;
		point_b = glob_para.b_point;
	}

	
	if(glob_para.pulse_cout == point_a || glob_para.pulse_cout == point_b)
	{
		if(glob_para.pulse_cout == point_a)
		{
			glob_para.slider_dir = A_TO_B;
		}
		else if(glob_para.pulse_cout == point_b)
		{
			glob_para.slider_dir = B_TO_A;
		}
		
		TheDir_check();
		/*if(check_start_point)
		{
			start_point_check();
			//mcu_set_data_to_apk(0x09, 2);
			return;
		}*/
		
		motor_stop();
		if(move_begin)return;
		time_lapse_clear_para();
		mcu_set_data_to_apk(0x09, 1);
		
	}
}

