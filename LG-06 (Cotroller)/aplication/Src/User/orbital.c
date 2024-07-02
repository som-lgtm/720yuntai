#include "stdio.h"
#include "oled.h"
#include "common.h"
#include "keyscan.h"
#include "orbital.h"
#include "config.h"
#include "stm32f0xx_ll_tim.h"
#include "uart.h"
#include "gpio.h"

PARA_STRUCT glob_para ={0};
ID_CODE wifi_id={0};

uint8_t D_amount[4]={0};
uint8_t page_id_back = 0;
uint8_t specialty_get = 0;
uint8_t std_manul_tag = 0;
uint8_t wid_manul_tag = 0;
uint8_t move_begin =0;
uint8_t m_start = 0;
uint8_t move_begin_backup=0;
uint8_t reverse_cur = 0;
uint8_t minutes =0;
uint8_t seconds =0;
uint8_t check_start_point =0;
uint8_t ramp_flag=0;
uint8_t ramp_flag11 = 0;
uint8_t move_mode=0;
uint8_t locus_numb = 0;
uint8_t Inverse_Dis=0;
uint8_t adj_parm_sel = 0;
uint8_t delay_time_set = 0;
uint8_t video_fisrt_time = 0;
uint8_t manual_flag = 0;
uint8_t get_init_tag = 0;
uint8_t fps_buffer[3]={0};
uint16_t get_init_time = 0;

uint16_t delay_speed =0;
uint16_t led_time=0;
uint16_t sub_contunue=0;
uint16_t add_contunue=0;
uint16_t MS_count=0;
uint16_t p_amount =0;
uint16_t p_move_step = 0;
uint16_t hours =0;
uint16_t addend_value=0;
uint16_t connection_flag = 0;
uint16_t V_agnle = 0;
uint16_t H_agnle = 0;

uint32_t fast_time_bakeup =0;
uint32_t stop_time_bakeup = 0;
uint32_t move_time_bakeup = 0;
uint32_t p_move_time = 0;
uint32_t sys_stop_time = 0;
uint32_t dynamic_speed=0;
uint32_t time_limit = 0; // ms
uint32_t boot_time=0;

//uint16_t *stp;

float value_add=0.0;

void parameter_init(void)
{
	dynamic_speed = 80;
	addend_value = 1;
	value_add = 0.1;
	connection_set();
	fps_buffer[0]=30;
	fps_buffer[1]=60;
	fps_buffer[2]=120;
	D_amount[0]=glob_para.shoot_amount % 10; //个
	D_amount[1]=glob_para.shoot_amount/10 % 10; //十
	D_amount[2]=glob_para.shoot_amount/100 % 10; // 百
	D_amount[3]=glob_para.shoot_amount/1000 % 10; // 千
}

void time_count(void)
{
	if(boot_time)boot_time--;
	if(led_time)led_time--;
	if(get_init_time)get_init_time--;
//	delay_The_countdown();
	//tick_count();
}

void set_addend_data(uint16_t pvalue, float fvalue)
{
/*	addend_value = pvalue;
	value_add = fvalue;*/
}

void Time_DCount(void)
{
	if(connection_flag)connection_flag--;
}

void battery_data_display11(float  data)
{
	uint8_t buff[10];
	

	
	if(data < 10)sprintf( (char *)buff,"%0.2fV ", data);
	else sprintf( (char *)buff,"%0.2fV", data);
	
	//oled_display_6x8str(71, 0, buff);
	Oled_EnlPrint(72, 4, buff, "", ENGLISH);
}

void move_speed_calculat(void)
{}


//  检查是否需要慢启慢停
void slow_check(void)
{}


void cursor_glint(void)
{
	static uint8_t tempp=0;
	//uint8_t cur_back=0;
	
	if(adj_parm_sel)
	{
		if(led_time)return;
		
		led_time = 500;
		tempp = ~tempp;

		if(tempp)inverse_get_value(0xff);
		else inverse_get_value(0);

		if(page_id == DELAY_SET)
		{
			if(cursor_id == 3)
			{
				Delay_pix_amt_display(cursor_id, adj_parm_sel-1);
				//if(adj_parm_sel == 1)Delay_pix_amt_display(cursor_id, adj_parm_sel-1);
				//else if(adj_parm_sel == 2)durationORdelay_time_dis( glob_para.fram_time_m, cursor_id);
				//else if(adj_parm_sel == 3)durationORdelay_time_dis( glob_para.fram_time_s, cursor_id);
			}
		}
		else if(page_id == VIDEO_MODE)
		{
			if(adj_parm_sel == 1)durationORdelay_time_dis( glob_para.duration_h, cursor_id);
			else if(adj_parm_sel == 2)durationORdelay_time_dis( glob_para.duration_m, cursor_id);
			else if(adj_parm_sel == 3)durationORdelay_time_dis( glob_para.duration_s, cursor_id);
		}
		else if(page_id == MANUAL_MODE)
		{
			param_display(2);
		}
	}
}

////////////////////// ORBITAL /////////////////////////////////////////


uint16_t data_count(uint16_t data, uint8_t dir, uint16_t max, uint8_t mix)
{
	if(dir & KEY_RIGHT_MASK)
	{
		if(data < max)
		{
			data = data + addend_value;
			//data = data + addend_value;
		}
		else
		{
			if(page_id == PREINSTALL_SET1)data = mix;
		}
	}
	else if(dir & KEY_LEFT_MASK)
	{
		if(data > mix)
		{
			//if(data)data--;
			
			if(data >= addend_value)data = data - addend_value;
		}
		else
		{
			if(page_id == PREINSTALL_SET1)data = max;
		}
	}

	return data;
}

void ab_point_change_dir(void)
{
	if(m_start == 0)return;
	
	if(glob_para.pulse_cout<= glob_para.a_point)
	{
		glob_para.orbital_dir = A_TO_B;
		motor_direction_change();
	}
	else if(glob_para.pulse_cout >= glob_para.b_point)
	{
		glob_para.orbital_dir = B_TO_A;
		motor_direction_change();
	}
}

void video_page_OK(void)
{	
	if(page_id != VIDEO_MODE)return;
		if(cursor_id == 4)
		{
			m_start = ~m_start;
			inverse_get_value(0xff);
			param_display(cursor_id);
			inverse_get_value(0);

			controller_send_data_to_motor(0x00,0x01, 0x05);
		//	Send_data_to_ER1(0x00,0x02,0x02);
			//if(m_start)Send_data_to_ER1(0x00,0x02,0x04);
			//else Send_data_to_ER1(0x00,0x02,0x02);
		}
		else if(cursor_id == 5)
		{
			if(m_start)return;
			set_mode_back(page_id);
			page_id = REVERSE_MODE;
			cursor_id = 3;
			
			change_page();
		//	controller_send_data_to_motor(0x00,0x01, 0x02);
		}
		else if(cursor_id == 2)
		{
			//if(m_start)return;
			if(adj_parm_sel==0)adj_parm_sel =2;
			else if(adj_parm_sel)adj_parm_sel=0;
		//	String_Printf(&adj_parm_sel,1);
			inverse_get_value(0xff);
			hours_time_dis(glob_para.duration_h, cursor_id);
			minutes_time_dis(glob_para.duration_m, cursor_id);
			secondes_time_dis(glob_para.duration_s, cursor_id);
			inverse_get_value(0);
			//if(adj_parm_sel == 1)stp = (uint16_t *)&glob_para.delay_time_h;
			//else if(adj_parm_sel == 2)stp = (uint16_t *)&glob_para.delay_time_m;
				
		}
	

}


float move_time_cout(uint8_t dir, float data, float p_range)
{
	if(dir == KEY_LEFT_MASK)
	{
		if(data >= 0.1)
		{
			if(data >= value_add)data -= value_add;
		}
		else
		{
		//	data = p_range;
		}

		if(data <= 0.1)data=0.1;
	}
	else if(dir == KEY_RIGHT_MASK)
	{
		if(data < p_range)
		{
			data += value_add;
		}
		else
		{
			//data = 0.0;
		}

		if(data>p_range)data = p_range;
	}

	return data;
}

// 小数点加减数
float  para_count(uint8_t dir, float data, uint16_t max, uint8_t mix)
{
	if(dir & KEY_RIGHT_MASK)
	{
		if(data < max)
		{
			//data++;
			data = data + 0.5;
			//if(data > 59)data = 59;
		}
		else
		{
			//data = mix;
		}
	}
	else if(dir & KEY_LEFT_MASK)
	{
		if(data > mix)
		{
			//data--;
			
			if(data >= 0.5)data = data - 0.5;
		}
		else
		{
			//data = max;
		}
	}

	return data;
}

// 启动时确认各参数：
// move_time_bakeup：滑轨移动的脉冲数
//stop_time_bakeup：曝光后的暂停时间让其完全关闭快门
//sys_stop_time：曝光前的暂停时间让其停稳后再开快门
// fast_time_bakeup：曝光的时间
void get_run_param(void)
{}

void delay_time_over(void)
{
	if(page_id != DELAY_DIS)return;
	
	if(delay_time_set)
	{
		delay_time_set = 0;
//		total_time_display();
		
//		SHUTTER_ON;
//		SHUTTER_ON11;

//		LED_ON;
//		LED_ON11;
		p_move_time = 0;
		move_begin = 3;
	}
}


void adv_time_OK(void)
{}

void delay_dis_interface(void)
{
	//LCD_Fill(0);  //初始清屏
	Task_Percentage_Dis(0);
	pix_amt_display(2);
	delay_move_page_dis();
	status_display(3);
	single_cursor_dis(cursor_id);
}

void delay_page_ok(void)
{	
	if(page_id == DELAY_SET)
	{
		if(cursor_id == 5)
		{
			m_start = 0xff;
			page_id = DELAY_DIS;
			cursor_id = 5;
			change_page();
			Time_DownCount(3);
			//delay_dis_interface();
			//delay_begin_function();
			LCD_Fill(0, 140, LCD_W-1, 160, BLACK);
			controller_send_data_to_motor(0x00,0x02, 0x06);
			
		}
		else if(cursor_id == 6)
		{
			if(m_start)return;
			set_mode_back(page_id);
			page_id = REVERSE_MODE;
			cursor_id = 3;
			
			change_page();
		}
		else if(cursor_id == 3)
		{
			if(m_start)return;
			if(adj_parm_sel==0)adj_parm_sel = 3;
			else adj_parm_sel=0;
			if(adj_parm_sel==0)
			{
				inverse_get_value(0xff);
				param_display(3);
				inverse_get_value(0);
			}
		}
	}
	else if(page_id == DELAY_DIS)
	{
		if(cursor_id == 5)
		{
			m_start =~m_start;

			controller_send_data_to_motor(0x00,0x02, 0x06);
			param_display(5);
		}
		else if(cursor_id == 6)
		{
			m_start = 0;
			controller_send_data_to_motor(0x00,0x02, 0x06);
			page_id = DELAY_SET;
			cursor_id = 5;
			change_page();
		}
	}

}

void countdown_display(uint8_t x, uint8_t param, uint8_t cur)
{
	uint8_t buff[5];

	if(param < 10)
	{
		sprintf( (char *)buff,"0%d", param);
	}
	else if(param < 100)
	{
		sprintf( (char *)buff,"%d", param);
	}

	Oled_EnlPrint(x, cur<<1, buff, "", ENGLISH);
}

void delay_The_countdown(void)
{
	if(move_begin)MS_count++;

	if(MS_count >= 1000)
	{
		MS_count = 0;

		if(delay_time_set == 0)
		{
			if(seconds < 59)
			{
				seconds +=1;
				countdown_display(48,seconds, TDINDEX_ONE);
			}
			else
			{
				seconds = 0;
				
				if(minutes < 59)
				{
					minutes +=1;
					countdown_display(24,minutes, TDINDEX_ONE);
					countdown_display(48,seconds, TDINDEX_ONE);
				}
				else
				{
					minutes = 0;
					if(hours < glob_para.tatol_time_h)
					{
						hours +=1;
					}

					countdown_display(0,hours, TDINDEX_ONE);
					countdown_display(24,minutes, TDINDEX_ONE);
				}
			}
		}
		else 
		{
			if(seconds)
			{
				seconds -= 1;
				countdown_display(48,seconds, TDINDEX_ONE);
			}
			else
			{
				if(minutes)
				{
					minutes -=1;
					seconds = 59;
					countdown_display(24,minutes, TDINDEX_ONE);
					countdown_display(48,seconds, TDINDEX_ONE);
				}
				else
				{
					if(hours)
					{
						hours -=1;
						minutes = 59;
						countdown_display(0,hours, TDINDEX_ONE);
						countdown_display(24,minutes, TDINDEX_ONE);
					}
					else
					{
						delay_time_over();
					}
				}
			}
		}

	}
}

void delay_move_time_compara(void)
{
	if(move_begin == 1)
	{
		p_move_step++;
		if( p_move_step >=  move_time_bakeup)
		{
			//print_word_data(p_move_time);
			p_move_time = 0;
			//if(page_id == DELAY_DIS)move_begin = 2;
			//else if(page_id == VIDEO_CUSTOM_ID)move_begin=0;
			p_move_step = 0;
			move_begin = 2;
		}
	}
}



void delay_mode_main(void)
{
	cursor_glint();
	factory_cofirm();
}


