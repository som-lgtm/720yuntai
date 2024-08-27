/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void LL_Init(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM1_Init(void);
//static void MX_ADC_Init(void);
static void MX_IWDG_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM14_Init(void);
static void MX_TIM6_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART1_UART_Init(void);
static void MX_TIM3_Init(void);
void motor_init_stop(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
  
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/
  __enable_irq();

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_Init();

  SystemClock_Config();
  MX_GPIO_Init();  
  A_d_c_int();
  MX_TIM3_Init();
  MX_TIM6_Init();
MX_TIM14_Init();
//  MX_ADC_Init();
 
//  MX_RTC_Init();
  MX_USART2_UART_Init();  
	MX_USART1_UART_Init();
	check_uid();
//  MX_RTC_Init();
	  motor_init();
	Nrf24L01_init();
	Burn_code_value();
	//BLE_PWR_TurnOn();
	Ble_init();
//  MX_IWDG_Init();
	write_flash_active();
	usb_voltage_tatus();
	set_active_time_out(1800000); //30分钟
//MX_IWDG_Init();

  while (1)
  {
	//Feed_IWDG();
	keyscan();
		 read_ADC_value();
		  Angle_real_time_transmission(1);
	 	bluetooth_pair_led();
		set_abpoint_slow_stopORstart(MOTOR_HORITAL);
		Manual_HH_slowStartOrStop();
		DelayMode_slowly_startedORstop(MOTOR_HORITAL);
		DelayMode_FindABpoint_End();
		BLE_shut_stop();
		camera_shutter_shot(0);
		specialty_mode_main();
		Group_main();
		FindOrigin_over_to_Sart();
		Sync_data_for_controller();
		SpHHmotor_slowly_startedORstop();
		Video_MotorHH_slowly_startedORstop();
		Video_FindABpoint_End();
		limit_angle_360();
		delay_mode_main();
		wifi24G_data_Dispose();
	  Time_Out_And_Enter_Stop_Mode();
	 All_slow_check();
  }
  /* USER CODE END 3 */

}

static void LL_Init(void)
{
  

  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);

  /* System interrupt init*/
  /* SVC_IRQn interrupt configuration */
 // NVIC_SetPriority(SVC_IRQn, 0);
  /* PendSV_IRQn interrupt configuration */
 // NVIC_SetPriority(PendSV_IRQn, 0);
  /* SysTick_IRQn interrupt configuration */
//  NVIC_SetPriority(SysTick_IRQn, 0);

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

//*(uint32_t *)0x400210e0 |= 0x00060000;

  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
  {
  Error_Handler();	
  }
  
  LL_RCC_HSI_Enable();

  while(LL_RCC_HSI_IsReady() != 1)
  {
	
  }

 LL_RCC_HSE_Disable();

   /* Wait till HSE is ready */
// while(LL_RCC_HSE_IsReady() != 1)
//	{
	
//	}
  
 LL_RCC_HSI_SetCalibTrimming(16);

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
/*	LL_PWR_EnableBkUpAccess();
	
	  LL_RCC_ForceBackupDomainReset();
	
	  LL_RCC_ReleaseBackupDomainReset();

LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);

  LL_RCC_EnableRTC();*/

  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_12);

  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {
	
  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  
  }
	LL_Init1msTick(48000000);

	LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);

//	LL_SetSystemCoreClock(48000000);

//	LL_RCC_HSI14_EnableADCControl();

  /* SysTick_IRQn interrupt configuration */
//	NVIC_SetPriority(SysTick_IRQn, 0);
}



/* ADC init function */


// IWDG init function : LL_IWDG_PRESCALER_32*IWDG_RLR(1250) / 40KHz =1000mS
static void MX_IWDG_Init(void)
	{
	
	  LL_IWDG_Enable(IWDG);
	
	  LL_IWDG_EnableWriteAccess(IWDG);
	
	  LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_128);
	
	//	LL_IWDG_SetWindow(IWDG, 1250);
	
	  LL_IWDG_SetReloadCounter(IWDG, 1250);
	
	 while (LL_IWDG_IsReady(IWDG) != 1)
	 {
	  }
	
	  LL_IWDG_ReloadCounter(IWDG);
	//	LL_IWDG_Enable(IWDG);
	//LL_IWDG_DisableWriteAccess(IWDG);
	
	
	}


// Feed the dog
void Feed_IWDG(void)
{
	//LL_IWDG_ReloadCounter(IWDG);
}

