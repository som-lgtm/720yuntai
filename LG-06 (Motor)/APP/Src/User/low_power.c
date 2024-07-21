
#include "gpio.h"
#include "rf24l01.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_hal_pwr.h"
#include "stm32f0xx_ll_adc.h"
#include "stm32f0xx_ll_dma.h"
#include "stm32f0xx_ll_tim.h"
#include "stm32f0xx_ll_rcc.h"
#include "low_power.h"
#include "stm32f0xx_ll_pwr.h"
#include "stm32f0xx_ll_rtc.h"
#include "stm32f0xx_ll_exti.h"
#include "stm32f0xx_ll_usart.h"
uint32_t active_time_out= 0; 
uint8_t lowbattery_dis_if = 0;
uint8_t charge_tag; // 当插电充电时至1
uint8_t rtc_wackup=0; //

void Reset_theSystem(void)
{

/*	LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
  {
  Error_Handler();	
  }
  */
 /* LL_RCC_HSI_Enable();

   // Wait till HSI is ready 
  while(LL_RCC_HSI_IsReady() != 1)
  {
	
  }*/
  LL_RCC_HSE_Disable();
  LL_RCC_PLL_Disable();
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
////  LL_RCC_HSI_SetCalibTrimming(16);

 //// LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_2);

 /// LL_RCC_PLL_Enable();

   // Wait till PLL is ready /
////  while(LL_RCC_PLL_IsReady() != 1)
////  {
	
 //// }
}


void set_active_time_out(uint32_t aaa)
{
//	active_time_out = wifi_id.sleep_time*60*1000;
	active_time_out = aaa;
}

uint32_t return_active_time_out(void)
{
	return active_time_out;
}

void LowBattery_wakeup(void)
{
//	set_active_time_out();

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

void ShutDown(void)
{
	wifi_sleep();
	disable_interrupt();
		ADC_shutdown();
		shutdown_motor_pwr();
		RED_LED_OFF;
		GREEN_LED_OFF;
		SHUTTER_OFF;
		BLE_PWR_DOWN;
		A3_3_PWR_DOWN;
		shutter_processing(0);
	//	Nixie_all_Off();
	//	lowbattery_dis_if = 1;
		ShutDown_AllIO_High_resistance_mode();
	//MX_RTC_Init();
//	set_power_on_int(0XFF);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	pwrKey_exti_interrupt();
//	RTC_AlarmConfig();
//	SysTick->CTRL  = 0; //DISABLE SYSYTICK
	Reset_theSystem();
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
//	RTC_disable();
	if(charge_tag)
	{
		//LED22_ON;
		__disable_irq();
		NVIC_SystemReset();
	}
	SystemClock_Config();
	enable_interrupt();
//	Led_init();
//	key_gpio_init();
//	LED_ON;
//	LED11_ON;
	
	//LL_RCC_DisableRTC();
}


void active_time_out_countdown(void)
{
	if(active_time_out)active_time_out--;
}

void Time_Out_And_Enter_Stop_Mode(void)
{
	if(m_start || sp_start || video_p.Pause || delay_p.m_start)
	{
		set_active_time_out(1800000); //30分钟
		return;
	}
	
	if(return_active_time_out())return;
	set_active_time_out(1800000); //30分钟
	
	ShutDown();
}



void disable_interrupt(void)
{
	LL_TIM_DisableCounter(TIM3);
	LL_TIM_ClearFlag_UPDATE(TIM3);
	NVIC_DisableIRQ(TIM3_IRQn);
	LL_TIM_ClearFlag_UPDATE(TIM3);
	
	LL_TIM_DisableCounter(TIM14);
	LL_TIM_ClearFlag_UPDATE(TIM14);
	NVIC_DisableIRQ(TIM14_IRQn);
	LL_TIM_ClearFlag_UPDATE(TIM14);
	
	LL_TIM_DisableCounter(TIM16);
	LL_TIM_ClearFlag_UPDATE(TIM16);
	NVIC_DisableIRQ(TIM16_IRQn);
	LL_TIM_ClearFlag_UPDATE(TIM16);
	
	LL_USART_Disable(USART2);
	LL_USART_DisableDMAReq_RX(USART2);
	LL_USART_DisableDMAReq_TX(USART2);
	LL_USART_DisableIT_IDLE(USART2);
	LL_USART_ClearFlag_IDLE(USART2); // 
	NVIC_DisableIRQ(USART2_IRQn);
	NVIC_DisableIRQ(DMA1_Channel4_5_IRQn);
	LL_USART_ClearFlag_IDLE(USART2); // 
	Dma_SendIRQ_Dispose();
	
	LL_USART_Disable(USART1);
	LL_USART_DisableDMAReq_RX(USART1);
	LL_USART_DisableDMAReq_TX(USART1);
	LL_USART_DisableIT_IDLE(USART1);
	LL_USART_ClearFlag_IDLE(USART1); // 
	NVIC_DisableIRQ(USART1_IRQn);
	NVIC_DisableIRQ(DMA1_Channel2_3_IRQn);
	LL_USART_ClearFlag_IDLE(USART1); // 
	//Dma_SendIRQ_Dispose();
	LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_7);
	LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_7);
	NVIC_DisableIRQ(EXTI4_15_IRQn);

}