void delay_page_return(void)
{
	move_begin = 0;
	p_move_time = 0;
	p_amount = 0;
	m_start = 0;
	move_begin_backup = 0;
	p_move_step = 0;
//	SHUTTER_OFF;
//	SHUTTER_OFF11;
//	LED_OFF;
//	LED_OFF11;
	MS_count = 0;
	check_start_point = 0;
	adj_parm_sel = 0;
	delay_time_set = 0;
	seconds = 0;
	minutes = 0;
	hours = 0;
	video_fisrt_time = 0;
	//glob_para.stop_shoot = 0;
	get_data_form_A650();
}

void press_key_continue_if(uint8_t dir)
{
	if((page_id == DELAY_SET)||(page_id == PREINSTALL_SET1)||(page_id == PREINSTALL_MOVE) ||(page_id == SINGLE_PANORAMA) ||(page_id == SINGLE_PANORAMA_11) )
	{
	/*	if(dir & KEY_UP_MASK)
		{
			add_contunue = 1001;
		}
		else if(dir & KEY_DOWN_MASK)
		{
			sub_contunue = 1001;
		}
		else */if(dir & KEY_LEFT_MASK)
		{
			//sub_contunue = 1201;
			add_contunue = 1201;
		}
		else if(dir & KEY_RIGHT_MASK)
		{
			add_contunue = 1201;
		}
		else
		{
			add_contunue = 0;
			sub_contunue = 0;
		}
	}
	if((page_id == CONFIG_ID))
	{
		if(dir & KEY_RIGHT_MASK)
		{
			add_contunue = 7000;
		}
		else if(dir & KEY_LEFT_MASK)
		{
			sub_contunue = 7000;
			add_contunue = 7000;
		}
		else
		{
			add_contunue = 0;
			sub_contunue = 0;
			set_addend_data(1, 0.1);
		}
	}
	
}

void Time_DownCount(uint8_t cur)
{
	uint8_t buff[6];
	
	if(hours < 10)
	{
		sprintf( (char *)buff,"0%d", hours);
	}
	else if(hours < 100)
	{
		sprintf( (char *)buff,"%d", hours);
	}
	
	Oled_EnlPrint(SCREEN_MIGRATION+4, cur, buff, " ", ENGLISH);
	LCD_shi_fen_miao_display(SCREEN_MIGRATION+4+12, cur, "H");
	
	if(minutes < 10)
	{
		sprintf( (char *)buff,"0%d", minutes);
	}
	else if(minutes < 100)
	{
		sprintf( (char *)buff,"%d", minutes);
	}
	
	Oled_EnlPrint(SCREEN_MIGRATION+18+4, cur, buff, " ", ENGLISH);
	LCD_shi_fen_miao_display(SCREEN_MIGRATION+18+4+12, cur, "M");
	
	if(seconds < 10)
	{
		sprintf( (char *)buff,"0%d /", seconds);
	}
	else if(seconds < 100)
	{
		sprintf( (char *)buff,"%d /", seconds);
	}
	
	Oled_EnlPrint(SCREEN_MIGRATION+36+4, cur, buff, " ", ENGLISH);
	LCD_shi_fen_miao_display(SCREEN_MIGRATION+36+4+12, cur, "S");
}

// 
void param_display(uint8_t cur)
{
	switch(page_id)
	{
		case DELAY_SET:
		{
			if(cur == 1) // frames
			{
				//locus_disp();
				//hours_time_dis(glob_para.fram_time_h, cur);
				//minutes_time_dis(glob_para.fram_time_m, cur);
				secondes_time_dis(glob_para.interval, cur);
				
			}
			else if(cur == 2) // shutter time
			{
				//locus_disp();
				//hours_time_dis(glob_para.shut_time_h, cur);
				//minutes_time_dis(glob_para.shut_time_m, cur);
				secondes_time_dis(glob_para.exposure, cur);
			}
			else if(cur == 3) // 
			{
				//pix_amt_display(cur);
				Delay_pix_amt_display(cur, 0);
				Delay_pix_amt_display(cur, 1);
				Delay_pix_amt_display(cur, 2);
				Delay_pix_amt_display(cur, 3);
			}
			else if(cur == 7)
			{
			////	hours_time_dis(glob_para.tatol_time_h, cur);
			//	minutes_time_dis(glob_para.tatol_time_m, cur);
			//	secondes_time_dis(glob_para.tatol_time_s, cur);
			}
			else if(cur == 5)
			{
				status_display(cur);
			}
			if(cur == 4)
			{
				if(cursor_id == cur)inverse_get_value(0xff);
				move_to_disp(cur);
				inverse_get_value(0);
			}
		/*	else if(cur == 7)
			{
				tatol_hours_time_dis(glob_para.tatol_time_h, cur);
				minutes_time_dis(glob_para.tatol_time_m, cur);
				secondes_time_dis(glob_para.tatol_time_s, cur);
			}*/

			break;
		}
		case DELAY_DIS:
		{
	//		if(lowbattery_dis_if)return; // 正在显示低电量时不要显示动态延时界面数据
			
			if(cur == 2)
			{
				Task_Percentage_Dis(p_amount*100 / glob_para.shoot_amount);
			//	pix_amt_display(cur);
				
				//hours_time_dis(glob_para.tatol_time_h, cur);
				//minutes_time_dis(glob_para.tatol_time_m, cur);
				//secondes_time_dis(glob_para.tatol_time_s, cur);
			}
			else if(cur == 3)
			{
				hours_time_dis(glob_para.tatol_time_h, cur);
				minutes_time_dis(glob_para.tatol_time_m, cur);
				secondes_time_dis(glob_para.tatol_time_s, cur);
				//Time_DownCount(cur);
				//hours_time_dis(hours, cur);
				//minutes_time_dis(minutes, cur);
				//secondes_time_dis(seconds, cur);
			}
			else if(cur == 4)
			{
				//pix_amt_display(cur);
				pix_amt_display(cur);
				pix_amt_compara_dis(cur);
			}
			else if(cur == 5)
			{
				//cursor_id = cur;
				status_display(cur);
			}
			break;
		}
		case CONFIG_ID:
		{
			if(cur == 1)
			{
				language_display(cur);
			}
			else if(cur == 2)
			{
				factory_set_display(cur);
			}
			else if(cur == 3) // brightness
			{
				Brightness_dis(cur);
			}
			else if(cur == 4) // sleep time setting
			{
				sleep_time_dis(cur);
			}
			
			break;
		}
		case PREINSTALL_SET1:
		{
			specielty_para_dis(cur);
			break;
		}
		case PREINSTALL_MOVE:
		{
			specialty_move_dis(cur);
			break;
		}
		case PREINSTALL_SESHOT:
		{
			specialty_move_dis(cur);
			break;
		}
		case VIDEO_MODE:
		{
			if(cur == 1)
			{
				if(cursor_id == cur)inverse_get_value(0xff);
				move_to_disp(cur);
				inverse_get_value(0);
			}
			else if(cur == 2)
			{
				hours_time_dis(glob_para.duration_h, cur);
				minutes_time_dis(glob_para.duration_m, cur);
				secondes_time_dis(glob_para.duration_s, cur);
			}
			else if(cur == 3) //ramp
			{
				slow_start_stop_dis(cur);
			} 
			else if(cur == 4) // start / stop
			{
				status_display(cur);
			}
			else if(cur == 5) // ab point set
			{
			
			}
			break;
		}
		case MANUAL_MODE:
		{
			if(cur == 2)
			{
				Speed_display(cur);
			}
			else if(cur == 4) // 府轴角度显示
			{
				angle_display(cur, H_agnle);
			}
			else if(cur == 5)
			{
				//angle_display(cur, V_agnle);
			}
			break;
		}
		case SET_B_POINT:
		{
			if(cur == 4) // 府轴角度显示
			{
				angle_display(cur, H_agnle);
			}
			else if(cur == 5)
			{
			//	angle_display(cur, V_agnle);
			}
			
			break;
		}
	}
}

void single_cursor_dis(uint8_t cur)
{
	if(page_id == PREINSTALL_MODE)return;
	if(page_id == PREINSTALL_SET)return;
	if(page_id == PREINSTALL_SESHOT)return;
	inverse_get_value(0xff);
	//TFTDis_Selet(cur);
	page_display(cur);
	param_display(cur);
	inverse_get_value(0);
}



// datat为0xff时使OLED显示反相，即白底黑字
void inverse_get_value(uint8_t datat)
{
	Inverse_Dis = datat;
}
void cursor_shift(uint8_t dir)
{
	if(adj_parm_sel)return;
	
	switch(page_id)
	{
		case MAIN_ID:
		{
			cursor_count(dir, 6, 1);
			cursor_id_back = cursor_id;
			
			break;
		}
		case DELAY_SET:
		{
			cursor_count(dir, 6, 1);
//			cursor_display(6,0);
			break;
		}
		case DELAY_DIS:
		{
			cursor_count(dir, 6, 5);
			break;
		}
		case SINGLE_PANORAMA:
		{
			cursor_count(dir, 7, 1);
			break;
		}
		case SINGLE_PANORAMA_11:
		{
			cursor_count(dir, 4, 1);
			break;
		}
		case CONFIG_ID:
		{
			cursor_count(dir, 4, 1);
//			cursor_display(1,0);
			break;
		}
		case PREINSTALL_MODE:
		{
			specialty_cursor_id_shift(dir);
			break;
		}	
		case PREINSTALL_SET:
		{
			specialty_cursor_id_shift(dir);
			break;
		}
		case PREINSTALL_SET1:
		{
			cursor_count(dir, 8, 1);
			break;
		}		
		case PREINSTALL_MOVE:
		{
			cursor_count(dir, 4, 1);
			break;
		}
		/*case PREINSTALL_SESHOT:
		{
			cursor_count(dir, 4, 3);
			break;
		}*/
		case VIDEO_MODE:
		{
			cursor_count(dir, 5, 1);
			break;
		}	
		case REVERSE_MODE:
		{
			cursor_count(dir, 4, 3);
			break;
		}
		case ORIGIN_ACK:
		{
			cursor_count(dir, 4, 3);
			break;
		}
	}
}

void speed_display(uint8_t cur, uint8_t speed )
{
	unsigned char buff[10];
	uint8_t x_size = 0;
	uint8_t x = 0;

	if(speed< 10)
	{
		sprintf( (char *)buff,"  %d%%", speed);
	}
	else if(speed < 100)
	{
		sprintf( (char *)buff," %d%%", speed);
	}
	else
	{
		sprintf( (char *)buff,"%d%%", speed);
	}

	x_size = Check_String(buff, ENGLISH);
	x = LCD_W - (x_size + SCREEN_MIGRATION);
	
	Oled_EnlPrint(x, cur,buff, "", ENGLISH);
}

//轨道方向显示
void video_dir_display(uint8_t cur)/////////////////////////////////////////////////////
{
	uint8_t x=0, x1 = 0;

	
	{
		x = 28;
		x1 = 88;
	}

	
	if(glob_para.orbital_dir)
	{
		Oled_EnlPrint(x, cur,"< ", "左", ENGLISH);
		
		Oled_EnlPrint(x1, cur,"  ", "右", ENGLISH);
	}
	else
	{
		Oled_EnlPrint(x1, cur," >", "右", ENGLISH);
		
		Oled_EnlPrint(x, cur,"  ", "左", ENGLISH);
	}
	

//	orbital_run();	

}


