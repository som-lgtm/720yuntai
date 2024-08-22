
#include "stm32f0xx_ll_TIM.h"
#include "gpio.h"

uint8_t Updata_time = 0;
MOTOR_PARA motorHH_p = {0};

//水平电机的脉冲计数
void motorHH_pulse_count(void)
{
	if(motorHH_p.DIR)
	{
		if(motorHH_p.pulse_count > ORIGIN_POINT)motorHH_p.pulse_count--;
		else motorHH_p.pulse_count = B_POINT_END;
		if(mode_backup == VIDEO_MODE)
		{
			if(motorHH_p.DVpulse_count)motorHH_p.DVpulse_count--;
			//if(motorHH_p.DVpulse_count > video_p.locusH.point_pulse_a)motorHH_p.DVpulse_count--;
			//else motorHH_p.DVpulse_count = video_p.locusH.point_pulse_b;
		}
		if(mode_backup == DELAY_SET)
		{
			if(motorHH_p.DLLpulse_count)motorHH_p.DLLpulse_count--;
		}

		if(mode_backup == GROUP_PHOTO)
		{
			if(motorHH_p.GPpulse_count)motorHH_p.GPpulse_count--;
		}
	}
	else
	{
		if(motorHH_p.pulse_count < (B_POINT_END))motorHH_p.pulse_count++;
		else motorHH_p.pulse_count = ORIGIN_POINT;
		
		if(mode_backup == VIDEO_MODE)
		{
			motorHH_p.DVpulse_count++;
			//if(motorHH_p.DVpulse_count < video_p.locusH.point_pulse_b)motorHH_p.DVpulse_count++;
			//else motorHH_p.DVpulse_count = video_p.locusH.point_pulse_a;
		}
		if(mode_backup == DELAY_SET)motorHH_p.DLLpulse_count++;
		
		if(mode_backup == GROUP_PHOTO)
		{
			motorHH_p.GPpulse_count++;
		}
	}
}

void motorHH_direction_change(uint8_t dir)
{
	if(dir==0)
	{
		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_0); //left
	}
	else
	{
		LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0); //right
	}
}

void motorHH_stop(void)
{
	LL_TIM_DisableCounter(TIM3);
	Updata_time = 0;
	MOTOR_PND_OFF;
}

void motorHH_start(void)
{
	MOTOR_PND_ON;
	//time_delay_ms(50);
	LL_TIM_EnableCounter(TIM3);
	Updata_time = 1;
}

void motorHH_speed_set(uint32_t speedd)
{
	LL_TIM_SetPrescaler(TIM3, speedd);
	if(Updata_time == 0)LL_TIM_GenerateEvent_UPDATE(TIM3);
	if(Updata_time == 1)Updata_time = 2;
}

void motorHH_ToUpdata_timer(void)
{
	if(Updata_time == 2)
	{
		Updata_time = 1;
		LL_TIM_GenerateEvent_UPDATE(TIM3);
	}
}



