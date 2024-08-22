
#include "main.h"

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
	RTC_AlarmStructure.AlarmTime.Seconds = RTC_TimeStructure.Seconds + 2; // 2秒醒一次
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

void RTC_clock_init(void)
{
	 LL_RCC_HSI14_Enable();
	
	/* Wait till HSI14 is ready */
	 while(LL_RCC_HSI14_IsReady() != 1)
	 {
	 
	 }
	 LL_RCC_HSI14_SetCalibTrimming(16);
	
	 LL_RCC_LSI_Enable();
	
	/* Wait till LSI is ready */
	 while(LL_RCC_LSI_IsReady() != 1)
	 {
	 
	 }
	 
	 LL_Init1msTick(48000000);
	 
	 LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
}

/* RTC init function */
void MX_RTC_Init(void)
{

  LL_RTC_InitTypeDef RTC_InitStruct;
  LL_RTC_TimeTypeDef	  RTC_TimeStructure;
  LL_EXTI_InitTypeDef	EXTI_InitStructure;

  
  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_PWR_EnableBkUpAccess();
  LL_RCC_ForceBackupDomainReset();
  
  LL_RCC_ReleaseBackupDomainReset();
	  
  
  LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
	LL_RCC_EnableRTC();
	LL_RTC_WaitForSynchro(RTC);  
  /* RTC interrupt Init */
  NVIC_SetPriority(RTC_IRQn, 0);
  NVIC_EnableIRQ(RTC_IRQn);


	/**Initialize RTC and set the Time and Date 
	秒输出计算：Tsec=LSI/[(RTC_AsynchPrediv+1)*(RTC_SynchPrediv +1)]*/
  RTC_InitStruct.HourFormat = LL_RTC_HOURFORMAT_24HOUR;
  RTC_InitStruct.AsynchPrescaler = 127;
  RTC_InitStruct.SynchPrescaler = 311; //255;
  LL_RTC_Init(RTC, &RTC_InitStruct);

	RTC_TimeStructure.TimeFormat = LL_RTC_ALMA_TIME_FORMAT_AM;
	RTC_TimeStructure.Hours   = 0x00;
	RTC_TimeStructure.Minutes = 0x00;
	RTC_TimeStructure.Seconds = 0x00;  

	LL_RTC_TIME_Init(RTC, LL_RTC_FORMAT_BIN, &RTC_TimeStructure);

	/* Configure EXTI line 17 (connected to the RTC Alarm event) */
	LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_17);
	EXTI_InitStructure.Line_0_31 = LL_EXTI_LINE_17;
	EXTI_InitStructure.Mode = LL_EXTI_MODE_IT;
	EXTI_InitStructure.Trigger = LL_EXTI_TRIGGER_RISING;
	EXTI_InitStructure.LineCommand = ENABLE;
	LL_EXTI_Init(&EXTI_InitStructure);
	
}