void status_display(uint8_t cur)
{
	uint8_t x = 96;
	uint8_t Lang = 0;
	uint8_t tag = 0;
	
	if(page_id == DELAY_DIS)
	{
		if(cursor_id == 5)inverse_get_value(0xff);
		
		if(m_start)
		{
			if(wifi_id.language_sel == ENGLISH || wifi_id.language_sel == FRENCH || wifi_id.language_sel == GERMAN)
			{
				x = Check_String("   Pause    ", wifi_id.language_sel);
				Lang = ENGLISH;
			}
			else if(wifi_id.language_sel == ITALIANO || wifi_id.language_sel == SPANISH)
			{
				x = Check_String("   Pausa   ", wifi_id.language_sel);
				Lang = ENGLISH;
			}
			else if(wifi_id.language_sel == NELANDS)
			{
				x = Check_String("  Pauze  ", wifi_id.language_sel);
				Lang = ENGLISH;
			}
			else if(wifi_id.language_sel == CHINESE)
			{
				x = Check_String("  暂停  ", wifi_id.language_sel);
				Lang = CHINESE;
			}
				
		//	Oled_EnlPrint111(x, cur, z_buffer);
		}
		else
		{
			if(wifi_id.language_sel == ENGLISH)
			{
				x = Check_String("Continue", wifi_id.language_sel);
				Lang = ENGLISH;
			}
			else if(wifi_id.language_sel == CHINESE)
			{
				x = Check_String("  继续  ", wifi_id.language_sel);
				Lang = CHINESE;
			}
			else if(wifi_id.language_sel == NELANDS)
			{
				x = Check_String("Doorgaan", wifi_id.language_sel);
				Lang = ENGLISH;
			}
			else if(wifi_id.language_sel == FRENCH)
			{
				x = Check_String("Continuer", wifi_id.language_sel);
				Lang = ENGLISH;
			}
			else if(wifi_id.language_sel == SPANISH)
			{
				x = Check_String("Continuar", wifi_id.language_sel);
				Lang = ENGLISH;
			}
			else if(wifi_id.language_sel == ITALIANO)
			{
				x = Check_String("Continuare", wifi_id.language_sel);
				Lang = ENGLISH;
			}
			else if(wifi_id.language_sel == GERMAN)
			{
				x = Check_String("Weitermachen", wifi_id.language_sel);
				Lang = ENGLISH;
			}
		}
		
		x = (LCD_W - x) / 2;
		Oled_EnlPrint(x, cur, z_buffer,z_buffer,Lang);
		inverse_get_value(0);
	}
	else
	{
		if(cursor_id == 0)return;
		
		if(page_id == DELAY_SET)
		{
			if(cursor_id == 5)inverse_get_value(0xff);
		}
		else if(page_id == PREINSTALL_MOVE)
		{
			if(cursor_id == 3)inverse_get_value(0xff);
		}
		else if(page_id == VIDEO_MODE)
		{
			if(cursor_id == 4)inverse_get_value(0xff);
		}
		else
		{
			if(cursor_id == 3)inverse_get_value(0xff);
		}
		
		if(m_start)
		{
			if(wifi_id.language_sel == ENGLISH || wifi_id.language_sel == FRENCH || wifi_id.language_sel == ITALIANO|| wifi_id.language_sel == NELANDS || wifi_id.language_sel == SPANISH)
			{
				x = Check_String("  Stop  ", wifi_id.language_sel);
				Lang = ENGLISH;
			}
			else if(wifi_id.language_sel == CHINESE)
			{
				if(page_id == VIDEO_MODE)
				{
					x = Check_String("  停止  ", wifi_id.language_sel);
				}
				else
				{
					x = Check_String("  暂停  ", wifi_id.language_sel);
				}
				Lang = CHINESE;
			}
			else if(wifi_id.language_sel == GERMAN)
			{
				x = Check_String("Stoppen ", wifi_id.language_sel);
				Lang = ENGLISH;
			}
				
		//	Oled_EnlPrint111(x, cur, z_buffer);
		}
		else
		{
			if(wifi_id.language_sel == ENGLISH || wifi_id.language_sel == NELANDS || wifi_id.language_sel == GERMAN)
			{
				x = Check_String(" Start  ", wifi_id.language_sel);
				Lang = ENGLISH;
			}
			else if(wifi_id.language_sel == CHINESE)
			{
				x = Check_String("  启动  ", wifi_id.language_sel);
				Lang = CHINESE;
			}
			else if(wifi_id.language_sel == FRENCH)
			{
				x = Check_String("D marrer", wifi_id.language_sel);
				tag = 1;
				Lang = ENGLISH;
			}
			else if(wifi_id.language_sel == SPANISH)
			{
				x = Check_String(" Inicio ", wifi_id.language_sel);
				Lang = ENGLISH;
			}
			else if(wifi_id.language_sel == ITALIANO)
			{
				x = Check_String(" Avvio  ", wifi_id.language_sel);
				Lang = ENGLISH;
			}
			
		}
		
		x = (LCD_W - x) / 2;
		Oled_EnlPrint(x, cur, z_buffer,z_buffer,Lang);
		
		if(tag == 1)
		{
			Oled_EnlPrint(46, cur,"é","", wifi_id.language_sel); // Set Point A
		}
		
		/*if(page_id == DELAY_SET)
		{
			if(cursor_id == 6)inverse_get_value(0);
		}
		else if(page_id == PREINSTALL_MOVE)
		{
			if(cursor_id == 3)inverse_get_value(0);
		}
		else if(page_id == VIDEO_MODE)
		{
			if(cursor_id == 4)inverse_get_value(0);
		}
		else
		{
			if(cursor_id == 5)inverse_get_value(0);
		}*/
		inverse_get_value(0);
	}
}



//张数显示
void pix_amt_display(uint8_t cur)
{
	uint8_t buff[10];
	uint8_t x=0;
	uint8_t x_size = 0;
	uint32_t amounts = 0;
	
	if(page_id == SINGLE_PANORAMA)
	{
		amounts = glob_para.sd_amount;
	}
	else 
	{
		amounts = glob_para.shoot_amount;
	}

	if(page_id == DELAY_SET)
	{
		if(amounts < 10)
		{
			sprintf( (char *)buff,"   %d", amounts);
		}
		else if(amounts < 100)
		{
			sprintf( (char *)buff,"  %d", amounts);
		}
		else if(amounts < 1000)
		{
			sprintf( (char *)buff," %d", amounts);
		}
		/*else if(amounts < 10000)
		{
			sprintf( (char *)buff,"%d ", amounts);
		}*/
		else
		{
			sprintf( (char *)buff,"%d", amounts);
		}
	}
	else
	{
		if(amounts < 10)
		{
			sprintf( (char *)buff," %d", amounts);
		}
		else if(amounts < 100)
		{
			sprintf( (char *)buff,"%d", amounts);
		}
		/*else if(amounts < 1000)
		{
			sprintf( (char *)buff,"%d  ", amounts);
		}
		else if(amounts < 10000)
		{
			sprintf( (char *)buff,"%d ", amounts);
		}
		else
		{
			sprintf( (char *)buff,"%d", amounts);
		}*/
	}
	
	x_size = Check_String(buff, ENGLISH);
	if(page_id == DELAY_SET)x = LCD_W - (x_size + SCREEN_MIGRATION);
	else if(page_id == DELAY_DIS)x=LCD_W - (x_size + SCREEN_MIGRATION);
	else x=LCD_W - (x_size + SCREEN_MIGRATION);
	Oled_EnlPrint(x, cur, buff, " ", ENGLISH);
}

void fps_amt_display(uint8_t cur)
{
	uint8_t buff[10];
	uint8_t x_size=0;
	
	//if(page_id == DELAY_SET)x=72;
	//else if(page_id == DELAY_DIS)x=88;
	if(glob_para.fps>2)glob_para.fps=2;
	if(fps_buffer[glob_para.fps]< 10)
	{
		sprintf( (char *)buff,"  %d", fps_buffer[glob_para.fps]);
	}
	else if(fps_buffer[glob_para.fps] < 100)
	{
		sprintf( (char *)buff," %d", fps_buffer[glob_para.fps]);
	}
	else 
	{
		sprintf( (char *)buff,"%d", fps_buffer[glob_para.fps]);
	}
	
	x_size = Check_String(buff, ENGLISH);
	x_size = LCD_W - (x_size + SCREEN_MIGRATION);
	Oled_EnlPrint(x_size, cur, buff, " ", ENGLISH);
}

void play_back_dis(uint8_t cur)
{
	uint8_t buff[10];
	uint8_t x=0;
		
	if(glob_para.playback< 10)
	{
		sprintf( (char *)buff,"  %dS", glob_para.playback);
	}
	else if(glob_para.playback < 100)
	{
		sprintf( (char *)buff," %dS", glob_para.playback);
	}
	else
	{
		sprintf( (char *)buff,"%dS", glob_para.playback);
	}
	
	x = Check_String(buff, ENGLISH);
	x = LCD_W - (x+ SCREEN_MIGRATION);
	Oled_EnlPrint(x, cur, buff, " ", ENGLISH);
}


void interval_time_dis(uint8_t cur)
{
	uint8_t buff[10];
	uint8_t x=0;
	uint8_t x_size=0;

	
	if(glob_para.ssinterval <10)
	{
		sprintf( (char *)buff," %.1fS", glob_para.ssinterval);
	}
	/*else if(glob_para.shut_time_s <100)
	{
		sprintf( (char *)buff,"00%.1fS", glob_para.shut_time_s);
	}
	else if(glob_para.shut_time_s <1000)
	{
		sprintf( (char *)buff,"%.1fS", glob_para.shut_time_s);
	}*/
	else 
	{
		sprintf( (char *)buff,"%.1fS", glob_para.ssinterval);
	}
	
	x_size = Check_String(buff, ENGLISH);
	x = LCD_W - (x_size + SCREEN_MIGRATION);
	Oled_EnlPrint(x, cur, buff, " ", ENGLISH);
}

//符点显示
void decimal_point_time_dis(float datat, uint8_t cur)
{
	uint8_t buff[10];
	uint8_t x=0;
	uint8_t x_size=0;

	
	if(datat <10)
	{
		sprintf( (char *)buff," %.1fS", datat);
	}
	else 
	{
		sprintf( (char *)buff,"%.1fS", datat);
	}
	
	x_size = Check_String(buff, ENGLISH);
	x = LCD_W - (x_size + SCREEN_MIGRATION);
	Oled_EnlPrint(x, cur, buff, " ", ENGLISH);
}

//整数显示
void integer_display(uint16_t datat, uint8_t cur)
{
	uint8_t buff[10];
	uint8_t x=0;
	uint8_t x_size = 0;
	
	if(datat < 10)
	{
		sprintf( (char *)buff," %d", datat);
	}
	else if(datat < 100)
	{
		sprintf( (char *)buff,"%d", datat);
	}
	
	x_size = Check_String(buff, ENGLISH);
	//if(page_id == DELAY_SET)x = LCD_W - (x_size + SCREEN_MIGRATION);
	//else if(page_id == DELAY_DIS)x=LCD_W - (x_size + SCREEN_MIGRATION);
	x=LCD_W - (x_size + SCREEN_MIGRATION);
	Oled_EnlPrint(x, cur, buff, " ", ENGLISH);
}

//整数显示,指定位置显示 
void integer_position_display(uint16_t datat, uint8_t cur, uint8_t curxx)
{
	uint8_t buff[10];
	//uint8_t x=0;
	//uint8_t x_size = 0;
	
	if(datat < 10)
	{
		sprintf( (char *)buff,"(%d) ", datat);
	}
	else if(datat < 100)
	{
		sprintf( (char *)buff,"(%d)", datat);
	}
	
	//x_size = Check_String(buff, ENGLISH);
	//if(page_id == DELAY_SET)x = LCD_W - (x_size + SCREEN_MIGRATION);
	//else if(page_id == DELAY_DIS)x=LCD_W - (x_size + SCREEN_MIGRATION);
	//x=LCD_W - (x_size + SCREEN_MIGRATION);
	Oled_EnlPrint(curxx, cur, buff, " ", ENGLISH);
}

void start_stop_dis(uint8_t cur)
{
	if(page_id == DELAY_DIS)
	{
		if(m_start)
		{
			Oled_EnlPrint(42, cur<<1,"  Go ", "继续", wifi_id.language_sel);
		}
		else
		{
			Oled_EnlPrint(42, cur<<1,"Pause", "暂停", wifi_id.language_sel);
		}
	}
}

void Fast_time_dis(uint8_t cur)
{
	uint8_t buff[10];
	uint8_t x=0;
	uint8_t x_size=0;

	
	if(glob_para.shut_time_s<10)
	{
		sprintf( (char *)buff," %.1fS", glob_para.shut_time_s);
	}
	else 
	{
		sprintf( (char *)buff,"%.1fS", glob_para.shut_time_s);
	}
	
	x_size = Check_String(buff, ENGLISH);
	x = LCD_W - (x_size + SCREEN_MIGRATION);
	Oled_EnlPrint(x, cur, buff, " ", ENGLISH);

}

//设置页显示自动参数，Y是或N否
void auto_return_display(uint8_t cur)
{
	uint8_t x_size = 0;
	
	x_size = Check_String("Y", ENGLISH);
	x_size = LCD_W - (x_size + SCREEN_MIGRATION);
	
	if(glob_para.auto_return_tag)
	{
		Oled_EnlPrint(x_size, cur,"Y", "是", ENGLISH);
	}
	else
	{
		Oled_EnlPrint(x_size, cur,"N", "否", ENGLISH);
	}
}

void duration_time_dis(uint8_t types, uint8_t cur)
{
	uint8_t buff[5];

	if(types == MINUTES_DIS)
	{
		if(glob_para.duration_m < 10)
		{
			sprintf( (char *)buff,"0%dM", glob_para.duration_m);
		}
		else if(glob_para.duration_m < 100)
		{
			sprintf( (char *)buff,"%dM", glob_para.duration_m);
		}
		
		Oled_EnlPrint(72, cur<<1, buff, " ", ENGLISH);
	}
	else if(types == SECONDS_DIS)
	{
		if(glob_para.duration_s < 10)
		{
			sprintf( (char *)buff,"0%dS", glob_para.duration_s);
		}
		else if(glob_para.duration_s < 100)
		{
			sprintf( (char *)buff,"%dS", glob_para.duration_s);
		}
		
		Oled_EnlPrint(96, cur<<1, buff, " ", ENGLISH);
	}
}

void tatol_hours_time_dis(uint32_t param, uint8_t cur)
{
	
	uint8_t buff[5];
	uint8_t x=0;
	uint8_t x_size = 0;
	
	
	if(param < 10)
	{
		sprintf( (char *)buff,"   0%dH", param);
	}
	else if(param < 100)
	{
		sprintf( (char *)buff,"   %dH", param);
	}
	else if(param < 1000)
	{
		sprintf( (char *)buff,"  %dH", param);
	}
	else if(param < 10000)
	{
		sprintf( (char *)buff," %dH", param);
	}
	else
	{
		sprintf( (char *)buff,"%dH", param);
	}

	
	x_size = Check_String(buff, ENGLISH);
	
	x=LCD_W - (x_size + SCREEN_MIGRATION + 38);
	
	Oled_EnlPrint(x, cur, buff, " ", ENGLISH);
	
	
	
}

