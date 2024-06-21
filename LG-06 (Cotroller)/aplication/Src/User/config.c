#include "config.h"
#include "oled.h"
#include "common.h"
#include "orbital.h"
#include "keyscan.h"
#include "uart.h"
#include "ab_set.h"
#include "stm32f0xx_ll_tim.h"

uint8_t page_id = 0;
uint8_t cursor_id = 0;
uint8_t cursor_id_back = 0;
uint8_t Factory_set=0;
uint8_t auto_fine_abpoint=0;
uint8_t language_confirm=0;
uint8_t device_confirm=0;

void para_init(void)
{
	glob_para.orbital_dir = B_TO_A;
	glob_para.orbital_speed = 100;
	glob_para.auto_return_tag = 1;
	glob_para.shoot_amount = 168;
	wifi_id.language_sel = CHINESE;
	wifi_id.sleep_time = 5;
	wifi_id.brightness = 20;
	glob_para.duration_h = 0;
	glob_para.duration_m = 10;
	glob_para.duration_s = 0;
	glob_para.delay_time_h = 0;
	glob_para.delay_time_m = 0;
	glob_para.tatol_time_h = 0;
	glob_para.tatol_time_m = 9;
	glob_para.fram_time_h = 0;
	glob_para.fram_time_m = 0;
	glob_para.fram_time_s = 0;
	glob_para.shut_time_h = 0;
	glob_para.shut_time_m = 0;
	glob_para.shut_time_s = 0;
}

//language select display:english or chinese
void language_display(uint8_t p_langu)
{
	/*if(glob_para.language_sel== ENGLISH)
	{
		Oled_EnlPrint(72, p_langu<<1,"english", "英语", glob_para.language_sel);
	}
	else if(glob_para.language_sel == CHINESE)
	{
		Oled_EnlPrint(72, p_langu<<1,"chinese", "中文", glob_para.language_sel);
	}
	else if(glob_para.language_sel == KOREANH)
	{
		Oled_EnlPrint(72, p_langu<<1,"-", "中文", glob_para.language_sel);
	}*/
}

//factory set display : Y是 or N否
void factory_set_display(uint8_t cur)
{
	uint8_t x_size = 0;
	
	x_size = Check_String("Y", ENGLISH);
	x_size = LCD_W - (x_size + SCREEN_MIGRATION);
	
	if(Factory_set)
	{
		Oled_EnlPrint(x_size, cur,"Y", "是", ENGLISH);
	}
	else
	{
		Oled_EnlPrint(x_size, cur,"N", "否", ENGLISH);
	}
}

//auto finding ab point 
void auto_fine_abpoint_display(uint8_t cur)
{
	uint8_t x_size = 0;
	
	x_size = Check_String("Y", ENGLISH);
	x_size = LCD_W - (x_size + 12);
	
	if(auto_fine_abpoint)
	{
		Oled_EnlPrint(x_size, cur,"Y", "是", ENGLISH);
	}
	else
	{
		Oled_EnlPrint(x_size, cur,"N", "否", ENGLISH);
	}
}


void cursor_display(uint8_t maax, uint8_t miix, uint8_t dir)
{
	if(cursor_id == miix)
	{
		//reverse_cur = 0;
		
		/*if(page_id != CONFIG_ID)
		{
			oled_print_page(3);
			param_display(3);
			
			oled_print_page(2);
			param_display(2);
		}*/

		oled_print_page(cursor_id+1);
		param_display(cursor_id+1);
		
		single_cursor_dis(cursor_id);
	}
	else if(cursor_id == maax)
	{
		oled_print_page(cursor_id-1);
		param_display(cursor_id-1);

		//oled_print_page(cursor_id+1);
		//param_display(cursor_id+1);

		single_cursor_dis(cursor_id);
	}
	else 
	{
		if(dir & KEY_DOWN_MASK)
		{
			oled_print_page(cursor_id-1);
			param_display(cursor_id-1);
		}
		
		if(dir & KEY_UP_MASK)
		{
			oled_print_page(cursor_id+1);
			param_display(cursor_id+1);
		}

		single_cursor_dis(cursor_id);
	}
}



