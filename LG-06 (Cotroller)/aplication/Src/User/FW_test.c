
#include "stm32f0xx_ll_gpio.h"
#include "motor_dri.h"
#include "common.h"


extern uint8_t ab_set_tag;


void FW_test_main(void)
{
	SHUTTER_ON;
//	LED_ON;
	time_delay_ms(500);
	SHUTTER_OFF;
//	LED_OFF;
	time_delay_ms(500);
	SHUTTER_ON;
//	LED_ON;
	time_delay_ms(500);
	SHUTTER_OFF;
//	LED_OFF;

	ab_set_tag = 2;
	glob_para.pulse_cout = 0;
	glob_para.b_point = 1000;
	glob_para.a_point = 0;
	motor_speed_adjust(50);
	glob_para.pulse_cout = A_TO_B;
	motor_direction_change();
	motor_start();
	while(1)
	{
		
	}
}

void enter_test_mode(void)
{
	 LL_GPIO_InitTypeDef GPIO_InitStruct;
	// GPIO INPUT INIT
	GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	// GPIO INPUT INIT
	GPIO_InitStruct.Pin = LL_GPIO_PIN_12;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	time_delay_ms(1);

	if(!LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_11) && !LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_12))
	{
		time_delay_ms(1000);
		
		if(!LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_11) && !LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_12))
		{
			FW_test_main();
		}
	}
}