void hours_time_dis(uint32_t param, uint8_t cur)
{
	
	uint8_t buff[5];
	uint8_t x=0;
	uint8_t x_size = 0;
	
		if(((page_id == DELAY_SET) && (cur == 7)) || ((page_id == DELAY_DIS) && (cur == 3)))
		{
			if(param < 10)
			{
				sprintf( (char *)buff,"0%d", param);
			}
			else if(param < 100)
			{
				sprintf( (char *)buff,"%d", param);
			}
			x_size = Check_String(buff, ENGLISH);
			
			x = LCD_W - (x_size + SCREEN_MIGRATION + 38+6);
			
			LCD_shi_fen_miao_display(x+12, cur, "H");
		}
		else
		{
			if(param < 10)
			{
				sprintf( (char *)buff,"0%dH", param);
			}
			else if(param < 100)
			{
				sprintf( (char *)buff,"%dH", param);
			}
			x_size = Check_String(buff, ENGLISH);
			
			if(page_id == VIDEO_MODE)x = LCD_W - (x_size + SCREEN_MIGRATION + 38+12);
			else x = LCD_W - (x_size + SCREEN_MIGRATION + 38);
		}
	
	
	Oled_EnlPrint(x, cur, buff, " ", ENGLISH);
	
	
	
}

void minutes_time_dis(uint16_t param, uint8_t cur)
{
	uint8_t buff[5];
	uint8_t x=0;
	uint8_t x_size = 0;
//	uint8_t cur_back=0;
	
	if(((page_id == DELAY_SET) && (cur == 7)) || ((page_id == DELAY_DIS) && (cur == 3)))
	{
		if(param < 10)
		{
			sprintf( (char *)buff,"0%d", param);
		}
		else if(param < 100)
		{
			sprintf( (char *)buff,"%d", param);
		}
		x_size = Check_String(buff, ENGLISH);
		
		x = LCD_W - (x_size +19 + SCREEN_MIGRATION+6);
		
		LCD_shi_fen_miao_display(x+12, cur, "M");
	}
	else
	{
		if(param < 10)
		{
			sprintf( (char *)buff,"0%dM", param);
		}
		else if(param < 100)
		{
			sprintf( (char *)buff,"%dM", param);
		}
		x_size = Check_String(buff, ENGLISH);
		if(page_id == VIDEO_MODE)x = LCD_W - (x_size +19 + SCREEN_MIGRATION+6);
		else x = LCD_W - (x_size +19 + SCREEN_MIGRATION);
	}
	
	Oled_EnlPrint(x, cur, buff, " ", ENGLISH);
}


void secondes_time_dis(uint16_t param, uint8_t cur)
{
	uint8_t buff[5];
	uint8_t x=0;
	uint8_t x_size = 0;
	
	//if(page_id == AUTO_MODE)x=192;
	//else if(page_id == DELAY_DIS)x=88;
	
	if(page_id == SINGLE_PANORAMA || page_id == PREINSTALL_SET1 || page_id == PREINSTALL_MOVE)
	{
		if(param < 10)
		{
			sprintf( (char *)buff," 0%dS", param);
		}
		else if(param < 100)
		{
			sprintf( (char *)buff," %dS", param);
		}
		else if(param < 1000 && param >= 100)
		{
			sprintf( (char *)buff,"%dS", param);
		}
		x_size = Check_String(buff, ENGLISH);
		x = LCD_W - (x_size + SCREEN_MIGRATION);
		//Oled_EnlPrint(x, cur, buff, " ", ENGLISH);
	}
	else if(page_id == DELAY_SET || page_id == DELAY_DIS)
	{
		if(((page_id == DELAY_SET) && (cur == 7)) || ((page_id == DELAY_DIS) && (cur == 3)))
		{
			if(param < 10)
			{
				sprintf( (char *)buff,"0%d", param);
			}
			else if(param < 100)
			{
				sprintf( (char *)buff,"%d", param);
			}
			x_size = Check_String(buff, ENGLISH);
			
			x = LCD_W - (x_size + SCREEN_MIGRATION+6);
			
			LCD_shi_fen_miao_display(x+12, cur, "S");
		}
		else
		{
			if(param < 10)
			{
				sprintf( (char *)buff," 0%dS", param);
			}
			else if(param < 100)
			{
				sprintf( (char *)buff," %dS", param);
			}
			else if(param < 1000 && param >= 100)
			{
				sprintf( (char *)buff,"%dS", param);
			}
			x_size = Check_String(buff, ENGLISH);
			x = LCD_W - (x_size + SCREEN_MIGRATION);
		//	Oled_EnlPrint(x, cur, buff, " ", ENGLISH);
		}
	}
	else
	{
		if(param < 10)
		{
			sprintf( (char *)buff,"0%dS", param);
		}
		else if(param < 100)
		{
			sprintf( (char *)buff,"%dS", param);
		}
		x_size = Check_String(buff, ENGLISH);
		x = LCD_W - (x_size + SCREEN_MIGRATION);
		//Oled_EnlPrint(x, cur, buff, " ", ENGLISH);
	}
	
	Oled_EnlPrint(x, cur, buff, " ", ENGLISH);
}

// delay mode :duraction  and delay time display
void durationORdelay_time_dis(uint16_t param, uint8_t cur)
{
	if((page_id == DELAY_SET) || (page_id == DELAY_DIS) || (page_id == VIDEO_MODE))
	{
		if(adj_parm_sel == 1)hours_time_dis(param, cur);
		else if(adj_parm_sel == 2)minutes_time_dis(param, cur);
		else if(adj_parm_sel == 3)secondes_time_dis(param, cur);
	}
}

void move_to_disp(uint8_t cur)
{
	uint8_t x_size = 0;
	
	x_size = Check_String("B->A", ENGLISH);
	x_size = LCD_W - (x_size + SCREEN_MIGRATION);
	
	/*if(page_id == MANUAL_MODE || page_id == AUTO_MODE)
	{
		if(glob_para.orbital_dir)Oled_EnlPrint(x_size, cur<<1, ">>A", " ", ENGLISH);
		else Oled_EnlPrint(x_size, cur<<1, ">>B", " ", ENGLISH);
	}
	if((page_id == DELAY_SET) || (page_id == ADVANCE_TIME))
	{
		if(cur >= 4)cur -=4;
	}*/
		if(glob_para.orbital_dir)Oled_EnlPrint(x_size, cur, "B->A", " ", ENGLISH);
		else Oled_EnlPrint(x_size, cur, "A->B", " ", ENGLISH);
}


void pix_amt_compara_dis(uint8_t cur)
{
	uint8_t buff[10];
	uint8_t x = 0;

	//if(page_id == DELAY_DIS)
	{
		if(p_amount < 10)
		{
			sprintf( (char *)buff,"    %d /", p_amount);
		}
		else if(p_amount < 100)
		{
			sprintf( (char *)buff,"   %d /", p_amount);
		}
		else if(p_amount < 1000)
		{
			sprintf( (char *)buff,"  %d /", p_amount);
		}
		else if(p_amount < 10000)
		{
			sprintf( (char *)buff," %d /", p_amount);
		}
		else
		{
			sprintf( (char *)buff,"%d /", p_amount);
		}
	}

	
	x = Check_String(buff, ENGLISH);
	if(page_id == PREINSTALL_MOVE)
	{
		x = LCD_W-(x+SCREEN_MIGRATION+18);
	}
	else
	{
		x =LCD_W-(x+SCREEN_MIGRATION+36);
	}
	Oled_EnlPrint(x, cur, buff, "张数", ENGLISH);
}

void versions_display(void)
{
	uint8_t buff[20];
	sprintf( (char *)buff,"G10 V%d", FW_VERSION);
	Oled_EnlPrint(0, 6, buff, "", ENGLISH);
}

void angle_display(uint8_t cur, uint16_t angles)
{
	
	uint8_t buff[5];
	uint8_t x=0;
	uint8_t x_size = 0;
	
	
	if(angles < 10)
	{
		sprintf( (char *)buff,"   %d", angles);
	}
	else if(angles < 100)
	{
		sprintf( (char *)buff,"   %d", angles);
	}
	else if(angles < 1000)
	{
		sprintf( (char *)buff,"  %d", angles);
	}
	else if(angles < 10000)
	{
		sprintf( (char *)buff," %d", angles);
	}
	else
	{
		sprintf( (char *)buff,"%d", angles);
	}

	
	x_size = Check_String(buff, ENGLISH);
	
	x=LCD_W - (x_size + SCREEN_MIGRATION);
	
	Oled_EnlPrint(x, cur, buff, " ", ENGLISH);
	
	
	
}


void param_adjust(uint8_t dir)
{	
	switch(page_id)
	{
		case SINGLE_PANORAMA:
		{
			if(cursor_id == 1)
			{
				inverse_get_value(0xff);
				glob_para.sd_amount = data_count(glob_para.sd_amount, dir, 99, 1);
				param_display(cursor_id);
				controller_send_data_to_motor(0,0x04, 0x01);
				inverse_get_value(0);
			}
			else if(cursor_id == 2)
			{
				inverse_get_value(0xff);
				glob_para.ssinterval = para_count(dir, glob_para.ssinterval, 30, 0.5);
				if(glob_para.ssinterval > 30)glob_para.ssinterval = 30;
				else if(glob_para.ssinterval < 0.5)glob_para.ssinterval = 0.5;
				interval_time_dis(cursor_id);
				controller_send_data_to_motor(0,0x04, 0x02);
				inverse_get_value(0);
			}
			else if(cursor_id == 4) // delay begin time
			{
				inverse_get_value(0xff);
				glob_para.delay_time_s = data_count(glob_para.delay_time_s, dir,  255, 0);
				if(glob_para.delay_time_s > 255)glob_para.delay_time_s = 255;
				secondes_time_dis(glob_para.delay_time_s, cursor_id);
				controller_send_data_to_motor(0,0x04, 0x04);
				inverse_get_value(0);
			}
			else if(cursor_id == 3) // exposures time
			{
				inverse_get_value(0xff);
				glob_para.shut_time_s = para_count(dir, glob_para.shut_time_s, 99, 0.5);
				//secondes_time_dis(glob_para.shut_time_s, cursor_id);
				param_display(cursor_id);
				controller_send_data_to_motor(0,0x04, 0x03);
				inverse_get_value(0);
			}
			else if(cursor_id == 5)// auto or manual
			{
				if(dir & KEY_RIGHT_MASK)
				{
					std_manul_tag = 0xff;
				}
				else if(dir & KEY_LEFT_MASK)
				{
					std_manul_tag = 0;
				}
				inverse_get_value(0xff);
				param_display(cursor_id);
				inverse_get_value(0);
				controller_send_data_to_motor(0,0x04, 0x05);
			}
			else if(cursor_id == 6)
			{
				inverse_get_value(0xff);
				glob_para.shut_times = data_count(glob_para.shut_times, dir, 99, 1);
				param_display(cursor_id);
				controller_send_data_to_motor(0,0x04, 0x06);
				inverse_get_value(0);
			}
			break;
		}		
		case SINGLE_PANORAMA_11:
		{
			if(cursor_id == 1)
			{
				inverse_get_value(0xff);
				glob_para.sys_stop_t = para_count(dir, glob_para.sys_stop_t, 99, 0.5);
				//if(glob_para.ssinterval > 30)glob_para.ssinterval = 30;
				//else if(glob_para.ssinterval < 0.5)glob_para.ssinterval = 0.5;
				param_display(cursor_id);
				controller_send_data_to_motor(0,0x04, 0x07);
				inverse_get_value(0);
			}
			else if(cursor_id == 2)
			{
				inverse_get_value(0xff);
				glob_para.orbital_speed = data_count(glob_para.orbital_speed, dir, 4, 1);
			//	if(glob_para.wid_time_s > 255)glob_para.wid_time_s = 255;
				param_display(cursor_id);
				controller_send_data_to_motor(0,0x05, 0x08);
				inverse_get_value(0);
			}
			
			break;
		}
		case DELAY_SET:
		{			
			if(cursor_id == 1)
			{
				inverse_get_value(0xff);
				glob_para.interval = data_count(glob_para.interval, dir, 0xffff, 2);
				param_display(cursor_id);
				
				inverse_get_value(0);
				controller_send_data_to_motor(0,0x02, 0x01);
			}
			else if(cursor_id == 2)
			{
				inverse_get_value(0xff);
				
				glob_para.exposure = data_count(glob_para.exposure, dir, 0xffff-3, 1);
				param_display(cursor_id);
				
				inverse_get_value(0);
				controller_send_data_to_motor(0,0x02, 0x02);
				if_write_flash();
			}
			else if(cursor_id == 4)
			{
				if(dir & KEY_RIGHT_MASK)glob_para.orbital_dir = 0;
				else if(dir & KEY_LEFT_MASK)glob_para.orbital_dir = 0xff;
				controller_send_data_to_motor(0,0x02, 0x05);
				param_display(cursor_id);
			}
			
			//inverse_get_value(0);
			break;
		}		
		case CONFIG_ID:
		{
			if(cursor_id == 1)
			{
				if(dir & KEY_LEFT_MASK)
				{
					if(wifi_id.language_sel > CHINESE)wifi_id.language_sel--;
					else return;
				}
				else if(dir & KEY_RIGHT_MASK)
				{
					if(wifi_id.language_sel < ENGLISH)wifi_id.language_sel++;
					else
					{
						wifi_id.language_sel = ENGLISH;
						return;
					}
				}
				
				change_page();
				version_dis();
				if_write_flash();
			}
			else if(cursor_id == 2)
			{
				inverse_get_value(0xff);
				
				if(dir & KEY_LEFT_MASK)Factory_set= 1;
				if(dir & KEY_RIGHT_MASK)Factory_set= 0;
				param_display(cursor_id);
				
				inverse_get_value(0);
			}
			else if(cursor_id == 3) // BRIGHTNESS
			{
				inverse_get_value(0xff);
				
				//glob_para.shoot_amount = para_count(dir, glob_para.shoot_amount, 10000, 2);
				wifi_id.brightness= data_count(wifi_id.brightness,dir,  100, 14);
				if(wifi_id.brightness < 14)wifi_id.brightness = 14;
				if(wifi_id.brightness > 100)wifi_id.brightness = 100;
				LL_TIM_OC_SetCompareCH4(TIM3, wifi_id.brightness);
//				param_calibration();
				param_display(cursor_id);
				if_write_flash();
				inverse_get_value(0);
			}
			else if(cursor_id == 4) // sleep time
			{
				inverse_get_value(0xff);
				
				//glob_para.shoot_amount = para_count(dir, glob_para.shoot_amount, 10000, 2);
				wifi_id.sleep_time = data_count(wifi_id.sleep_time,dir,	10, 1);
				if(wifi_id.sleep_time < 1)wifi_id.sleep_time = 1;
				if(wifi_id.sleep_time > 10)wifi_id.sleep_time = 10;
				
//				param_calibration();
				param_display(cursor_id);
				if_write_flash();
				inverse_get_value(0);
			}
			
			break;
		}
		case PREINSTALL_SET1:
		{
			specielty_para_adjust(dir);
			break;
		}
		case PREINSTALL_MOVE:
		{
			specielty_para_adjust(dir);
			break;
		}
		case VIDEO_MODE:
		{
			if(cursor_id == 1)
			{
				if(dir & KEY_RIGHT_MASK)glob_para.orbital_dir = 0;
				else if(dir & KEY_LEFT_MASK)glob_para.orbital_dir = 0xff;
				controller_send_data_to_motor(0,0x01, 0x06);
				param_display(cursor_id);
			}
			else if(cursor_id == 3) // ramp
			{
				inverse_get_value(0xff);
				
				if(dir & KEY_LEFT_MASK)ramp_flag = 0xff;
				else if(dir & KEY_RIGHT_MASK)ramp_flag = 0;
				
				param_display(cursor_id);
			//	if_write_flash();
				inverse_get_value(0);
			controller_send_data_to_motor(0,0x01, 0x07);
			}
			break;
		}
		case MANUAL_MODE:
		{
			if(adj_parm_sel)
			{
				glob_para.orbital_speed = data_count(glob_para.orbital_speed, dir, 3, 1);
				controller_send_data_to_motor(0,0x07, 0x01);
				param_display(2);
			}
			else
			{
				
			}
			break;
		}
	}
}


