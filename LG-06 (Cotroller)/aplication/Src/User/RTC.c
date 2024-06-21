

void rtc_clock_init(void)
{
	LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
	
	LL_RCC_EnableRTC();

}

/* RTC init function */
static void MX_RTC_Init(void)
{

  LL_RTC_InitTypeDef RTC_InitStruct;

  //Peripheral clock enable 
//  LL_RCC_EnableRTC();

    /**Initialize RTC and set the Time and Date 
    */
  RTC_InitStruct.HourFormat = LL_RTC_HOURFORMAT_24HOUR;
  RTC_InitStruct.AsynchPrescaler = 127;
  RTC_InitStruct.SynchPrescaler = 255;
  LL_RTC_Init(RTC, &RTC_InitStruct);

  LL_RTC_SetAsynchPrescaler(RTC, 127);

  LL_RTC_SetSynchPrescaler(RTC, 255);

}

void Nvic_rtc_init(void)
{
	NVIC_SetPriority(RTC_IRQn, 0);
	NVIC_EnableIRQ(RTC_IRQn);
}

void rtc_seting(void)
{
	LL_EXTI_InitTypeDef exit_init;
	LL_RTC_AlarmTypeDef Rtc_alarm_struct;
	
	LL_AHB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	rtc_clock_init();
	MX_RTC_Init();
	//LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);

	exit_init.Line_0_31 = LL_EXTI_LINE_17;
	exit_init.Mode = LL_EXTI_MODE_IT;
	exit_init.Trigger = LL_EXTI_TRIGGER_RISING;
	exit_init.LineCommand = ENABLE;
	LL_EXTI_Init(exit_init);

	Nvic_rtc_init();

	Rtc_alarm_struct.AlarmDateWeekDay = 31;
	//Rtc_alarm_struct.AlarmDateWeekDaySel = 
	//Rtc_alarm_struct.AlarmMask = 
}
