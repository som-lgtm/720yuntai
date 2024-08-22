
#include "gpio.h"
#include "config.h"
#include "orbital.h"
#include "rf24l01.h"
#include "oled.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_hal_pwr.h"
#include "stm32f0xx_ll_adc.h"
#include "stm32f0xx_ll_dma.h"
#include "stm32f0xx_ll_tim.h"
#include "stm32f0xx_ll_rcc.h"
#include "low_power.h"
#include "keyscan.h"
#include "stm32f0xx_ll_usart.h"
#include "stm32f0xx_ll_exti.h"
__IO uint32_t active_time_out= 0; 
uint8_t lowbattery_dis_if = 0;
uint8_t charge_tag=0; // 当插电充电时至1
uint8_t rtc_wackup=0; //

void set_rtc_wackup(uint8_t sets)
{
	rtc_wackup = sets;
}

void Reset_theSystem(uint8_t types)
{
	if(types)
	{
	  LL_RCC_HSE_Disable();
	  LL_RCC_PLL_Disable();
	  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
	}
	else
	{
		LL_RCC_PLL_Enable();
		
		 // Wait till PLL is ready /
		while(LL_RCC_PLL_IsReady() != 1)
		{
		}
		LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
		while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
		{
		
		}
	}
}


void set_active_time_out(void)
{
	active_time_out = wifi_id.sleep_time*60*1000;
	//active_time_out = 10000;
}

uint32_t return_active_time_out(void)
{
	return active_time_out;
}

void LowBattery_wakeup(void)
{
	set_active_time_out();

	if(lowbattery_dis_if)
	{
		lowbattery_dis_if = 0;
		//lowbattery_dis = 60;
		//LCD_Fill(0);  //初始清屏
		
		//OLED_BLK_Set();
		/*change_page();
		if(page_id == DELAY_DIS)
		{
			delay_dis_interface();
		}
		cursor_shift(0);*/
	}
}

void ADC_shutdown(void)
{
	if(LL_ADC_REG_IsConversionOngoing(ADC1) == ADC_CR_ADSTART)
	{
		LL_ADC_REG_StopConversion(ADC1);
	}
	while(LL_ADC_REG_IsStopConversionOngoing(ADC1))
	{}
	LL_APB1_GRP2_ForceReset(1<<9);

	LL_ADC_Disable(ADC1);	
	while(LL_ADC_IsEnabled(ADC1))
	{
	//LL_ADC_Disable(ADC1);
	}	
	while(LL_ADC_REG_IsConversionOngoing(ADC1))
	{
	//LL_ADC_Disable(ADC1);
	}
	LL_APB1_GRP2_ForceReset(1<<9);
}
void Forbidden_Used(void)
{
	disable_interrupt();
	OLED_PWR_OFF;
	CH340N_PWR_OFF;
	BOOST_PWR_OFF;
	continues = 0;
	lowbattery_dis_if = 1;
	ADC_shutdown();
	//LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_NONE);
//	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
//	LL_APB1_GRP2_DisableClock(LL_APB1_GRP2_PERIPH_ADC1);
	wifi_sleep();
	ShutDown_AllIO_High_resistance_mode();
	ChangeKey_exti_interrupt();
	if(wifi_id.wakaup_tag != 0)
	{
		wifi_id.wakaup_tag = 0;
		if_write_flash();
		write_flash_active();
	}
	Reset_theSystem(1);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	
	__disable_irq();
	NVIC_SystemReset();
}

void ShutDown(void)
{
	if(CHARGE_STATUS == CHARGING)return;
	disable_interrupt();
	OLED_PWR_OFF;
	CH340N_PWR_OFF;
	BOOST_PWR_OFF;
	continues = 0;
	lowbattery_dis_if = 1;
	if(rtc_wackup==0)
	{
		ADC_shutdown();
	}
	wifi_sleep();
	if(wifi_id.wakaup_tag != 0)
	{
		wifi_id.wakaup_tag = 0;
		if_write_flash();
		write_flash_active();
	}
	set_power_on_int(0XFF);
	ShutDown_AllIO_High_resistance_mode();
	rtc_wackup = 0;
	RTC_AlarmConfig();
	SysTick->CTRL  = 0; //DISABLE SYSYTICK
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	pwrKey_exti_interrupt();
	Reset_theSystem(1);
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	Reset_theSystem(0);
//	SystemClock_Config();
	if(charge_tag)
	{
		__disable_irq();
		NVIC_SystemReset();
	}
	//SystemClock_Config();
	enable_interrupt();
}