static uint8_t RTC_Bcd2ToByte(uint8_t Value)
{
  uint8_t tmp = 0;
  tmp = ((uint8_t)(Value & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;
  return (tmp + (Value & (uint8_t)0x0F));
}

void RTC_GetTime(uint32_t RTC_Format, LL_RTC_TimeTypeDef* RTC_TimeStruct)
{
//  uint32_t tmpreg = 0;

  /* Get the RTC_TR register */
//  tmpreg = (uint32_t)(RTC->TR); 
  
  /* Fill the structure fields with the read parameters */
  //RTC_TimeStruct->Hours = (uint8_t)((tmpreg & (RTC_TR_HT | RTC_TR_HU)) >> 16);
  //RTC_TimeStruct->Minutes = (uint8_t)((tmpreg & (RTC_TR_MNT | RTC_TR_MNU)) >>8);
 // RTC_TimeStruct->Seconds = (uint8_t)(tmpreg & (RTC_TR_ST | RTC_TR_SU));
 // RTC_TimeStruct->TimeFormat = (uint8_t)((tmpreg & (RTC_TR_PM)) >> 16);  
  
  RTC_TimeStruct->Hours = LL_RTC_TIME_GetHour(RTC);;
  RTC_TimeStruct->Minutes = LL_RTC_TIME_GetMinute(RTC);
  RTC_TimeStruct->Seconds = LL_RTC_TIME_GetSecond(RTC);
  RTC_TimeStruct->TimeFormat = LL_RTC_TIME_GetFormat(RTC);  

  /* Check the input parameters format */
  if (RTC_Format == LL_RTC_FORMAT_BIN)
  {
    /* Convert the structure parameters to Binary format */
    RTC_TimeStruct->Hours = (uint8_t)RTC_Bcd2ToByte(RTC_TimeStruct->Hours);
    RTC_TimeStruct->Minutes = (uint8_t)RTC_Bcd2ToByte(RTC_TimeStruct->Minutes);
    RTC_TimeStruct->Seconds = (uint8_t)RTC_Bcd2ToByte(RTC_TimeStruct->Seconds);
  }
}

void RTC_AlarmConfig(void)
{
	LL_RTC_TimeTypeDef   RTC_TimeStructure;
	LL_RTC_AlarmTypeDef  RTC_AlarmStructure;
//	LL_RTC_InitTypeDef RTC_InitStruct;
	
/*	RTC_InitStruct.HourFormat = LL_RTC_HOURFORMAT_24HOUR;
	RTC_InitStruct.AsynchPrescaler = 127;
	RTC_InitStruct.SynchPrescaler = 255;
	LL_RTC_Init(RTC, &RTC_InitStruct);
	*/
	RTC_TimeStructure.TimeFormat = LL_RTC_ALMA_TIME_FORMAT_AM;
	RTC_TimeStructure.Hours   = 0x00;
	RTC_TimeStructure.Minutes = 0x00;
	RTC_TimeStructure.Seconds = 0x00;  
	
	LL_RTC_TIME_Init(RTC, LL_RTC_FORMAT_BIN, &RTC_TimeStructure);
	//LL_RCC_EnableRTC();

	RTC_GetTime(LL_RTC_FORMAT_BIN, &RTC_TimeStructure);
	
	/* Set the alarm to current time + 10s */
	RTC_AlarmStructure.AlarmTime.TimeFormat	 = LL_RTC_ALMA_TIME_FORMAT_AM;
	RTC_AlarmStructure.AlarmTime.Hours	 = RTC_TimeStructure.Hours;
	RTC_AlarmStructure.AlarmTime.Minutes = RTC_TimeStructure.Minutes;
	RTC_AlarmStructure.AlarmTime.Seconds = RTC_TimeStructure.Seconds + 2;
	RTC_AlarmStructure.AlarmDateWeekDay = 31;
	RTC_AlarmStructure.AlarmDateWeekDaySel = LL_RTC_ALMA_DATEWEEKDAYSEL_DATE;
	RTC_AlarmStructure.AlarmMask =LL_RTC_ALMA_MASK_DATEWEEKDAY | LL_RTC_ALMA_MASK_HOURS |
									   LL_RTC_ALMA_MASK_MINUTES;
	LL_RTC_ALMA_Init(RTC, LL_RTC_FORMAT_BIN,  &RTC_AlarmStructure);
	LL_RTC_ClearFlag_ALRA(RTC);
	LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_17);
	LL_RTC_DisableWriteProtection(RTC);
	LL_RTC_EnableIT_ALRA(RTC);
	LL_RTC_ALMA_Enable(RTC);
	LL_RTC_ClearFlag_ALRA(RTC);
	LL_RTC_EnableWriteProtection(RTC);
	//LL_RCC_EnableRTC();
}

void RTC_disable(void)
{
	LL_RTC_DisableWriteProtection(RTC);
	LL_RTC_DisableIT_ALRA(RTC);
	LL_RTC_ALMA_Disable(RTC);
	LL_RTC_EnableWriteProtection(RTC);
}

void enable_interrupt(void)
{
	//if(rtc_wackup)
	//{
	//	LL_Init1msTick(48000000);
	//}

	LL_TIM_EnableCounter(TIM14);
	NVIC_EnableIRQ(TIM14_IRQn);
}


