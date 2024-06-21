

#include "stm32f0xx_ll_TIM.h"
#include "stm32f0xx_ll_gpio.h"
#include "common.h"
#include "GPIO.h"

uint8_t UpdataVV_time = 0;
MOTOR_PARA motorVV_p = {0};

//府轴电机的脉冲计数
void motorVV_pulse_count(void)
{
	if(motorVV_p.DIR)
	{
		if(motorVV_p.pulse_count > ORIGIN_POINT)motorVV_p.pulse_count--;
		else motorVV_p.pulse_count = B_POINT_END;
		
		if(mode_backup == VIDEO_MODE)if(motorVV_p.DVpulse_count)motorVV_p.DVpulse_count--;
		if(mode_backup == DELAY_SET)if(motorVV_p.DLLpulse_count)motorVV_p.DLLpulse_count--;
	}
	else
	{
		if(motorVV_p.pulse_count < B_POINT_END)motorVV_p.pulse_count++;
		else motorVV_p.pulse_count = ORIGIN_POINT;
		
		if(mode_backup == VIDEO_MODE)motorVV_p.DVpulse_count++;
		if(mode_backup == DELAY_SET)motorVV_p.DLLpulse_count++;
	}
}

void motorVV_direction_change(uint8_t dir)
{
	
	if(dir)
	{
		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_14); //left
	}
	else
	{
		LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_14); //right
	}
}

void motorVV_stop(void)
{
	//LL_TIM_DisableCounter(TIM1);
	//UpdataVV_time = 0;
	//MOTORVV_PND_OFF;
}

void motorVV_start(void)
{
	//MOTORVV_PND_ON;
	//time_delay_ms(50);
//	LL_TIM_EnableCounter(TIM1);
//	UpdataVV_time = 1;
}

void motorVV_speed_set(uint32_t speedd)
{
	LL_TIM_SetPrescaler(TIM1, speedd);
	if(UpdataVV_time == 0)LL_TIM_GenerateEvent_UPDATE(TIM1);
	if(UpdataVV_time == 1)UpdataVV_time = 2;
}

void motorVV_ToUpdata_timer(void)
{
	if(UpdataVV_time == 2)
	{
		UpdataVV_time = 1;
		LL_TIM_GenerateEvent_UPDATE(TIM1);
	}
}