void change_page(void)
{
	
	oled_print_screen();
	if(page_id > LOCUS_PAGE && page_id != FINDING_ORIGIN)
	{
		single_cursor_dis(cursor_id);
	}
	//auto_mode_speed_init();
	
}

void TimeDown_Dis(void)
{
	uint8_t buff[10]={0};
	
	if(hours < 10)
	{
		sprintf( (char *)buff,"0%dH", hours);
	}
	else if(hours < 100)
	{
		sprintf( (char *)buff,"%dH", hours);
	}
	/*else if(p_hour < 1000)
	{
		sprintf( (char *)buff," %d:", p_hour);
	}
	else
	{
		sprintf( (char *)buff,"%d:", p_hour);
	}*/
	
	Oled_EnlPrint(0, 1<<1, buff, "", ENGLISH);

	if((minutes <10)&&(seconds < 10))
	{
		sprintf( (char *)buff,"0%dM0%dS", minutes, seconds);
	}
	else if((minutes <100)&&(seconds < 10))
	{
		sprintf( (char *)buff,"%dM0%dS", minutes, seconds);
	}
	else if((minutes <10)&&(seconds < 100))
	{
		sprintf( (char *)buff,"0%dM%dS", minutes, seconds);
	}
	else if((minutes <100)&&(seconds < 100))
	{
		sprintf( (char *)buff,"%dM%dS", minutes, seconds);
	}

	Oled_EnlPrint(24, 1<<1, buff, "", ENGLISH);
}

void total_time_display(void)
{
//	uint32_t p_pix_time=0;
//	uint16_t p_amount_b=0;

	if(delay_time_set) // 当延时有设置时间时先倒计延后时间再显示拍摄时长
	{
		//p_pix_time = glob_para.delay_time_h * 3600 + glob_para.delay_time_m * 60;

		hours = glob_para.delay_time_h;
		minutes = glob_para.delay_time_m;
		seconds = 0;
	}
	else // 显示拍摄时长
	{
		/*if((glob_para.b_point - glob_para.a_point) == 0)return;
		
		if(glob_para.shoot_amount)
		{
			p_amount_b = glob_para.shoot_amount;
			p_pix_time = p_amount_b * glob_para.interval + glob_para.fast_time;
		}
		else
		{
			p_pix_time = 0;
		}*/
		
		hours = 0;
		minutes = 0;
		seconds = 0;
	}
	
	TimeDown_Dis();
	
}

void Task_Percentage_Dis(uint8_t datat)
{
	uint8_t buff[6]={0};
	uint8_t x_size = 0;
	
	if(datat < 10)
	{
		sprintf( (char *)buff,"  %d%%", datat);
	}
	else if(datat < 100)
	{
		sprintf( (char *)buff," %d%%", datat);
	}
	else
	{
		sprintf( (char *)buff,"%d%%", datat);
	}
	
	x_size = Check_String(buff, ENGLISH);
	Oled_EnlPrint(LCD_W-(SCREEN_MIGRATION+x_size), 2, buff, "", ENGLISH);
}



void interval_time_error_adjust(void)
{
//	if(interval_time_error)
//	{
//		interval_time_error = 0;
//		glob_para.interval = ((float)(glob_para.tatol_time_h * 3600 + glob_para.tatol_time_m * 60) - glob_para.fast_time) / (float)(glob_para.shoot_amount-1);
//		param_display(2);
//	}
}

/////////////////////////////////// yun tai /////////////////////////////////

// the moving mode of device to display
void move_mode_dis(uint8_t cur)
{
	uint8_t *buff00;
	uint8_t *buff11;
	uint8_t x=0;
	if(page_id == DELAY_SET)
	{
		x= 72;
		if(cur >= 4)cur -= 4;
	}
	
	if(move_mode)
	{
		buff00 = "Focus";
		buff11 = "跟焦";
	}
	else
	{
		buff00 = "Scan";
		buff11 = "扫景";
	}
	
	if(wifi_id.language_sel== ENGLISH)
	{
		Oled_EnlPrint(x, cur<<1,buff00, buff11, wifi_id.language_sel);
	}
	else if(wifi_id.language_sel == CHINESE)
	{
		Oled_EnlPrint(x, cur<<1,buff00, buff11, wifi_id.language_sel);
	}
	else if(wifi_id.language_sel == KOREANH)
	{
		Oled_EnlPrint(x, cur<<1,"-", "中文", wifi_id.language_sel);
	}
}

// scan view or forces
void shoot_mode_adjust(void)
{
/*	if(move_mode)
	{
		glob_para.yun_dir = ~glob_para.orbital_dir;
	}
	else
	{
		glob_para.yun_dir = glob_para.orbital_dir;
	}

	yuntai_direction_change();*/
}

void locus_disp(void)
{
	if(locus_numb == 0)
	{
		Oled_EnlPrint(40, 0<<1,"Locus A", "轨迹A ", wifi_id.language_sel);
	}
	else if(locus_numb == 1)
	{
		Oled_EnlPrint(40, 0<<1,"Locus B", "轨迹B ", wifi_id.language_sel);
	}
	else if(locus_numb == 2)
	{
		Oled_EnlPrint(40, 0<<1,"Locus C", "轨迹C ", wifi_id.language_sel);
	}
}


void display_test(uint8_t y, uint8_t param11)
{
	uint8_t buff[5];
	
	
	if(param11 < 10)
	{
		sprintf( (char *)buff,"%d", param11);
	}
	else if(param11 < 100)
	{
		sprintf( (char *)buff,"0%d", param11);
	}
	else
	{
		sprintf( (char *)buff,"%d", param11);
	}

	Oled_EnlPrint(0, y<<1, buff, " ", ENGLISH);
	
}

void stop_shoot_dis(uint8_t cur)
{
	uint8_t x_size = 0;
	
	x_size = Check_String(" ON", ENGLISH);
	x_size = LCD_W - (x_size + SCREEN_MIGRATION);
	
	if(glob_para.stop_shoot)
	{
		Oled_EnlPrint(x_size, cur, " ON", " ", ENGLISH);
	}
	else
	{
		Oled_EnlPrint(x_size, cur, "OFF", " ", ENGLISH);
	}
}

void get_init_from_A650(void)
{	
	uint8_t i;
	i = Check_Buffer_Empty();
	if(i == NOT_EMPTY)return;
	
	
	if(get_init_tag != 0xff && get_init_tag != 0x01)
	{
		if(get_init_time)return;
		get_init_time = 1000;
		App_Buffer[i].app_send_buffer[0] = 0x90;
		App_Buffer[i].app_send_buffer[1] = 0x04;
		App_Buffer[i].app_send_buffer[2] = 0x09;
		App_Buffer[i].app_send_buffer[3] = 0x01;
		App_Buffer[i].app_send_buffer[4] = 0;
		App_Buffer[i].app_send_buffer[5] = 0;
		App_Buffer[i].app_send_buffer[6] = 0;
		App_Buffer[i].app_send_buffer[7] = 0;
		App_Buffer[i].app_send_buffer[8] = check_sum_add(8, App_Buffer[i].app_send_buffer);
		App_Buffer[i].app_send_size = 9;
	//	controller_send_data_to_motor(0x04,0x01, 0);
	}
}

void get_data_form_A650(void)
{	
	if(page_id == VIDEO_MODE)
	{
		controller_send_data_to_motor(0x06,0, 0x01);
	}
	else if((page_id == DELAY_SET) || (page_id == DELAY_DIS))
	{
		controller_send_data_to_motor(0x06,0, 0x02);
	}
	else if(page_id == MAIN_ID)
	{
		controller_send_data_to_motor(0x06,0, 0x04);
	}
	else if(page_id == SINGLE_PANORAMA)
	{
		controller_send_data_to_motor(0x06,0, 0x05);
	}	
	else if((page_id == SET_A_POINT))
	{
		controller_send_data_to_motor(0x06,0, 0x03);
	}
	else if(page_id == PREINSTALL_MODE)
	{
		controller_send_data_to_motor(0x06,0, 0x07);
	}
	else if(page_id == MANUAL_MODE)
	{
		controller_send_data_to_motor(0x06,0, 0x0a);
	}
	else if(page_id == ORIGIN_SET_MOVE)
	{
		controller_send_data_to_motor(0x06,0, 0x03);
	}
}

void all_page_param_dis(void)
{
	uint8_t i;
	for(i=0;i<5;i++)
	{
		param_display(i);
	}
	single_cursor_dis(cursor_id);
}

void tatol_time_calculat(void)
{
	uint32_t times = 0;

	times = (glob_para.shoot_amount-1)*(glob_para.fram_time_s + glob_para.fram_time_m*60 + glob_para.fram_time_h*3600) + (glob_para.shut_time_s + glob_para.shut_time_m*60 + glob_para.shut_time_h*3600);
	glob_para.tatol_time_h = times/60/60;
	glob_para.tatol_time_m = times / 60 - glob_para.tatol_time_h *60;
	glob_para.tatol_time_s = times - glob_para.tatol_time_m*60 - glob_para.tatol_time_h * 3600;
}

