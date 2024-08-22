#include "stm32f030x8.h"
#include "keyscan.h"
#include "uart.h"
#include "config.h"
#include "orbital.h"
#include "common.h"
#include "ab_set.h"
#include "oled.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_bus.h"
#include "GPIO.h"

//#define Bootooth_disconnect		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)//ADCIN2
//#define RETURN_KEY1	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10)
//#define RETURN_KEY2	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)


uint8_t scan_cycle=0;
uint8_t key_final_status=0;
uint8_t return_key=0;
uint16_t return_key_time=0;
uint8_t shutter_key_satus = 0;

uint8_t keyup_status=0;
uint8_t keydown_status=0;
uint8_t keyleft_status=0;
uint8_t keyright_status=0;
uint8_t keyok_status=0;
uint8_t keyPwr_status=0;
uint8_t keyMain_status=0;
uint8_t keyShut_status=0;
uint8_t keypress=0;

uint8_t keyreturn_left=0;
uint8_t keyreturn_right=0;
uint8_t key_debounce = 0;
uint8_t Pwrkey_debounce = 0;
uint16_t key_contunue_cout=0;
uint8_t powers_off = 0;

uint16_t press_time = 0; // 电源开关误触计算时间3s

uint16_t continues = 0;

extern uint16_t add_contunue;
extern uint16_t sub_contunue;


void set_press_time(void)
{
	press_time = 3000;
}


unsigned char key_Handle(void)
{
	uint8_t p_keyup=0;
	uint8_t p_keydown=0;
	uint8_t p_keyleft=0;
	uint8_t p_keyright=0;
	uint8_t p_keyok=0;
	uint8_t shuuters;
	uint8_t mian_k=0;
	uint8_t pwr =0;
	//unsigned char buttun6=0;
	
//	p_keyup = ~(KeyUp|0xfe);
//	p_keydown = ~(KeyDown|0xfe);
//	p_keyleft = ~(KeyLeft|0xfe);
//	p_keyright = ~(KeyRight|0xfe);
//	p_keyok = ~(KeyOk|0xfe);

// extern open drain:
	p_keyup = KeyUp;
	p_keydown = KeyDown;
	p_keyleft = KeyLeft;
	p_keyright = KeyRight;
	p_keyok = KeyOk;
	shuuters = SHUTTER;
	mian_k = MAIN_KEY;
	pwr = PWR_KEY;
	
	//shuuters = SHUTTER;
	return (p_keyup | (p_keydown<<1) | (p_keyleft<<2) | (p_keyright<<3) | (p_keyok<<4)| (shuuters<<5)| (mian_k<<6) | (pwr<<7));
	
}

