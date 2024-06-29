/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
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
#include "stm32f0xx.h"
#include "stm32f0xx_it.h"

/* USER CODE BEGIN 0 */
#include "common.h"
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN HardFault_IRQn 1 */

  /* USER CODE END HardFault_IRQn 1 */
}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles DMA1 channel 1 interrupt.
*/
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
	/* if(LL_TIM_IsActiveFlag_UPDATE(TIM1))
	 {
	 	 LL_TIM_ClearFlag_UPDATE(TIM1);
		 motorVV_ToUpdata_timer();
	 	 motorVV_pulse_count();
		// Vertical_pulse_goBack();
		 Vertical_pulse_check();
		 Vertical_slow_check();
		 Panorama_find_end_point_checkStop(MOTOR_Vertical);
		 //Panorama_find_end_point_slowChcek(MOTOR_Vertical);
		 Speialty_Vertical_pulse_check();
		 SpVVertical_slow_check();
		 video_motorVV_pulse_check();
		 Video_slow_pulse_check(MOTOR_Vertical);
		 Video_FindABpoint_pluse_check(MOTOR_Vertical);
		 Video_FindABpoint_slow_check(MOTOR_Vertical);
		Video_find_Origin_checkStop(MOTOR_Vertical);
		//Video_find_Origin_slow_check(MOTOR_Vertical);
		 Video_compensation_change_speed(MOTOR_Vertical);
		 Delay_find_Origin_checkStop(MOTOR_Vertical);
		// Delay_find_Origin_slow_check(MOTOR_Vertical);
		 Delay_motorVV_move_time_compara();
		 Delay_FindABpoint_pluse_check(MOTOR_Vertical);
		 Delay_FindABpoint_slow_check(MOTOR_Vertical);
		// Delay_slow_pulse_check(MOTOR_Vertical);
		ManulVV_mode_pulse_check();
	 }*/
}

void TIM3_IRQHandler(void)
{
	LL_TIM_ClearFlag_UPDATE(TIM3);
	motorHH_ToUpdata_timer();
	motorHH_pulse_count();
	//Horizontal_pulse_check();
	//Horizontal_slow_check();
	Specialty_loop_check();
	Video_loop_check();
	Timelapse_loop_check();
	ManulHH_mode_pulse_check();

}


/**
* @brief This function handles TIM14 global interrupt.
*/
void TIM14_IRQHandler(void)
{
  /* USER CODE BEGIN TIM14_IRQn 0 */

  /* USER CODE END TIM14_IRQn 0 */
  /* USER CODE BEGIN TIM14_IRQn 1 */
//  if(LL_TIM_IsActiveFlag_UPDATE(TIM14))
  {
	  LL_TIM_ClearFlag_UPDATE(TIM14);
	  adc_cout();
	  scan_cycle_count();
	  Gpio_time_count();
	  send_interval_count();
	  Pairing_time();
	  time_count();
	  standard_countdown_time_display();
	  ble_shut_time_count();
	  delay_mode_time_countdwon();
	  shutter_time_counts();
  }
//	  usart_timer_test();

  /* USER CODE END TIM14_IRQn 1 */
}

void TIM6_IRQHandler(void)
{
	LL_TIM_ClearFlag_UPDATE(TIM6);
	Green_Led_Brightness();
}


/**
* @brief This function handles USART2 global interrupt.
*/
void USART2_IRQHandler(void)
{
	  /* USER CODE BEGIN USART2_IRQn 0 */
	  if(LL_USART_IsActiveFlag_IDLE(USART2))
	  {
			LL_USART_ClearFlag_IDLE(USART2); // 
			usart22_read_byte();
	  }
	
  /* USER CODE END USART2_IRQn 0 */
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

void USART1_IRQHandler(void)
{
	  /* USER CODE BEGIN USART2_IRQn 0 */
	  if(LL_USART_IsActiveFlag_IDLE(USART1))
	  {
		 LL_USART_ClearFlag_IDLE(USART1);
		 usart1_receiver();
	  }
	
  /* USER CODE END USART2_IRQn 0 */
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

void DMA1_Channel4_5_IRQHandler(void)
{
	Usart22_Dma_SendIRQ_Dispose();
}

void DMA1_Channel2_3_IRQHandler(void)
{
	Dma_SendIRQ_Dispose();
}

void EXTI4_15_IRQHandler(void)
{
	//LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_10);
	//LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_11);
	if(LL_EXTI_ReadFlag_0_31(LL_EXTI_LINE_7) == LL_EXTI_LINE_7)
	{
		set_press_time();
	}
	
	LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_7);
}

void RTC_IRQHandler(void)
{
//	lights_flash();
	LL_RTC_ClearFlag_ALRA(RTC);
	LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_17);
	//rtc_wackup = 0xff;
	//charge_tag = 0;
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