void receiver_data_from_A650(void)
{
	uint8_t temp = 0, i;
//	uint16_t aoumt = 0;
//	float battery = 0;
	if(app_read_buffer[0] == 0x09)
	{
		connection_set();
		if(get_init_tag)get_init_tag=0xff;
		//if(charge_tag)return;
		//if(CHARGE_STATUS == CHARGING)if(return_powers())return;
		switch(app_read_buffer[1])
		{
			case 0:
			{
				get_init_tag = 1;
				if(app_read_buffer[3] == 0x01) // video mode para
				{
					Set_ab_set_if(app_read_buffer[9]);
					
					glob_para.duration_s = app_read_buffer[4];
					glob_para.duration_m = app_read_buffer[5];
					glob_para.duration_h = app_read_buffer[6];
					ramp_flag = app_read_buffer[7];
					glob_para.orbital_dir = app_read_buffer[8];
					m_start = app_read_buffer[11];
					
					if(check_abpoint_Set_if(app_read_buffer[3]))
					{
						if(page_id != VIDEO_MODE)
						{
							page_id = VIDEO_MODE;
							cursor_id = 4;
							change_page();
						}
						else
						{
							
							param_display(2);
							param_display(3);
							param_display(4);
							param_display(1);
							single_cursor_dis(cursor_id);
						}
					}
					else
					{
						//set_mode_back(page_id);
						page_id = SET_A_POINT;
						cursor_id = 3;
						Set_ab_set_if(0);
						change_page();
					}

					cursor_id_back = 2;
				}
				else if(app_read_buffer[3] == 0x02) // timelapse mode para
				{
						Set_DDab_set_if(app_read_buffer[11]);
						glob_para.interval = (uint16_t)app_read_buffer[4] | (uint16_t)app_read_buffer[5]<<8;
						glob_para.exposure = (uint16_t)app_read_buffer[6] | (uint16_t)app_read_buffer[7]<<8;
						glob_para.shoot_amount = (uint16_t)app_read_buffer[8] | (uint16_t)app_read_buffer[9]<<8;
						m_start = app_read_buffer[10];
						glob_para.orbital_dir = app_read_buffer[12];
						
						D_amount[0]=glob_para.shoot_amount % 10; //个
						D_amount[1]=glob_para.shoot_amount/10 % 10; //十
						D_amount[2]=glob_para.shoot_amount/100 % 10; // 百
						D_amount[3]=glob_para.shoot_amount/1000 % 10; // 千
						
						if(check_abpoint_Set_if(app_read_buffer[3]))
						{
							if(m_start)
							{
								if(page_id != DELAY_DIS)
								{
									page_id = DELAY_DIS;
									cursor_id = 5;
									change_page();
								}
								else
								{
									single_cursor_dis(5);
								}
							}
							else
							{
								if(page_id != DELAY_SET && page_id != DELAY_DIS)
								{
									page_id = DELAY_SET;
									cursor_id = 1;
									change_page();
									//controller_send_data_to_motor(6,0x01, 0x05);
								}
								else
								{
									//param_display(7);
									if(page_id == DELAY_DIS)
									{
										single_cursor_dis(5);
									}
									else 
									{
										param_display(1);
										param_display(2);
										param_display(3);
										param_display(4);
										param_display(5);
										single_cursor_dis(cursor_id);
										//controller_send_data_to_motor(6,0x01, 0x05);
									}
								}
							}

							
						}
						else
						{
							//set_mode_back(page_id);
							page_id = SET_A_POINT;
							cursor_id = 3;
							Set_DDab_set_if(0);
							change_page();
						}

						
					if(m_start)
					{
						if(page_id != DELAY_DIS)
						{
							page_id = DELAY_DIS;
							cursor_id = 4;
							change_page();
						}
						else
						{
							for(i=0;i<temp;i++)
							{
								//TFTDis_Selet(i);
								param_display(i);
							}
							single_cursor_dis(cursor_id);
						}
					}

					cursor_id_back = 2;
					
				}
				else if(app_read_buffer[3] == ORIGIN_SET_MOVE) // origin setting mode 
				{
					page_id = ORIGIN_SET_MOVE;
					cursor_id = 3;
					change_page();
				}
				else if(app_read_buffer[3] == 0x04)
				{
					page_id = MAIN_ID;
					cursor_id = 2;
					change_page();
				}
				else if(app_read_buffer[3] == 0x05) // 标准模式所有参数
				{
					glob_para.delay_time_s = app_read_buffer[4];
					m_start = app_read_buffer[5];
					glob_para.sd_amount = app_read_buffer[6];
					glob_para.shut_time_s = (float)((uint16_t)app_read_buffer[7] | (uint16_t)app_read_buffer[8]<<8) / 1000.0;
					std_manul_tag = app_read_buffer[9];
					glob_para.ssinterval = (float)((uint16_t)app_read_buffer[10] | (uint16_t)app_read_buffer[11]<<8) / 1000.0;
					//glob_para.delay_time_h = app_read_buffer[6];
					//if(page_id != STANDAR_MODE)
					{
						page_id = SINGLE_PANORAMA;
						cursor_id = 1;
						param_display(1);
						param_display(2);
						param_display(3);
						param_display(4);
						param_display(6);
						change_page();
					}
					
				}				
				else if(app_read_buffer[3] == 0x07) // 专业模式
				{
					specialty_get = 1;
					Get_data_from_Motor(app_read_buffer);
					
						if(m_start)
						{
							if(page_id != PREINSTALL_MOVE)
							{
								page_id = PREINSTALL_MOVE;
								cursor_id = 3;
								change_page();
								specialty_totaol_time_dis();
							}
						}
						else
						{
							if(page_id == FINDING_ORIGIN)return;
							if(page_id != PREINSTALL_MODE && page_id != PREINSTALL_SET && page_id !=PREINSTALL_SET1 && page_id !=PREINSTALL_MOVE)
							{
								page_id = PREINSTALL_MODE;
								cursor_id = 1;
								change_page();
								specialty_page_mode_dis();
							}
							
						}
				}
				else if(app_read_buffer[3] == 0x08)// 专业模式总模式ID
				{
					mode_id = app_read_buffer[4];
					mid_cr_back = app_read_buffer[5];
					cear_the_id_add(app_read_buffer[6]);
					m_start = app_read_buffer[7];
					//id_add = mid_cr_back;
					if(page_id == PREINSTALL_MOVE)
					{
						Cunrent_para_display(app_read_buffer[6]);
						param_display(3);
					}
					if(m_start)
					{
						Current_Status_display(1);
						specialty_move_dis(4);
					}
					specialty_totaol_time_dis();
					specialty_page_mode_dis();
					specialty_get = 0;
					if(page_id == PREINSTALL_MOVE)pix_amt_compara_dis(6);
					if(page_id == FINDING_ORIGIN)return;
					if(page_id != PREINSTALL_MODE && page_id != PREINSTALL_SET && page_id !=PREINSTALL_SET1 && page_id !=PREINSTALL_MOVE)
					{
						page_id = PREINSTALL_MODE;
						cursor_id = 1;
						change_page();
						specialty_page_mode_dis();
					}
				}
				else if(app_read_buffer[3] == MAIN_ID) // 
				{
					page_id = MAIN_ID;
					cursor_id = 2;
					change_page();
				}
				else if(app_read_buffer[3] == 9) // manual mode
				{
					//if(page_id != MANUAL_MODE)
					{
						page_id = MANUAL_MODE;
						glob_para.orbital_speed = app_read_buffer[4];
						if(glob_para.orbital_speed>3)glob_para.orbital_speed=3;
						cursor_id = 2;
						change_page();
					}
				}
			//	single_cursor_dis(cursor_id);
				break;
			}
			case 1:
			{
				if(app_read_buffer[3] == 0x00) // 正在自动寻找AB点
				{
					if(app_read_buffer[4] == 0xff)
					{
						page_id = SET_A_POINT;
						LCD_Fill(0, 24, 240, 240, BLACK);
						bornd_code_dis(3, 6);
						//get_init_tag = 0xff;
					}	
				}
				else if(app_read_buffer[3] == 0x01)
				{
					if(app_read_buffer[4] == 0x00) // AB点设置失败
					{
						LCD_Fill(0, 24, 240, 240, BLACK);
						bornd_code_dis(3, 7);
						time_delay_ms(1500);
						page_id = SET_A_POINT;
						cursor_id = 3;
						change_page();
					}
					else if(app_read_buffer[4] == 0x03) // AB点设置成功
					{
						if(return_mode_back() == VIDEO_MODE)
						{
							page_id = VIDEO_MODE;
							cursor_id = 2;
							get_data_form_A650();
						}
						else if(return_mode_back() == DELAY_SET)
						{
							page_id = DELAY_SET;
							cursor_id = 1;
							get_data_form_A650();
						}
						//ab_set_if = 2; // 设置成功志置
						change_page();
					}
				}
				else if(app_read_buffer[3] == 0x02) //自动找原的处理
				{
					if(app_read_buffer[4])
					{
						page_id_back = page_id;
						//String_Printf("BBBB",4);
						//String_Printf(&page_id_back,1);
						page_id = FINDING_ORIGIN;
						cursor_id_back = cursor_id;
						cursor_id = 3;
						change_page();
					}					
					else // 自动找原点完成
					{
						page_id = app_read_buffer[5];
						page_id = page_id_back;
						cursor_id = cursor_id_back;
						if(page_id == PREINSTALL_MOVE)cursor_id = 3;
						//String_Printf("AAAA",4);
						//String_Printf(&page_id,1);
					//	else cursor_id = 5;
						change_page();
						if(page_id == PREINSTALL_SET)
						{
							Page_preinstall_move_return();
						}
						else if(page_id == PREINSTALL_MODE)
						{
							cursor_id = 1;
						//	fanye = 0;
							specialty_page_mode_dis();
						}
						else if(page_id == PREINSTALL_MOVE)
						{
							m_start = 0;
							
							status_display(3);
							if(sp_start==0)cear_the_id_add(1);
							specialty_totaol_time_dis();
							Current_Status_display(2);
							specialty_move_dis(4);
						}
					}
				}
				
				break;
			}
			case 2:
			{
				break;
			}
			case 3:
			{
			/*	if(app_read_buffer[6] == 100)
				{
					battery = 8.4;
				}
				else
				{
					battery = (float)((uint16_t)app_read_buffer[4] | (uint16_t)app_read_buffer[5]<<8) / (float)100;
				}*/
				motor_battery_dis((float)app_read_buffer[6]);
				connection_set();
				if(page_id == LOCUS_PAGE)get_init_tag=0;
				break;
			}
			case 4:
			{
				if(app_read_buffer[3]==0) //定时时间倒计
				{
					cursor_id_back = 2;
					if(page_id == PREINSTALL_MOVE)
					{
						
					}
					
				}
				else if(app_read_buffer[3]==5) // 全景模式倒计时显示
				{
					shotting_Get_data_from_controller(app_read_buffer);
				}
				
				break;
			}
			case 5: // 
			{
				if(app_read_buffer[4] == 0xff) // voltage over of motor
				{
					LCD_Clear(BLACK);
					
					//Oled_EnlPrint(0, 3, "Motor end voltage ","电机端电压超过正常值", wifi_id.language_sel);
					//Oled_EnlPrint(0, 3, "exceeds normal value","电机端电压超过正常值", wifi_id.language_sel);
					}
				else  // 
				{
					change_page();
				}
				break;
			}
			case 6:
			{
				glob_para.tatol_time_s = app_read_buffer[4];
				glob_para.tatol_time_m = app_read_buffer[5];
				glob_para.tatol_time_h = app_read_buffer[6];
				
				if(page_id == DELAY_DIS)param_display(3);
				break;
			}
			case 7:
			{
				p_amount = (uint16_t)app_read_buffer[6] | (uint16_t)app_read_buffer[7]<<8;
				seconds = app_read_buffer[3];
				minutes = app_read_buffer[4];
				hours = app_read_buffer[5];
				m_start = app_read_buffer[8];
				param_display(2);
				if(page_id == DELAY_DIS)Time_DownCount(3);
				//param_display(3);
				param_display(4);
				if(app_read_buffer[8]==0)param_display(5);
				break;
			}
			case 8:
			{
				if(page_id == SET_B_POINT)
				{
					//if(app_read_buffer[3] == 1) //video mode
					V_agnle = (uint16_t)app_read_buffer[4] | (uint16_t)app_read_buffer[5]<<8;
					H_agnle = (uint16_t)app_read_buffer[6] | (uint16_t)app_read_buffer[7]<<8;
					param_display(4);
					param_display(5);
				}
				else if(page_id == MANUAL_MODE)
				{
					//V_agnle = (uint16_t)app_read_buffer[4] | (uint16_t)app_read_buffer[5]<<8;
					H_agnle = (uint16_t)app_read_buffer[6] | (uint16_t)app_read_buffer[7]<<8;
					param_display(4);
					//param_display(5);
				}
				
				break;
			}
			case 0x09: // app for sending times of video mode
			{
				if(app_read_buffer[3] == 2)
				{
					if(app_read_buffer[4] == 3)
					{
						glob_para.duration_h = app_read_buffer[7];
						glob_para.duration_m = app_read_buffer[6];
						glob_para.duration_s = app_read_buffer[5];
						//controller_send_data_to_motor(0,0x01, 0);
						param_display(4);
					}
				}
				else if(app_read_buffer[3] == 1 || app_read_buffer[3] == 0)
				{
					if(app_read_buffer[4] == 1)
					{
						m_start = app_read_buffer[5];
						param_display(4);
					}
				}
				break;
			}
			case 0x0a: // product was select
			{
				break;
			}
			case 0x0b:
			{
				specilty_get_reshot_form_motor(app_read_buffer[3]);
			}
		}
	}
}

void delay_move_page_dis(void)
{
	uint8_t buff[10];
	
	
	if(hours < 10)
	{
		sprintf( (char *)buff,"   0%dH", hours);
	}
	else if(hours < 100)
	{
		sprintf( (char *)buff,"   %dH", hours);
	}
	else if(hours < 1000)
	{
		sprintf( (char *)buff,"  %dH", hours);
	}
	else if(hours < 10000)
	{
		sprintf( (char *)buff," %dH", hours);
	}
	else
	{
		sprintf( (char *)buff,"%dH", hours);
	}
	
	Oled_EnlPrint(24, 2, buff, " ", ENGLISH);

	
	if(minutes < 10)
	{
		sprintf( (char *)buff,"0%dM", minutes);
	}
	else if(minutes < 100)
	{
		sprintf( (char *)buff,"%dM", minutes);
	}
	
	Oled_EnlPrint(60, 2, buff, " ", ENGLISH);
	
	if(seconds < 10)
	{
		sprintf( (char *)buff,"0%dS", seconds);
	}
	else if(seconds < 100)
	{
		sprintf( (char *)buff,"%dS", seconds);
	}
	
	Oled_EnlPrint(96, 2, buff, " ", ENGLISH);
	
	//status_display(3);
}