void keyscan(void)
{
	uint8_t key_status=0;
//	static uint8_t bt_flag_back=0xff;
	
	Pwr_key_continue();
	if(press_time == 1)
	{
		press_time = 0;
		check_the_powers_status();
	}

	if(scan_cycle)
		return;
	scan_cycle = 8;//20;
	
	key_status = key_Handle();
		
	if((key_status & KEY_PWR_MASK)==0)
	{
		if(return_powers())
		{
			Pwrkey_debounce += 1;
			if(Pwrkey_debounce >= 50)
			{
				Pwrkey_debounce = 0;
				continues = 0;
				press_time = 0;
				check_the_powers_status(); // Continue standby for less than 1S
			}
		}
	}
		
	if(key_status != key_final_status)
	{
		key_debounce++;
		if(key_debounce < 5)return;

		key_debounce = 0;
		//if(key_status && key_final_status)return;
		//key_final_status = key_status;
		//if(key_status && key_final_status)return;
		key_final_status = key_status;
		
		
		LowBattery_wakeup();
		//if(key_status)keypress = 0xff;
	//	String_Printf(&key_final_status, 1);
///////////////////////////////////////////////////////////////////////
		if((key_final_status & KEY_LEFT_MASK) != keyleft_status)
		{
			keyleft_status = key_final_status & KEY_LEFT_MASK;
		//	if((key_final_status&KEY_RIGHT_MASK) || (key_final_status&KEY_UP_MASK) || (key_final_status&KEY_DOWN_MASK))return;
			if(keyleft_status)
			{
				param_adjust(keyleft_status);
				Video_mode_cursor_shift(keyleft_status);
				Delay_mode_cursor_shift(keyleft_status);
				specilty_keyscan_send(keyleft_status);
			}
			ab_set_move(keyleft_status, 1);
		//	manual_move(keyleft_status,1);
			press_key_continue_if(keyleft_status);
		}
//////////////////////////////////////////////////////////////////////////
		if((key_final_status & KEY_RIGHT_MASK) != keyright_status)
		{
			keyright_status = key_final_status & KEY_RIGHT_MASK;
			//if((key_final_status&KEY_LEFT_MASK) || (key_final_status&KEY_UP_MASK) || (key_final_status&KEY_DOWN_MASK))return;
			if(keyright_status)
			{
				param_adjust(keyright_status);
				Video_mode_cursor_shift(keyright_status);
				Delay_mode_cursor_shift(keyright_status);
				specilty_keyscan_send(keyright_status);
			}
			
			ab_set_move(keyright_status,1);
			//manual_move(keyright_status,1);
			press_key_continue_if(keyright_status);
		}
		///////////////////////////////////////////////////////////////////
		if((key_final_status & KEY_UP_MASK) != keyup_status)
		{
			keyup_status = key_final_status & KEY_UP_MASK;
			if(remotoe_cursor_dispose(keyup_status))return;
		//	if((key_final_status&KEY_RIGHT_MASK) || (key_final_status&KEY_LEFT_MASK) || (key_final_status&KEY_DOWN_MASK))return;
			if(keyup_status)
			{
				cursor_shift(keyup_status);
				UpOrDown_adjust_para(keyup_status);
			}
			
			//remotoe_cursor_dispose(keyup_status);
			ab_set_move(keyup_status,0);
		//	manual_move(keyup_status,0);

		}
///////////////////////////////////////////////////////////////////////
		if((key_final_status & KEY_DOWN_MASK) != keydown_status)
		{
			keydown_status = key_final_status & KEY_DOWN_MASK;
		//	if((key_final_status&KEY_RIGHT_MASK) || (key_final_status&KEY_UP_MASK) || (key_final_status&KEY_LEFT_MASK))return;
			
			if(keydown_status)
			{
				cursor_shift(keydown_status);
				UpOrDown_adjust_para(keydown_status);
			}
			ab_set_move(keydown_status,0);
		//	manual_move(keydown_status,0);
		//	remotoe_cursor_dispose(keydown_status);
		}
////////////////////////////////////////////////////////////////////////
		if((key_final_status & KEY_OK_MASK) != keyok_status)
		{
			keyok_status = key_final_status & KEY_OK_MASK;
			if(keyok_status)
			{
				return_key_time = 1000;
			}
			else
			{
				if(return_key == 0)
				{
					press_OK_function();
					//uart_send_data(0);
				}
				
				return_key_time = 0;
				return_key = 0;
			}
		}

		if((key_final_status & KEY_PWR_MASK) != keyPwr_status)
		{
			keyPwr_status = key_final_status & KEY_PWR_MASK;
			if(keyPwr_status)
			{
				if(return_powers() == 0)
				{
					continues = 2001;
				}
				else
				{
					continues = 501;
				}
			}
			else
			{
				continues = 0;
				pwr_key_enter_moveInterfer();
				check_the_powers_status(); // Continue standby for less than 1S
				//String_Printf(&wifi_id.wifi_rfc, 6);
			}
		}
		
		if((key_final_status & KEY_MAIN_MASK) != keyMain_status)
		{
			keyMain_status = key_final_status & KEY_MAIN_MASK;
			if(keyMain_status)
			{
			}
			else
			{
				//continues = 0;
				//RED_LED_ON;
				page_return_adjust();
			}
		}
		
		if((key_final_status & SHUTTER_KEY_MASK) != keyShut_status)
		{
			keyShut_status = key_final_status & SHUTTER_KEY_MASK;
			if(keyShut_status)
			{
				shutKey_start();
			}
		}

		
//		press_key_continue_if(key_final_status);

//////////////////////////////////////////////////////////////////	
	}
	
	press_key_continue(key_final_status);
	page_return(); // 专业模式按住OK KEY 删除参数
}


void scan_cycle_count(void)
{
	if(scan_cycle)scan_cycle--;
	if(return_key_time > 1)return_key_time--;
	if(add_contunue>1)add_contunue--;
	if(sub_contunue>1)sub_contunue--;
	if(key_contunue_cout)key_contunue_cout--;
	if(continues>1)continues--;
	if(press_time>1)press_time--;
}

void boot_screen(void)
{
	//time_delay_ms(800);	
	//Feed_IWDG(); //喂狗
	
}

