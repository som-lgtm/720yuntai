#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_rtc.h"
#include "stm32f0xx_ll_exti.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_pwr.h"
#include "stm32f0xx_ll_gpio.h"
#include "gpio.h"

void rtc_interupt_init(void)
{
	//LL_EXTI_InitTypeDef EXTI_InitStructure;
	//EXTI_InitStructure.Line_0_31 = EXTI_Line17
}

void rtc_init(void)
{
	    LL_RTC_InitTypeDef *RTC_InitStructure;
		LL_RTC_AlarmTypeDef *RTC_AlarmStructure;
		LL_EXTI_InitTypeDef *EXTI_InitStructure;
		LL_RTC_TimeTypeDef *RTC_TimeStructure;
		
/*		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
		time_delay_ms(5);
		LL_PWR_EnableBkUpAccess();//Allow access to RTC
		time_delay_ms(5);
		//	Reset back up registers
		 LL_RCC_ForceBackupDomainReset();
		 LL_RCC_ReleaseBackupDomainReset();

		
		LL_RCC_LSI_Enable();
		LL_RCC_LSE_Disable();
	   // Wait till LSI is ready 
		while(LL_RCC_LSI_IsReady() != 1)
		{
		
		}
	LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
	
	LL_RCC_EnableRTC();
	time_delay_ms(5);
	LL_RTC_DeInit(RTC);

	RED_LED_ON;
	LL_RTC_WaitForSynchro(RTC);
	LED_ON;
*/

	
//	LL_RTC_DisableWriteProtection(RTC); // 解除RTC 的寄存器写保护
	//LL_RTC_SetAsynchPrescaler(RTC, 100);
	//LL_RTC_SetSynchPrescaler(RTC, 199);
	RTC_InitStructure->AsynchPrescaler = 199;
	RTC_InitStructure->SynchPrescaler = 199;
	RTC_InitStructure->HourFormat = LL_RTC_HOURFORMAT_24HOUR;
	LL_RTC_Init(RTC, RTC_InitStructure);
	
	RTC_TimeStructure->Hours = 1;
	RTC_TimeStructure->Minutes = 0;
	RTC_TimeStructure->Seconds = 0;
	RTC_TimeStructure->TimeFormat = LL_RTC_TIME_FORMAT_AM_OR_24;
	LL_RTC_TIME_Init(RTC, LL_RTC_FORMAT_BCD,RTC_TimeStructure);

//	LL_RTC_GetTime

	RTC_AlarmStructure->AlarmDateWeekDay = 0;
	RTC_AlarmStructure->AlarmMask = LL_RTC_ALMA_MASK_SECONDS;
	RTC_AlarmStructure->AlarmTime.TimeFormat = LL_RTC_TIME_FORMAT_AM_OR_24;
	RTC_AlarmStructure->AlarmTime.Hours = 0;
	RTC_AlarmStructure->AlarmTime.Minutes = 0;
	RTC_AlarmStructure->AlarmTime.Seconds = 10;

	LL_RTC_ALMA_Init(RTC, LL_RTC_FORMAT_BCD, RTC_AlarmStructure);
	LL_RTC_DisableWriteProtection(RTC); // 解除RTC 的寄存器写保护
	LL_RTC_ALMA_DisableWeekday(RTC);
	LL_RTC_EnableIT_ALRA(RTC);
	LL_RTC_ALMA_Enable(RTC);
	LL_RTC_EnableWriteProtection(RTC);
//	LL_RCC_EnableRTC();
//	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB,  LL_SYSCFG_EXTI_LINE7); // key

// EXTI SET
	EXTI_InitStructure->Line_0_31 = LL_EXTI_LINE_17;
	EXTI_InitStructure->LineCommand	= ENABLE;
	EXTI_InitStructure->Mode			= LL_EXTI_MODE_IT;
	EXTI_InitStructure->Trigger		= LL_EXTI_TRIGGER_RISING;
	LL_EXTI_Init(EXTI_InitStructure);	

	NVIC_SetPriority(RTC_IRQn, 0);
	NVIC_EnableIRQ(RTC_IRQn);
	 /* Wait till System clock is ready */
//	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
//	{
	
//	}
	 RED_LED_ON;
	 LED_ON;

}