void delay_mode_tatol_time_dis(void)
{
	uint32_t time_t = 0;
	time_t = (glob_para.shoot_amount - 1) * (glob_para.fram_time_h * 3600 + glob_para.fram_time_m * 60 + \
		glob_para.fram_time_s) + glob_para.shut_time_h * 3600 + glob_para.shut_time_m * 60 + glob_para.shut_time_s;
	
	glob_para.tatol_time_h = time_t / 3600;
	glob_para.tatol_time_m = time_t / 60 - glob_para.tatol_time_h * 60;
	glob_para.tatol_time_s = time_t - glob_para.tatol_time_m * 60 - glob_para.tatol_time_h * 3600;
	
	param_display(3);
}

void Return_main_manual(void)
{
	page_id = MAIN_ID;
	cursor_id = 0;
	change_page();
}

void check_connection(void)
{
	cursor_glint();
	if(connection_flag == 0)
	{
		connection_flag = 10000;//65535;
		cursor_id = 0;
		page_id = LOCUS_PAGE;
		 oled_print_screen();
		 LCD_Fill(0, 140, LCD_W-1, 160, BLACK);
		 motor_battery_dis((float)0);
		get_init_tag = 0;
		
	}
}

void Brightness_dis(uint8_t cur)
{
	uint8_t buff[6]={0};
	uint8_t x_size = 0;
	
	if(wifi_id.brightness < 10)
	{
		sprintf( (char *)buff,"  %d%%", wifi_id.brightness);
	}
	else if(wifi_id.brightness < 100)
	{
		sprintf( (char *)buff," %d%%", wifi_id.brightness);
	}
	else
	{
		sprintf( (char *)buff,"%d%%", wifi_id.brightness);
	}
	
	x_size = Check_String(buff, ENGLISH);
	Oled_EnlPrint(LCD_W-(SCREEN_MIGRATION+x_size), cur, buff, "", ENGLISH);

	
}

void sleep_time_dis(uint8_t cur)
{
	uint8_t buff[6]={0};
	uint8_t x_size = 0;
	
	if(wifi_id.sleep_time < 10)
	{
		sprintf( (char *)buff,"  %dM", wifi_id.sleep_time);
	}
	else
	{
		sprintf( (char *)buff,"%dM", wifi_id.sleep_time);
	}
	
	x_size = Check_String(buff, ENGLISH);
	Oled_EnlPrint(LCD_W-(SCREEN_MIGRATION+x_size), cur, buff, "", ENGLISH);
}

void shutKey_start(void)
{
	controller_send_data_to_motor(0x0b,0, 0);
}

void controller_send_data_to_motor(uint8_t opcode, uint8_t mode, uint8_t data)
{
	uint8_t i;
	i = Check_Buffer_Empty();
	if(i == NOT_EMPTY)return;

	App_Buffer[i].app_send_buffer[0] = 0x90;
	App_Buffer[i].app_send_buffer[2] = 0x09;
	App_Buffer[i].app_send_buffer[1] = opcode;

	if(opcode == 0x00)
	{
		if(mode == 0x01) // video mode
		{
			App_Buffer[i].app_send_buffer[3] = mode;
			App_Buffer[i].app_send_buffer[4] = data;
			if(data == 1) //move time
			{
				App_Buffer[i].app_send_buffer[5] = glob_para.duration_s;
				App_Buffer[i].app_send_buffer[6] = glob_para.duration_m;
				App_Buffer[i].app_send_buffer[7] = glob_para.duration_h;
			}
			else if(data == 0x02) // into the ab point set 
			{
				App_Buffer[i].app_send_buffer[5] = glob_para.orbital_dir;
				App_Buffer[i].app_send_buffer[6] = 0;
				App_Buffer[i].app_send_buffer[7] = 0;
			}
			else if(data == 0x03) // autoreturn
			{
				App_Buffer[i].app_send_buffer[5] = glob_para.auto_return_tag;
				App_Buffer[i].app_send_buffer[6] = 0;
				App_Buffer[i].app_send_buffer[7] = 0;
			}
			else if(data == 0x04) // manual
			{
				App_Buffer[i].app_send_buffer[5] = manual_flag;
				App_Buffer[i].app_send_buffer[6] = 0;
				App_Buffer[i].app_send_buffer[7] = 0;
			}
			else if(data == 0x05) // start / stop
			{
				App_Buffer[i].app_send_buffer[5] = m_start;
				App_Buffer[i].app_send_buffer[6] = 0;
				App_Buffer[i].app_send_buffer[7] = 0;
			}
			else if(data == 0x06) // move to 
			{
				App_Buffer[i].app_send_buffer[5] = glob_para.orbital_dir;
				App_Buffer[i].app_send_buffer[6] = 0;
				App_Buffer[i].app_send_buffer[7] = 0;
			}
			else if(data == 0x07) // ramp
			{
				App_Buffer[i].app_send_buffer[5] = ramp_flag;
				//App_Buffer[i].app_send_buffer[5] = 0;
				App_Buffer[i].app_send_buffer[6] = 0;
				App_Buffer[i].app_send_buffer[7] = 0;
			}
		}
		else if(mode == 0x02) // timelpase mode
		{
			App_Buffer[i].app_send_buffer[3] = mode;
			App_Buffer[i].app_send_buffer[4] = data;
			if(data == 0x01) // interval
			{
				App_Buffer[i].app_send_buffer[5] = glob_para.interval;
				App_Buffer[i].app_send_buffer[6] = glob_para.interval>>8;
				App_Buffer[i].app_send_buffer[7] = 0;
			}
			else if(data == 0x02) // exposure
			{
				App_Buffer[i].app_send_buffer[5] = glob_para.exposure;
				App_Buffer[i].app_send_buffer[6] = glob_para.exposure>>8;
				App_Buffer[i].app_send_buffer[7] = 0;
			}
			else if(data == 0x03) //frames 
			{
				App_Buffer[i].app_send_buffer[5] = glob_para.shoot_amount;
				App_Buffer[i].app_send_buffer[6] = glob_para.shoot_amount>>8;
			}
			else if(data == 0x04) // focus
			{
				App_Buffer[i].app_send_buffer[5] = glob_para.stop_shoot;
			}
			else if(data == 0x05) /// dir
			{
				App_Buffer[i].app_send_buffer[5] = glob_para.orbital_dir;
			}
			else if(data == 0x06) // start / stop
			{
				App_Buffer[i].app_send_buffer[5] = m_start;
			}
			else if(data == 0x07) //move to
			{
				App_Buffer[i].app_send_buffer[5] = glob_para.orbital_dir;
			}
			else if(data == 0x08) // delay time
			{
				App_Buffer[i].app_send_buffer[5] = glob_para.delay_time_s;
				App_Buffer[i].app_send_buffer[6] = glob_para.delay_time_m;
				App_Buffer[i].app_send_buffer[7] = glob_para.delay_time_h;
			}
		}		
		else if(mode == 0x03) // set origin point mode
		{
			App_Buffer[i].app_send_buffer[3] = mode;
			App_Buffer[i].app_send_buffer[4] = data;
			if(data == 0x01) // direction
			{
			//	App_Buffer[i].app_send_buffer[5] = glob_para.orbital_dir;
			}
			else if(data == 0x02) // conform
			{
				if(page_id == SET_A_POINT)App_Buffer[i].app_send_buffer[5] = 0x01;
				else if(page_id == SET_B_POINT)App_Buffer[i].app_send_buffer[5] = 0x02;
			}
			else if(data == 0x04) // RESET
			{
				App_Buffer[i].app_send_buffer[5] = 0XFF;
			}
			//App_Buffer[i].app_send_buffer[6] = 0;
		}
		else if(mode == 0x04) // 单层全景模式
		{
			App_Buffer[i].app_send_buffer[3] = mode;
			App_Buffer[i].app_send_buffer[4] = data;
			//if(data == 0) //speed
			if(data == 0x01)
			{
				App_Buffer[i].app_send_buffer[5] = glob_para.shut_times;
				App_Buffer[i].app_send_buffer[6] = 0;
			}
			else if(data == 0x04) // delay times
			{
				App_Buffer[i].app_send_buffer[5] = glob_para.delay_time_s;
				App_Buffer[i].app_send_buffer[6] = 0;
			}
			else if(data == 0x09) // start / stop
			{
				App_Buffer[i].app_send_buffer[5] = m_start;
				App_Buffer[i].app_send_buffer[6] = 0;
			}
			else if(data == 0x03) // exposure time
			{
				App_Buffer[i].app_send_buffer[5] = glob_para.shut_time_s*1000;
				App_Buffer[i].app_send_buffer[6] = (uint16_t)(glob_para.shut_time_s*1000)>>8;
			}
			else if(data == 0x05) // auto or manul
			{
				App_Buffer[i].app_send_buffer[5] = std_manul_tag;
				App_Buffer[i].app_send_buffer[6] = 0;
			}
			else if(data == 0x02) // inerval time
			{
				App_Buffer[i].app_send_buffer[5] = glob_para.ssinterval*1000;
				App_Buffer[i].app_send_buffer[6] = (uint16_t)(glob_para.ssinterval*1000)>>8;
			}
			else if(data == 0x06)
			{
				App_Buffer[i].app_send_buffer[5] = glob_para.shut_times;
				App_Buffer[i].app_send_buffer[6] = 0;
			}
			else if(data == 0x07) // inerval time
			{
				App_Buffer[i].app_send_buffer[5] = glob_para.sys_stop_t*1000;
				App_Buffer[i].app_send_buffer[6] = (uint16_t)(glob_para.sys_stop_t*1000)>>8;
			}
			else if(data == 0x08) // 转速档位 
			{
				App_Buffer[i].app_send_buffer[5] = glob_para.orbital_speed;
				App_Buffer[i].app_send_buffer[6] = 0;
			}
			
			App_Buffer[i].app_send_buffer[7] = 0;
		}
		else if(mode == 0x05) // 广角模式
		{
			App_Buffer[i].app_send_buffer[3] = mode;
			App_Buffer[i].app_send_buffer[4] = data;
			//if(data == 0) //speed
			if(data == 0x01) // delay times
			{
				App_Buffer[i].app_send_buffer[5] = glob_para.wid_time_s;
				App_Buffer[i].app_send_buffer[6] = 0;
			}
			else if(data == 0x02) // start / stop
			{
				App_Buffer[i].app_send_buffer[5] = m_start;
				App_Buffer[i].app_send_buffer[6] = 0;
			}
			else if(data == 0x03) // exposure time
			{
				App_Buffer[i].app_send_buffer[5] = glob_para.shut_time_s*1000;
				App_Buffer[i].app_send_buffer[6] = (uint16_t)(glob_para.shut_time_s*1000)>>8;
			}
			else if(data == 0x04) // auto or manul
			{
				App_Buffer[i].app_send_buffer[5] = wid_manul_tag;
				App_Buffer[i].app_send_buffer[6] = 0;
			}
			
			App_Buffer[i].app_send_buffer[7] = 0;
		}
		else if(mode == 7) // 手动模式
		{
			App_Buffer[i].app_send_buffer[3] = mode;
			App_Buffer[i].app_send_buffer[4] = data;
			if(data == 1) // speed
			{
				//App_Buffer[i].app_send_buffer[4] = 1;
				App_Buffer[i].app_send_buffer[5] = glob_para.orbital_speed;
				App_Buffer[i].app_send_buffer[6] = 0;
				App_Buffer[i].app_send_buffer[7] = 0;
			}
			else if(data == 2) // direction
			{
				//App_Buffer[i].app_send_buffer[5] = glob_para.orbital_speed;
				//App_Buffer[i].app_send_buffer[6] = 0;
				//App_Buffer[i].app_send_buffer[7] = 0;
			}
		}
		else if(mode == 8) // 专业模式方向键
		{
			App_Buffer[i].app_send_buffer[7] = 0;
		}
	}
	else if(opcode == 0x01)
	{
		App_Buffer[i].app_send_buffer[3] = low_voltage/100;
		App_Buffer[i].app_send_buffer[4] = (low_voltage%100)/10;
		App_Buffer[i].app_send_buffer[5] = (low_voltage%10);
	}
	else if(opcode == 0x04) // connectiong tag
	{
		//App_Buffer[i].app_send_buffer[1] = opcode;
		App_Buffer[i].app_send_buffer[3] = 0x01;
	}
	else if(opcode == 0x05) // APP最大或最小化
	{
		//App_Buffer[i].app_send_buffer[1] = opcode;
		App_Buffer[i].app_send_buffer[3] = 0x01;
		App_Buffer[i].app_send_buffer[4] = data;
	}
	else if(opcode == 0x06) // mode change
	{
		//App_Buffer[i].app_send_buffer[1] = opcode;
		App_Buffer[i].app_send_buffer[3] = data;
		//App_Buffer[i].app_send_buffer[4] = data;
	}	
	else if(opcode == 0x07) // mode change
	{
		//App_Buffer[i].app_send_buffer[1] = opcode;
		App_Buffer[i].app_send_buffer[3] = 0;
		App_Buffer[i].app_send_buffer[4] = 0xff;
		//App_Buffer[i].app_send_buffer[4] = data;
	}
	else if(opcode == 0x08) // disconnect send
	{
		//App_Buffer[i].app_send_buffer[1] = opcode;
		App_Buffer[i].app_send_buffer[3] = 0;
		App_Buffer[i].app_send_buffer[4] = 0xff;
		//App_Buffer[i].app_send_buffer[4] = data;
	}
	else if(opcode == 0x09) // mode change
	{
		//App_Buffer[i].app_send_buffer[1] = opcode;
		App_Buffer[i].app_send_buffer[3] = 0;
		App_Buffer[i].app_send_buffer[4] = 0xff;
		//App_Buffer[i].app_send_buffer[4] = data;
	}
	else if(opcode == 0x0a) // mode change
	{
	//	App_Buffer[i].app_send_buffer[1] = opcode;
		App_Buffer[i].app_send_buffer[3] = 0;
		App_Buffer[i].app_send_buffer[4] = 0xff;
		//App_Buffer[i].app_send_buffer[4] = data;
	}
	else if(opcode == 0x0b) // mode change
	{
	//	App_Buffer[i].app_send_buffer[1] = opcode;
		App_Buffer[i].app_send_buffer[3] = 0x01;
		App_Buffer[i].app_send_buffer[4] = 0xff;
		//App_Buffer[i].app_send_buffer[4] = data;
	}
	
	//App_Buffer[i].app_send_buffer[5] = 0;
	//App_Buffer[i].app_send_buffer[6] = 0;
	//App_Buffer[i].app_send_buffer[7] = 0;
	App_Buffer[i].app_send_buffer[8] = check_sum_add(8, App_Buffer[i].app_send_buffer);
	App_Buffer[i].app_send_size = 9;
}