void Enter_Sleep_Mode(void)
{
/*	//LCD_WR_REG(0x28); //display off
	LCD_WR_REG(0x10); //sleep in 
	OLED_PWR_OFF;	
//	LL_TIM_OC_SetCompareCH4(TIM3, 0);
	CH340N_PWR_OFF;
	BOOST_PWR_OFF;
	lowbattery_dis_if = 1;
	LL_ADC_REG_StopConversion(ADC1);
	LL_ADC_Disable(ADC1);
	LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_NONE);
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
	LL_APB1_GRP2_DisableClock(LL_APB1_GRP2_PERIPH_ADC1);
	wifi_sleep();
	key_interrupt(1);
	IOHigh_mode();
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
*/
	
	disable_interrupt();
	OLED_PWR_OFF;
	CH340N_PWR_OFF;
	BOOST_PWR_OFF;
	lowbattery_dis_if = 1;
	adj_parm_sel = 0;
	ADC_shutdown();
	wifi_sleep();
//	pwrKey_exti_interrupt();
//	ShutDown_AllIO_High_resistance_mode();
	key_interrupt(1);
	IOHigh_mode();
	Reset_theSystem(1);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

	if(wifi_id.wakaup_tag != 1)
	{
		wifi_id.wakaup_tag = 1;
		if_write_flash();
		write_flash_active();
	}
	
	__disable_irq();
	NVIC_SystemReset();
/*	SystemClock_Config();
	time_delay_ms(500);
	IO_recover();
	Lcd_Init();
	battery_init();
	RF24L01_Init();
	RF24L01_Set_Mode( MODE_RX );
	RF24L01_CE_ON;
	connection_flag = 0;//check wifi conection
	MX_TIM3_Init();
	Adc_Gpio_init();
	ADC->CCR = 1<<22;
	LL_APB1_GRP2_ForceReset(1<<9);
	LL_ADC_Enable(ADC1);
	while(!LL_ADC_IsActiveFlag_ADRDY(ADC1)); 
	LL_ADC_REG_StartConversion(ADC1);*/
}

// the mcu enter stop mode while any key not press after ten minutes; 
// but Pressing any key within 10 minutes will not allow entry stop mode.
void Time_Out_And_Enter_Stop_Mode(void)  
{
	/*if(return_active_time_out())return;
	set_active_time_out();
	if(CHARGE_STATUS == 0)return; // 
	*/
	
	if(CHARGE_STATUS == CHARGING)
	{
		if(return_active_time_out())return;
		set_active_time_out();
		return;
	}
	
	if(rtc_wackup)
	{
		//if(return_active_time_out())return;
		ShutDown();
	}
	else
	{
		if(return_active_time_out())return;
		set_active_time_out();
		ShutDown();
	}

	
}


void active_time_out_countdown(void)
{
	if(active_time_out)active_time_out--;
}


void disable_interrupt(void)
{
	
	LL_TIM_DisableCounter(TIM3);
	LL_TIM_ClearFlag_UPDATE(TIM3);
	LL_TIM_ClearFlag_UPDATE(TIM3);
	NVIC_DisableIRQ(TIM3_IRQn);
	
	LL_TIM_DisableCounter(TIM14);
	LL_TIM_ClearFlag_UPDATE(TIM14);
	NVIC_DisableIRQ(TIM14_IRQn);
	LL_TIM_ClearFlag_UPDATE(TIM14);
	
	NVIC_DisableIRQ(DMA1_Channel4_5_IRQn);
	NVIC_DisableIRQ(USART2_IRQn);
	Dma_SendIRQ_Dispose();
	LL_USART_Disable(USART2);
	LL_USART_DisableDMAReq_RX(USART2);
	LL_USART_DisableDMAReq_TX(USART2);
	LL_USART_DisableIT_IDLE(USART2);
	LL_USART_ClearFlag_IDLE(USART2); // 

	
//	LL_USART_Disable(USART1);
//	NVIC_DisableIRQ(USART1_IRQn);
//	NVIC_DisableIRQ(DMA1_Channel2_3_IRQn);
//	LL_USART_ClearFlag_IDLE(USART1); // 
//	Usart22_Dma_SendIRQ_Dispose();

	LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_15|LL_EXTI_LINE_11|LL_EXTI_LINE_12|LL_EXTI_LINE_3|LL_EXTI_LINE_4|LL_EXTI_LINE_5|LL_EXTI_LINE_6|LL_EXTI_LINE_7|LL_EXTI_LINE_8);
	LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_15|LL_EXTI_LINE_8);
	NVIC_DisableIRQ(EXTI4_15_IRQn);
	NVIC_DisableIRQ(EXTI2_3_IRQn);

}

void enable_interrupt(void)
{

	LL_TIM_EnableCounter(TIM14);
	NVIC_EnableIRQ(TIM14_IRQn);
}

