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
_app_interrupt_ser_t app_int_ser;

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/


/*******************************************************************************************************
* @function: NMI_Handler
* @brief:    NMI中断服务函数
*******************************************************************************************************/
void NMI_Handler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_NMI );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
/*******************************************************************************************************
* @function: HardFault_Handler
* @brief:    HardFault中断服务函数
*******************************************************************************************************/
void HardFault_Handler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_HARDFAULT );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
/*******************************************************************************************************
* @function: SVC_Handler
* @brief:    SVCall中断服务函数
*******************************************************************************************************/
void SVC_Handler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_SVCALL );
    app_int_ser();
  }
  else
  {
    // IAP中断服务程序
  }
}
/*******************************************************************************************************
* @function: PendSV_Handler
* @brief:    PendSV中断服务函数
*******************************************************************************************************/
void PendSV_Handler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_PENDSV );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
/*******************************************************************************************************
* @function: SysTick_Handler
* @brief:    SysTick中断服务函数
*******************************************************************************************************/
void SysTick_Handler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_SYSTICK );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  //  uwTick++;
  }
}
/*******************************************************************************************************
* @function: WWDG_IRQHandler
* @brief:    RTC中断服务函数
*******************************************************************************************************/
void WWDG_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_WWDG );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
#if (VTAB_SEL == VTAB031x)
/*******************************************************************************************************
* @function: PVD_IRQHandler
* @brief:    RTC中断服务函数
*******************************************************************************************************/
void PVD_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_PVD_VDDIO2 );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
#endif
/*******************************************************************************************************
* @function: RTC_IRQHandler
* @brief:    RTC中断服务函数
*******************************************************************************************************/
void RTC_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_RTC );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
/*******************************************************************************************************
* @function: FLASH_IRQHandler
* @brief:    中断服务函数
*******************************************************************************************************/
void FLASH_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_FLASH );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
/*******************************************************************************************************
* @function: RCC_IRQHandler
* @brief:    中断服务函数
*******************************************************************************************************/
void RCC_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_RCC );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
/*******************************************************************************************************
* @function: EXTI0_1_IRQHandler
* @brief:    中断服务函数
*******************************************************************************************************/
void EXTI0_1_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_EXTI0_1 );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
/*******************************************************************************************************
* @function: EXTI2_3_IRQHandler
* @brief:    中断服务函数
*******************************************************************************************************/
void EXTI2_3_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_EXTI2_3 );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
/*******************************************************************************************************
* @function: EXTI4_15_IRQHandler
* @brief:    中断服务函数
*******************************************************************************************************/
void EXTI4_15_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_EXTI4_15 );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
/*******************************************************************************************************
* @function: DMA1_Channel1_IRQHandler
* @brief:    中断服务函数
*******************************************************************************************************/
void DMA1_Channel1_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_DMA_CH1 );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
/*******************************************************************************************************
* @function: DMA1_Channel2_3_IRQHandler
* @brief:    中断服务函数
*******************************************************************************************************/
void DMA1_Channel2_3_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_DMA_CH2_3 );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
/*******************************************************************************************************
* @function: DMA1_Channel4_5_IRQHandler
* @brief:    中断服务函数
*******************************************************************************************************/
void DMA1_Channel4_5_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_DMA_CH4_5 );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
/*******************************************************************************************************
* @function: ADC1_IRQHandler
* @brief:    中断服务函数
*******************************************************************************************************/
void ADC1_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_ADC );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
/*******************************************************************************************************
* @function: TIM1_BRK_UP_TRG_COM_IRQHandler
* @brief:    中断服务函数
*******************************************************************************************************/
void TIM1_BRK_UP_TRG_COM_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_TIM1_BRK_UP_TRG_COM );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
/*******************************************************************************************************
* @function: TIM1_CC_IRQHandler
* @brief:    中断服务函数
*******************************************************************************************************/
void TIM1_CC_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_TIM1_CC );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
#if (VTAB_SEL == VTAB031x)
/*******************************************************************************************************
* @function: TIM2_IRQHandler
* @brief:    中断服务函数
*******************************************************************************************************/
void TIM2_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_TIM2 );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
#endif
/*******************************************************************************************************
* @function: TIM3_IRQHandler
* @brief:    中断服务函数
*******************************************************************************************************/
void TIM3_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_TIM3 );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
#if (VTAB_SEL == VTAB030x)
/*******************************************************************************************************
* @function: TIM6_IRQHandler
* @brief:    中断服务函数
*******************************************************************************************************/
void TIM6_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_TIM6 );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
#endif
/*******************************************************************************************************
* @function: TIM14_IRQHandler
* @brief:    中断服务函数
*******************************************************************************************************/
void TIM14_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_TIM14 );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
#if (VTAB_SEL == VTAB030x)
/*******************************************************************************************************
* @function: TIM15_IRQHandler
* @brief:    中断服务函数
*******************************************************************************************************/
void TIM15_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_TIM15 );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
#endif
/*******************************************************************************************************
* @function: TIM16_IRQHandler
* @brief:    中断服务函数
*******************************************************************************************************/
void TIM16_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_TIM16 );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
/*******************************************************************************************************
* @function: TIM17_IRQHandler
* @brief:    中断服务函数
*******************************************************************************************************/
void TIM17_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_TIM17 );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
/*******************************************************************************************************
* @function: I2C1_IRQHandler
* @brief:    中断服务函数
*******************************************************************************************************/
void I2C1_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_I2C1 );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
#if (VTAB_SEL == VTAB030x)
/*******************************************************************************************************
* @function: I2C2_IRQHandler
* @brief:    中断服务函数
*******************************************************************************************************/
void I2C2_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_I2C2 );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
#endif
/*******************************************************************************************************
* @function: SPI1_IRQHandler
* @brief:    中断服务函数
*******************************************************************************************************/
void SPI1_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_SPI1 );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
#if (VTAB_SEL == VTAB030x)
/*******************************************************************************************************
* @function: SPI2_IRQHandler
* @brief:    中断服务函数
*******************************************************************************************************/
void SPI2_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_SPI2 );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
#endif
/*******************************************************************************************************
* @function: USART1_IRQHandler
* @brief:    中断服务函数
*******************************************************************************************************/
void USART1_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_USART1 );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
    
  }
}
#if (VTAB_SEL == VTAB030x)
/*******************************************************************************************************
* @function: USART2_IRQHandler
* @brief:    中断服务函数
*******************************************************************************************************/
void USART2_IRQHandler( void )
{
  if( FROM_APP )
  {
    app_int_ser = APP_JUMP( APP_VECTOR_USART2 );
    app_int_ser();      // 跳转到APP中断复位程序
  }
  else
  {
    // IAP中断服务程序
  }
}
#endif

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/





/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