/* TIM1 PWM init function */
static void MX_TIM1_Init(void)
{

  LL_TIM_InitTypeDef TIM_InitStruct;
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct;

  LL_GPIO_InitTypeDef GPIO_InitStruct;

  /* Peripheral clock enable */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_TIM1);

  /* TIM3 interrupt Init */
  NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 0);
  NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);

  TIM_InitStruct.Prescaler = 500;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 500;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.RepetitionCounter = 0;  //重复溢出多少次才给出一个溢出中断。必须有否则进不了中断。如果这个值不配置，上电时寄存器值是随机的。
  LL_TIM_Init(TIM1, &TIM_InitStruct);

//  LL_TIM_EnableARRPreload(TIM1);

//  LL_TIM_OC_EnablePreload(TIM1, LL_TIM_CHANNEL_CH4);

  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_ENABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 250;
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_LOW;
  LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH4, &TIM_OC_InitStruct);

//  LL_TIM_OC_DisableFast(TIM1, LL_TIM_CHANNEL_CH4);
  
  LL_TIM_OC_EnablePreload(TIM1, LL_TIM_CHANNEL_CH4);
  LL_TIM_EnableARRPreload(TIM1);
  LL_TIM_EnableAllOutputs(TIM1);
  
  LL_TIM_SetUpdateSource(TIM1, LL_TIM_UPDATESOURCE_COUNTER);
//	LL_TIM_EnableCounter(TIM3);
  LL_TIM_EnableIT_UPDATE(TIM1);
 // LL_TIM_SetTriggerOutput(TIM3, LL_TIM_TRGO_RESET);

 // LL_TIM_DisableMasterSlaveMode(TIM3);

  /**TIM3 GPIO Configuration  
  PB4	------> TIM3_CH1 
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
//	 LL_TIM_EnableCounter(TIM3);

}



/* TIM3 init function */
static void MX_TIM3_Init(void)
{

  LL_TIM_InitTypeDef TIM_InitStruct;
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct;

  LL_GPIO_InitTypeDef GPIO_InitStruct;

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

  /* TIM3 interrupt Init */
  NVIC_SetPriority(TIM3_IRQn, 0);
  NVIC_EnableIRQ(TIM3_IRQn);

  TIM_InitStruct.Prescaler = 500;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 500;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM3, &TIM_InitStruct);

  LL_TIM_EnableARRPreload(TIM3);

  LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH4);

  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM2;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_ENABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 250;
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_LOW;
  LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH4, &TIM_OC_InitStruct);

  LL_TIM_OC_DisableFast(TIM3, LL_TIM_CHANNEL_CH4);
  
  LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH4);
  LL_TIM_EnableARRPreload(TIM3);
  LL_TIM_EnableAllOutputs(TIM3);
  
  LL_TIM_SetUpdateSource(TIM3, LL_TIM_UPDATESOURCE_COUNTER);
//  LL_TIM_EnableCounter(TIM3);
  LL_TIM_EnableIT_UPDATE(TIM3);
 // LL_TIM_SetTriggerOutput(TIM3, LL_TIM_TRGO_RESET);

 // LL_TIM_DisableMasterSlaveMode(TIM3);

  /**TIM3 GPIO Configuration  
  PB4   ------> TIM3_CH1 
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
//   LL_TIM_EnableCounter(TIM3);

}


/* TIM14 init function */
static void MX_TIM14_Init(void)
{

  LL_TIM_InitTypeDef TIM_InitStruct;

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM14);

  /* TIM14 interrupt Init */
  NVIC_SetPriority(TIM14_IRQn, 1);
  NVIC_EnableIRQ(TIM14_IRQn);

  TIM_InitStruct.Prescaler = 23999;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 1;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM14, &TIM_InitStruct);
  LL_TIM_EnableCounter(TIM14);
//  LL_TIM_EnableARRPreload(TIM14);
//  LL_TIM_EnableUpdateEvent(TIM14);
LL_TIM_EnableIT_UPDATE(TIM14);


}


/* TIM16 init function */
static void MX_TIM6_Init(void)
{

  LL_TIM_InitTypeDef TIM_InitStruct;

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);

  /* TIM14 interrupt Init */
  NVIC_SetPriority(TIM6_IRQn, 2);
  NVIC_EnableIRQ(TIM6_IRQn);

  TIM_InitStruct.Prescaler = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 5999;//4799;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM6, &TIM_InitStruct);
  LL_TIM_EnableCounter(TIM6);
//  LL_TIM_EnableARRPreload(TIM14);
//  LL_TIM_EnableUpdateEvent(TIM14);
LL_TIM_EnableIT_UPDATE(TIM6);


}


/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* Init with LL driver */
  /* DMA controller clock enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
//  NVIC_SetPriority(DMA1_Channel1_IRQn, 0);
//  NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/* RTC init function */
static void MX_RTC_Init(void)
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


/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);


Device_init();
//motor_stop();


}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