void cursor_count(uint8_t dir, uint8_t max, uint8_t mix)/////////////////////////////////////////////////////////////////
{
	if(dir & KEY_DOWN_MASK)
	{
		if(cursor_id < max)
		{
			cursor_id++;
		}
		else
		{
		//	cursor_id = mix;
			return;
		}
	}
	else if(dir & KEY_UP_MASK)
	{
		if(cursor_id>mix)
		{
			cursor_id--;
		}
		else
		{
		//	cursor_id = max;
			return;
		}
	}

	if(page_id == PREINSTALL_MODE || page_id == PREINSTALL_SET || page_id == PREINSTALL_SET1 || page_id == PREINSTALL_MOVE)
	{
		specialty_cursor_id_dis(dir);
	}
	else
	{
		cursor_display(max, mix, dir);
	}
}

uint8_t Check_Buffer_Empty(void)
{
	uint8_t i;
	
	for(i = 0; i< LOCUS_BUFFER_MAX; i++)
	{
		if(App_Buffer[i].app_send_size == 0)return i;
	}
	
	return NOT_EMPTY;
}

void main_page_ok(void)
{
	
	if(page_id == MAIN_ID)
	{
		if(cursor_id == 2)
		{			
			page_id = PREINSTALL_MODE;
			cursor_id = 1;
			change_page();
			specialty_page_mode_dis();
			get_data_form_A650();
			
			Set_enter_time();
		}
		else if(cursor_id == 3)
		{
		//	page_id = VIDEO_MAIN;
		//	cursor_id = 1;
		//	change_page();
			page_id = VIDEO_MODE;
			set_mode_back(page_id);
			get_data_form_A650();	
			page_id = MAIN_ID;
			/*if(check_abpoint_Set_if())
			{
				cursor_id = 1;
				change_page();
			}
			else
			{
				set_mode_back(page_id);
				page_id = SET_A_POINT;
				Set_ab_set_if(0);
				change_page();
			}
			*/
			
		}
		else if(cursor_id == 4)
		{			
			page_id = DELAY_SET;
			set_mode_back(page_id);
			get_data_form_A650();	
			page_id = MAIN_ID;
			
		}
		else if(cursor_id == 5)
		{
			page_id = MANUAL_MODE;
			cursor_id = 2;
			change_page();
			get_data_form_A650();	
			//version_dis();
		}
		else if(cursor_id == 6)
		{
			page_id = CONFIG_ID;
			cursor_id = 1;
			change_page();
			version_dis();

		}
		else if(cursor_id == 1)
		{
			//set_mode_back(page_id);
			page_id = ORIGIN_ACK;
			cursor_id = 3;
			change_page();
		}
	}
}


void config_page_ok(void)
{
	if(page_id != CONFIG_ID)return;
	
	if(cursor_id == 2)
	{
		if(Factory_set)
		{
			controller_send_data_to_motor(0x09,0, 0x03);
			page_id = MAIN_ID;
			cursor_id = 1;
			Factory_set = 0;
			specialty_mode_para_clear();
			para_init();
			if_write_flash();
			write_flash_active();
			LL_TIM_OC_SetCompareCH4(TIM3, wifi_id.brightness);
			change_page();
			device_confirm = 0xff;
			boot_time = 100;
		}

	}

	write_flash_active();
}


void factory_cofirm(void)
{

}

void page_return_adjust(void)
{
	if(page_id == ORIGIN_SET_MOVE)return;
	if(page_id > LOCUS_PAGE)
	{
		if(page_id == PREINSTALL_SET || page_id == PREINSTALL_SET1 || page_id == PREINSTALL_MOVE)
		{
			specilty_page_return();
		}
		else
		{
			page_id = MAIN_ID;
			cursor_id = cursor_id_back;
			reverse_cur = 0;
			adj_parm_sel = 0;
			m_start = 0;
			specialty_para_clear();
			controller_send_data_to_motor(0x06,0, 0x04); // return the main manual
			inverse_get_value(0);
			change_page();
			LCD_Fill(0, 140, LCD_W-1, 160, BLACK);
		}
		write_flash_active();
		
	}
}