void press_OK_function(void)
{	
	if(page_id == MAIN_ID)
	{
		main_page_ok();
	}
	else if(page_id == DELAY_SET)
	{
		delay_page_ok();
	}
	else if(page_id == DELAY_DIS)
	{
		delay_page_ok();
	}
	else if(page_id == VIDEO_MODE)
	{
		video_page_OK();
	}
	else if(page_id == GROUP_PHOTO)
	{
		Group_page_OK();
	}
	else if(page_id == CONFIG_ID)
	{
		config_page_ok();
	}
	else if((page_id == SET_A_POINT))
	{
		ab_set_press_ok();
	}
	else if((page_id == SET_B_POINT))
	{
		ab_set_press_ok();
	}
	else if((page_id == ORIGIN_SET_MOVE))
	{
		ab_set_press_ok();
	}
	else if(page_id == PREINSTALL_MODE || page_id == PREINSTALL_SET || page_id == PREINSTALL_SET1 || page_id == PREINSTALL_MOVE || page_id == PREINSTALL_SESHOT)
	{
		specialty_mode_OKkey();
	}
	else if(page_id == ORIGIN_ACK)
	{
		if(cursor_id == 3)
		{
			page_id = MAIN_ID;
			cursor_id = 2;
			change_page();
			get_data_form_A650();
		}
		else if(cursor_id == 4)
		{
			page_id = ORIGIN_SET_MOVE;
			cursor_id = 3;
			change_page();
			get_data_form_A650();
		}
	}
	else if(page_id == REVERSE_MODE)
	{
		if(return_mode_back() == VIDEO_MODE)
		{
			if(cursor_id == 3)
			{
				page_id = VIDEO_MODE;
				cursor_id = 5;
				change_page();
			}
			else if(cursor_id == 4)
			{
				page_id = SET_A_POINT;
				cursor_id = 3;
				ab_set_if = 0;
				angle_cear();
				//delay_mode_total_time_verify();
				change_page();
				
				controller_send_data_to_motor(0x06,0, 9);
			}
		}
		else if(return_mode_back() == DELAY_SET)
		{
			if(cursor_id == 3)
			{
				page_id = DELAY_SET;
				cursor_id = 5;
				change_page();
			}
			else if(cursor_id == 4)
			{
				page_id = SET_A_POINT;
				cursor_id = 3;
				ab_set_if = 0;
				angle_cear();
				//delay_mode_total_time_verify();
				change_page();
				
				controller_send_data_to_motor(0x06,0, 9);
			}
		}		
		else if(return_mode_back() == GROUP_PHOTO)
		{
			if(cursor_id == 3)
			{
				page_id = GROUP_PHOTO;
				cursor_id = 6;
				change_page();
			}
			else if(cursor_id == 4)
			{
				page_id = SET_A_POINT;
				cursor_id = 3;
				ab_set_if = 0;
				angle_cear();
				//delay_mode_total_time_verify();
				change_page();
				
				controller_send_data_to_motor(0x06,0, 9);
			}
		}
	}
	else if(page_id == MANUAL_MODE)
	{
		Manual_page_OK();
	}
}

void page_return(void)
{
	if(return_key_time == 1)
	{
		return_key = 1;
		return_key_time = 0;
		//page_return_adjust();
		delet_paraORmode();
	}
}


void press_key_continue(uint8_t dir)
{
//	static uint8_t temp = 0;

	if((add_contunue < 10) && (add_contunue > 0)/*||(sub_contunue == 1)*/)
	{
		if(key_contunue_cout == 0)
		{
			key_contunue_cout = 100;

			param_adjust(dir);
		}
	}

	if((add_contunue < 4000) && (add_contunue > 2000))
	{
		//if(temp > 0)return;
		//temp = 1;
		set_addend_data(10, 1);
	}
	else if((add_contunue < 2000) && (add_contunue > 1))
	{
		if(page_id == DELAY_SET)
		{
			if((cursor_id == 2)/* || (cursor_id == TINDEX_ZERO) || (cursor_id == TINDEX_TWO)*/)
			{
			//	if(temp > 1)return;
			//	temp = 2;
				set_addend_data(100, 10);
			}
		}
	}
	/*else if((add_contunue == 1))
	{
		if(page_id == DELAY_SET)
		{
			if((cursor_id == TINDEX_THREE) || (cursor_id == TINDEX_ZERO) || (cursor_id == TINDEX_TWO))
			{
				set_addend_data(100, 100);
			}
		}
	}*/
}

// init to pair ble device
void Key_To_Pair(void)
{
	if((PWR_KEY && SHUTTER))
	{
		time_delay_ms(500);
		if((PWR_KEY && SHUTTER))
		{
			Enter_into_Pair();
		}
	}
}

void set_power_on_int(uint8_t tatus)
{
	powers_off = tatus;
}

uint8_t return_powers(void)
{
	return powers_off;
}

void check_the_powers_status(void)
{
	if(CHARGE_STATUS == CHARGING)return;
	if(return_powers()) // stanby
	{
		ShutDown();
	}
}

void Pwr_key_continue(void)
{
	if(continues != 1)return;
	continues = 0;
	if(return_powers() == 0) // Hold down the button 1S to enter sleep
	{
		key_final_status = 0;
		keyPwr_status = 0;
		set_power_on_int(POWER_OFF);
		ShutDown();
		if(CHARGE_STATUS == CHARGING)
		{
			if(wifi_id.wakaup_tag != 0)
			{
				wifi_id.wakaup_tag = 0;
				if_write_flash();
				write_flash_active();
			}
		//	__disable_irq();
		//	NVIC_SystemReset();
			//wifi_id.wakaup_tag = 0;
			charge_tag = 1;
			LCD_Clear(BLACK);
			wait_for_init();
		}
	}
	else // Hold down the button 1S to wake up
	{
		set_power_on_int(POWER_ON);
		if(CHARGE_STATUS == CHARGING)
		{
			LCD_Clear(BLACK);
			battery_init();
			connection_flag = 0;
			charge_tag = 0;
			//wifi_id.wakaup_tag = 0;
			//wait_for_init();
		}
		else
		{
			//exti_init_beforEnterStop(0);
			__disable_irq();
			NVIC_SystemReset();
		}
	}
}