void connection_set(void)
{
	connection_flag = 10000;
}


// 视频手动模式下左推遥杆再上推，滑轨不停 的处理
uint8_t remotoe_cursor_dispose(uint8_t tage)
{
	return 0;
}


void motor_init(void)
{
	para_read_from_flash();
//	motor_prescale_setting(PRESCALE_32);
//	motor_dirver_mode_select();
//	motor_direction_change();
//	motor_speed_adjust(SLOW_THRESHOLD);
	glob_para.delay_time_h = 0;
	glob_para.delay_time_m = 0;
	glob_para.tatol_time_h = 0;

	parameter_init();
	boot_time = 500;
	cursor_id_back = 2;
	//set_addend_data(1, 0.1);
	//cursor_id = 2;
	//page_id = MAIN_ID;
}

void time_delay_ms(uint16_t counts)
{
	boot_time = counts;
	while(boot_time)
	{
		Feed_IWDG();
	}
}
	
void wait_for_init(void)
{
	connection_flag = 0;
	if(wifi_id.wakaup_tag == 0)
	{
		if(CHARGE_STATUS == CHARGING)
		{
			LCD_Fill(0, 58, LCD_W-1, 135, BLACK);
			set_power_on_int(POWER_OFF);
			TurnOff_ShutDonw_Battery_dis(1, 0, 1);
		//	TurnOff_ShutDonw_Battery_dis(0, 0, 1);
		}
	}
	
//	cursor_id = 0;
//	page_id = LOCUS_PAGE;
	/// oled_print_screen();
	/// LCD_Fill(0, 140, LCD_W-1, 160, BLACK);

	//page_id = MAIN_ID;
	//change_page();
}

uint8_t check_sum_add(uint8_t index, uint8_t *spt)
{
	uint8_t i;
	uint16_t temp=0;

	spt[index] = 0;
	
	for(i=0;i<index; i++)
	{
		temp += (uint16_t)spt[i];
	}

	return (temp & 0xff);
}

void Stander_mode_OK(void)
{
	if(page_id == SINGLE_PANORAMA)
	{
		if(cursor_id == 7)
		{
			page_id = SINGLE_PANORAMA_11;
			cursor_id = 1;
			change_page();
		}
	//	get_data_form_A650();
	}
	else if(page_id == SINGLE_PANORAMA_11)
	{
		if(cursor_id == 3)
		{
			m_start =~m_start;

			controller_send_data_to_motor(0x00,0x04, 0x02);
			param_display(cursor_id);
		}
		else if(cursor_id == 4)
		{
			page_id = SINGLE_PANORAMA;
			cursor_id = 7;
			change_page();
		}
	}
}

void manulORauto_dis(uint8_t dat, uint8_t cur)
{
	uint8_t x = 96;
	uint8_t x_size = 0;
	
	//	inverse_get_value(0xff);
		
		if(dat)
		{
			if(wifi_id.language_sel == ENGLISH)
			{
				x_size = Check_String(" Open", wifi_id.language_sel);
			}
			else if(wifi_id.language_sel == CHINESE)
			{
				x_size = Check_String("开", wifi_id.language_sel);
			}
		}
		else
		{
			if(wifi_id.language_sel == ENGLISH)
			{
				x_size = Check_String("Close", wifi_id.language_sel);
			}
			else if(wifi_id.language_sel == CHINESE)
			{
				x_size = Check_String("关", wifi_id.language_sel);
			}
		}
		
		x = LCD_W - (x_size + SCREEN_MIGRATION);
		Oled_EnlPrint(x, cur, z_buffer,z_buffer,wifi_id.language_sel);
	//	inverse_get_value(0);
}


void slow_start_stop_dis(uint8_t cur)
{
	uint8_t x = 96;
	uint8_t x_size = 0;
	
	//	inverse_get_value(0xff);
		
		if(ramp_flag)
		{
			if(wifi_id.language_sel == ENGLISH)
			{
				x_size = Check_String("Open", wifi_id.language_sel);
			}
			else if(wifi_id.language_sel == CHINESE)
			{
				x_size = Check_String("开", wifi_id.language_sel);
			}
		}
		else
		{
			if(wifi_id.language_sel == ENGLISH)
			{
				x_size = Check_String("Close", wifi_id.language_sel);
			}
			else if(wifi_id.language_sel == CHINESE)
			{
				x_size = Check_String("关", wifi_id.language_sel);
			}
		}
		
		x = LCD_W - (x_size + SCREEN_MIGRATION);
		Oled_EnlPrint(x, cur, z_buffer,z_buffer,wifi_id.language_sel);
	//	inverse_get_value(0);
}

void Manual_page_OK(void)
{
	if(cursor_id == 2)
	{
		if(adj_parm_sel == 0)adj_parm_sel=1;
		else{
		adj_parm_sel=0;
		inverse_get_value(0xff);
		Speed_display(2);
		inverse_get_value(0);
		}
	}
}

void Speed_display(uint8_t cur)
{
	uint8_t buff[10];
	uint8_t x=0;
	uint8_t x_size=0;
	
	//if(glob_para.orbital_speed<10)
	{
		sprintf( (char *)buff,"%d", glob_para.orbital_speed);
	}
	
	x_size = Check_String(buff, ENGLISH);
	x = LCD_W - (x_size + SCREEN_MIGRATION+24);
	Oled_EnlPrint(x, cur, buff, " ", ENGLISH);
}

void Video_mode_cursor_shift(uint8_t dir)
{
	uint8_t temp = 0;
	if(page_id != VIDEO_MODE)return;
	if(cursor_id != 2)return;
	if(adj_parm_sel ==0)return;
	if(dir & KEY_RIGHT_MASK)
	{
		if(adj_parm_sel < 3)adj_parm_sel += 1;
		temp = 1;
	}
	else if(dir & KEY_LEFT_MASK)
	{
		if(adj_parm_sel > 1)adj_parm_sel -=1;
		temp = 1;
	}

	if(temp)
	{
		inverse_get_value(0xff);
		hours_time_dis(glob_para.duration_h, cursor_id);
		minutes_time_dis(glob_para.duration_m, cursor_id);
		secondes_time_dis(glob_para.duration_s, cursor_id);
		inverse_get_value(0);
	}
}

//张数显示
void Delay_pix_amt_display(uint8_t cur, uint8_t numb)
{
	uint8_t buff[10];
	uint8_t x=0;
	uint8_t x_size = 0;
//	uint32_t amounts = 0;
	
	sprintf( (char *)buff,"%d", D_amount[numb]);	
	
	x_size = Check_String(buff, ENGLISH);
	x = LCD_W - (x_size + SCREEN_MIGRATION + numb*6);
	Oled_EnlPrint(x, cur, buff, " ", ENGLISH);
}

//延时模式下的张数拆分为个十百千四位数来独立调节并组合为一个整型数
void Delay_amount_adjust(uint8_t dir)
{
	if(cursor_id != 3)return;
	
	inverse_get_value(0xff);
	if(adj_parm_sel==1)
	{
		//D_amount[0] = data_count(D_amount[0],dir,  9, 1);
		if(dir & KEY_UP_MASK)
		{
			if(D_amount[0]<9)D_amount[0] +=1;
		}
		else if(dir & KEY_DOWN_MASK)
		{		
			if(D_amount[1]==0 && D_amount[2]==0 && D_amount[3]==0)
			{
				if(D_amount[0]>1)D_amount[0] -=1;
			}
			else
			{
				if(D_amount[0])D_amount[0] -=1;
			}
		}

		Delay_pix_amt_display(cursor_id, adj_parm_sel-1);
	}
	else if(adj_parm_sel==2)
	{
		if(dir & KEY_UP_MASK)
		{
			if(D_amount[1]<9)D_amount[1] +=1;
		}
		else if(dir & KEY_DOWN_MASK)
		{
			if(D_amount[1])D_amount[1] -=1;
		}
		Delay_pix_amt_display(cursor_id, adj_parm_sel-1);
	}
	else if(adj_parm_sel==3)
	{
		if(dir & KEY_UP_MASK)
		{
			if(D_amount[2]<9)D_amount[2] +=1;
		}
		else if(dir & KEY_DOWN_MASK)
		{
			if(D_amount[2])D_amount[2] -=1;
		}
		Delay_pix_amt_display(cursor_id, adj_parm_sel-1);
	}
	else if(adj_parm_sel==4)
	{
		if(dir & KEY_UP_MASK)
		{
			if(D_amount[3]<9)D_amount[3] +=1;
		}
		else if(dir & KEY_DOWN_MASK)
		{
			if(D_amount[3])D_amount[3] -=1;
		}
		Delay_pix_amt_display(cursor_id, adj_parm_sel-1);
	}

	if(D_amount[1]==0 && D_amount[2]==0 && D_amount[3]==0 && D_amount[0]==0)
	{
		D_amount[0] = 1;
		Delay_pix_amt_display(cursor_id, 0);
	}
	glob_para.shoot_amount = (uint16_t)D_amount[0] + (uint16_t)D_amount[1]*10 + (uint16_t)D_amount[2]*100 + (uint16_t)D_amount[3]*1000;
	
	//if(glob_para.shoot_amount < 24)glob_para.shoot_amount = 24;
	//if(glob_para.shoot_amount > 10000)glob_para.shoot_amount = 10000;
	
	//param_display(cursor_id);
	if_write_flash();
	inverse_get_value(0);
	controller_send_data_to_motor(0,0x02, 0x03);
}

void Delay_mode_cursor_shift(uint8_t dir)
{
	//uint8_t temp = 0;
	if(page_id != DELAY_SET)return;
	if(cursor_id != 3)return;
	if(adj_parm_sel ==0)return;
	if(dir & KEY_RIGHT_MASK)
	{
		if(adj_parm_sel > 1)adj_parm_sel -= 1;
	//	temp = 1;
	}
	else if(dir & KEY_LEFT_MASK)
	{
		if(adj_parm_sel < 4)adj_parm_sel +=1;
	//	temp = 1;
	}

	/*if(temp)
	{
		inverse_get_value(0xff);
		hours_time_dis(glob_para.duration_h, cursor_id);
		minutes_time_dis(glob_para.duration_m, cursor_id);
		secondes_time_dis(glob_para.duration_s, cursor_id);
		inverse_get_value(0);,
	}*/
}

void UpOrDown_adjust_para(uint8_t dir)
{
	if(page_id == VIDEO_MODE)
	{
		if(cursor_id == 2) // move time
		{
			if(adj_parm_sel==0)return;
			inverse_get_value(0xff);
			if(adj_parm_sel == 1)
			{
			//	glob_para.duration_h = data_count(glob_para.duration_h, dir, 2, 0);
				if(dir & KEY_UP_MASK)glob_para.duration_h +=1;
				else if(dir & KEY_DOWN_MASK)if(glob_para.duration_h)glob_para.duration_h -=1;
				if(glob_para.duration_h > 2)glob_para.duration_h = 2;
				hours_time_dis(glob_para.duration_h, cursor_id);
			}
			else if(adj_parm_sel == 2)
			{
				//glob_para.duration_m = data_count(glob_para.duration_m, dir, 59, 0);
				if(dir & KEY_UP_MASK)glob_para.duration_m +=1;
				else if(dir & KEY_DOWN_MASK)if(glob_para.duration_m)glob_para.duration_m -=1;
				if(glob_para.duration_m > 59)glob_para.duration_m = 59;
				minutes_time_dis(glob_para.duration_m, cursor_id);
			}
			else if(adj_parm_sel == 3)
			{
				//glob_para.duration_s = data_count(glob_para.duration_s, dir, 59, 0);
				if(dir & KEY_UP_MASK)glob_para.duration_s +=1;
				else if(dir & KEY_DOWN_MASK)if(glob_para.duration_s)glob_para.duration_s -=1;
				if(glob_para.duration_s > 59)glob_para.duration_s = 59;
				secondes_time_dis(glob_para.duration_s, cursor_id);
			}
			else
			{
				inverse_get_value(0);
				return;
			}
			
			
			inverse_get_value(0);
			controller_send_data_to_motor(0,0x01, 0x01);
		}
	}
	else if(page_id == DELAY_SET)
	{
		if(cursor_id == 3)
		{
			//delay_mode_tatol_time_dis();
			Delay_amount_adjust(dir);
		}
	}
}

void angle_cear(void)
{
	V_agnle = 0;
	H_agnle = 0;
}

