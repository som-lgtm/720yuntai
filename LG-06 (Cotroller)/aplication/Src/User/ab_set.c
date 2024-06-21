
#include "oled.h"
#include "common.h"
#include "pwm.h"
#include "config.h"
#include "keyscan.h"
#include "orbital.h"
#include "motor_dri.h"
#include "stdio.h"
#include "uart.h"

uint8_t ab_set_if = 0;
uint8_t DDab_set_if = 0;
uint8_t mode_back = 0;
SLIDER_POSE_STRUCT Poses[3]={0};
YUNTAI_POSE_STRUCT yuPoses[3]={0};

void set_mode_back(uint8_t datad)
{
	mode_back = datad;
}

uint8_t return_mode_back(void)
{
	return mode_back;
}

void Set_ab_set_if(uint8_t datas)
{
	ab_set_if = datas;
}

void Set_DDab_set_if(uint8_t datas)
{
	DDab_set_if = datas;
}

uint8_t check_abpoint_Set_if(uint8_t modes)
{
	uint8_t temp = 0;
	
	if(modes == 1)
	{
		if(ab_set_if == 3)
		{
			temp = 1;
		}
	}
	else if(modes == 2)
	{
		if(DDab_set_if == 3)
		{
			temp = 1;
		}
	}

	return temp;
}

void display_test11(uint8_t param, uint8_t cur)
{
	uint8_t buff[5];
	static uint8_t i=0;
	
	
	if(param < 10)
	{
		sprintf( (char *)buff,"00%dM", param);
	}
	else if(param < 100)
	{
		sprintf( (char *)buff,"0%dM", param);
	}
	else
	{
		sprintf( (char *)buff,"%dM", param);
	}

	Oled_EnlPrint(0, cur<<1, buff, " ", ENGLISH);
	
	if(i<4)i++;
	else i=0;
}

void ab_set_move_upkey(void)
{
	uint8_t i;
	i = Check_Buffer_Empty();
	if(i == NOT_EMPTY)return;

	App_Buffer[i].app_send_buffer[5] = 0x01;
	App_Buffer[i].app_send_buffer[6] = 0;

	controller_send_data_to_motor(0,0x03, 0x01);
}

void ab_set_move(uint8_t dir, uint8_t types)
{
	uint8_t i;
	static uint8_t tempp0=0;
	static uint8_t tempp1=0;

	if((page_id != SET_A_POINT)&&(page_id != SET_B_POINT)&&(page_id != ORIGIN_SET_MOVE)&&(page_id != MANUAL_MODE))return;
	if(page_id == MANUAL_MODE)if(adj_parm_sel)return;
	i = Check_Buffer_Empty();
	if(i == NOT_EMPTY)return;

	if(types)
	{
		if(dir & KEY_LEFT_MASK)
		{
			if(page_id == SET_A_POINT || page_id == SET_B_POINT)if(tempp0)return;
			tempp1 = 1;
			glob_para.orbital_dir = B_TO_A;
			App_Buffer[i].app_send_buffer[5] = 0x01;
			App_Buffer[i].app_send_buffer[6] = B_TO_A;
			controller_send_data_to_motor(0,0x03, 0x01);
		}
		else if( dir & KEY_RIGHT_MASK)
		{
			if(page_id == SET_A_POINT || page_id == SET_B_POINT)if(tempp0)return;
			tempp1 = 1;
			App_Buffer[i].app_send_buffer[5] = 0x02;
			App_Buffer[i].app_send_buffer[6] = 0xff;
			controller_send_data_to_motor(0,0x03, 0x01);
			
		}
		else
		{
			tempp1 = 0;
			App_Buffer[i].app_send_buffer[5] = 0x01;
			App_Buffer[i].app_send_buffer[6] = 0;
			controller_send_data_to_motor(0,0x03, 0x01);
		}
	}
	else
	{
		if(dir & KEY_UP_MASK)
		{
			if(page_id == SET_A_POINT || page_id == SET_B_POINT)if(tempp1)return;
			tempp0 = 1;
			App_Buffer[i].app_send_buffer[5] = 0x03;
			App_Buffer[i].app_send_buffer[6] = B_TO_A;
			controller_send_data_to_motor(0,0x03, 0x01);
		}
		else if(dir & KEY_DOWN_MASK)
		{
			if(page_id == SET_A_POINT || page_id == SET_B_POINT)if(tempp1)return;
			tempp0 = 1;
			App_Buffer[i].app_send_buffer[5] = 0x04;
			App_Buffer[i].app_send_buffer[6] = B_TO_A;
			controller_send_data_to_motor(0,0x03, 0x01);
		}
		else
		{
			tempp0 = 0;
			App_Buffer[i].app_send_buffer[5] = 0x03;
			App_Buffer[i].app_send_buffer[6] = 0;
			controller_send_data_to_motor(0,0x03, 0x01);
		}
	}
	
/*	else if(dir & KEY_UP_MASK)
	{
		ab_set_move_upkey();
	}
	else if(dir & KEY_DOWN_MASK)
	{
		ab_set_move_upkey();
	}
	else if(dir & KEY_OK_MASK)
	{
		//ab_set_move_upkey();
	}*/

}

//’≈ ˝œ‘ æ
void test_amt_display(uint8_t cur, int bbbb)
{
	uint8_t buff[10];

	sprintf( (char *)buff,"%d", bbbb);
	
	Oled_EnlPrint(0, cur<<1, buff, " ", ENGLISH);
}

void testr_dir_display(uint8_t cur, uint8_t dddd)/////////////////////////////////////////////////////
{
//	uint8_t x=0;

	//if(glob_para.language_sel)
//	if(page_id != MANUAL_MODE)return;
	
	if(cursor_id == VINDEX_THREE)inverse_get_value(0xff);
	
	if(dddd)
	{
		Oled_EnlPrint(32, cur<<1,"<", "◊Û", ENGLISH);
		
		Oled_EnlPrint(88, cur<<1,"  ", "”“", ENGLISH);
	}
	else
	{
		if(m_start)Oled_EnlPrint(88, cur<<1,"> ", "”“", ENGLISH);
		else Oled_EnlPrint(88, cur<<1," >", "”“", ENGLISH);
		
		Oled_EnlPrint(32, cur<<1," ", "◊Û", ENGLISH);
	}
	
	if(cursor_id == VINDEX_THREE)inverse_get_value(0);

//	orbital_run();	

}

void ab_set_press_ok(void)
{
	if(page_id == ORIGIN_SET_MOVE)
	{
		controller_send_data_to_motor(0,0x03, 0x02);

		page_id = MAIN_ID;
		cursor_id = 2;
		change_page();
	}
	else if(page_id == SET_A_POINT)
	{
		controller_send_data_to_motor(0,0x03, 0x02);

		page_id = SET_B_POINT;
		cursor_id = 2;
		angle_cear();
		change_page();
		//if(wifi_id.language_sel == CHINESE)Oled_EnlPrint(93, 3, "1","", ENGLISH);
	}
	else if(page_id == SET_B_POINT)
	{
		controller_send_data_to_motor(0,0x03, 0x02);
	}
}

