#include "stm32f030x8.h"
#include "keyscan.h"
#include "uart.h"
#include "gpio.h"
#include "common.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_hal_pwr.h"

//#define KeyUp		LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_4)//
//#define KeyDown		LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_3)//ADCIN4
//#define KeyLeft		LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_1)//ADCIN1
//#define KeyRight	LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_2)//ADCIN0
//#define KeyOk		LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_0)//ADCIN2
//#define Bootooth_disconnect		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)//ADCIN2
//#define RETURN_KEY1	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10)
//#define RETURN_KEY2	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)

uint16_t scan_cycle=0;
uint8_t key_final_status=0;

uint8_t keypowr_status=0;
uint16_t continues = 0;
uint16_t press_time = 0;
uint8_t powers_off = 0;
uint8_t Pwrkey_debounce = 0;

uint8_t key_debounce = 0;

void set_press_time(void)
{
	press_time = 3000;
}

unsigned char key_Handle(void)
{
	unsigned char p_keyup=0;
	
	p_keyup = ~KeyPowr;
	return (p_keyup);
	
}

void keyscan(void)
{
	uint8_t key_status=0;
//	static uint8_t bt_flag_back=0xff;
	
	press_key_continue();
	if(press_time == 1)
	{
		press_time = 0;
		check_the_powers_status();
	}

	if(scan_cycle)
		return;
	scan_cycle = 4;//20;
	
	key_status = key_Handle();
		
		if((key_status & KEY_POWR_MASK)==0)
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
		if(key_debounce < 3)return;

		key_debounce = 0;
		key_final_status = key_status;
		
		if((key_final_status & KEY_POWR_MASK) != keypowr_status)
		{
			keypowr_status = key_final_status & KEY_POWR_MASK;
			if(keypowr_status)
			{
				continues = 2001;
				//Ble_init();
			}
			else
			{
				continues = 0;
				press_time = 0;
				check_the_powers_status(); // Continue standby for less than 1S
				Specialty_Key_start();
			}
			//key_event_processing(keypowr_status);
		}

//////////////////////////////////////////////////////////////////	
	}
	
}


void scan_cycle_count(void)
{
	if(scan_cycle)scan_cycle--;
	if(continues>1)continues--;
	if(press_time>1)press_time--;
}

void set_power_on_int(uint8_t tatus)
{
	powers_off = tatus;
}

uint8_t return_powers(void)
{
	return powers_off;
}


void Enter_Sleep_Mode(void)
{
	//system_clock_init(RCC_SYSCLK_Div1);
	exti_init_beforEnterStop(1);
	keypowr_status = 0;
	key_final_status = 0;
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	//LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_PWR);
	SystemClock_Config();
	exti_init_beforEnterStop(0);		
//	__disable_irq();
//		NVIC_SystemReset();
}

void check_the_powers_status(void)
{
	//if(CHARGE_STATUS == 0)return;
	if(powers_off) // stanby
	{
		ShutDown();
	}
}

void press_key_continue(void)
{
	if(continues != 1)return;
	continues = 0;
	if(powers_off == 0) // Hold down the button 1S to enter sleep
	{
		//if(CHARGE_STATUS == 0)return;
		set_power_on_int(POWER_OFF);
		key_final_status = 0;
		keypowr_status = 0;
		ShutDown();
	}
	else // Hold down the button 1S to wake up
	{
		set_power_on_int(POWER_ON);
		//exti_init_beforEnterStop(0);
		__disable_irq();
		NVIC_SystemReset();
	}
}

void enter_test_mode(void)
{	
/*	uint32_t ptime = 0;
	LED_ON;
	if(key_Handle()&KEY_POWR_MASK)
	{
		//scan_cycle = 4;
		for(ptime=0;ptime <8000000;ptime++)
		{}
		if(key_Handle()&KEY_POWR_MASK)
		{
			FW_test_main();
		}
	}*/
}

