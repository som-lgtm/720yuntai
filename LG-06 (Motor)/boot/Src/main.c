/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 05/21/2007
* Description        : The main program
********************************************************************************
* History:
* 05/21/2007: V0.1 
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "ymodem.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void Usart11_Init(void);
static void Usart22_Init(void);
uint8_t select_usart = 0; // 0xaa -->Usart 1; 0xbb -->Usart 2
uint8_t force_updata = 0;

uint8_t check_ID(void)
{
	IAP_INF_T infor={0};
	uint8_t device_id[12]={0};
	uint8_t i = 0;
//	uint32_t address = FLASH_WRITE_START_ADDR;
	
	read_flash_holfword(FLASH_WRITE_START_ADDR, (uint16_t *)&infor, sizeof(infor));
	select_usart = infor.usart_number;
	if(infor.first_used != 0x70)
	{
		//memcpy(&infor.MCU_uid[0], device_id, 12); // load data to DMA send buffer
		//infor.Reserved = 0x06;
		infor.first_used = 0x70;
		write_flash_holfword_buffer((uint16_t *)&infor,sizeof(infor));
	}

	return 1;
}

void SystemClock_Config(void)
{

  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
  {
//  Error_Handler();	
  }

  
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
	
  }

//	LL_RCC_HSI14_Enable();

   /* Wait till HSI14 is ready */
//	while(LL_RCC_HSI14_IsReady() != 1)
//	{
	
//	}
//	LL_RCC_HSI14_SetCalibTrimming(16);

//	LL_RCC_LSI_Enable();

   /* Wait till LSI is ready */
//	while(LL_RCC_LSI_IsReady() != 1)
//	{
	
//	}
 // LL_PWR_EnableBkUpAccess();

 // LL_RCC_ForceBackupDomainReset();

//	LL_RCC_ReleaseBackupDomainReset();

 // LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);

//	LL_RCC_EnableRTC();

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
//	LL_Init1msTick(48000000);

//	LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);

//	LL_SetSystemCoreClock(48000000);

//	LL_RCC_HSI14_EnableADCControl();

  /* SysTick_IRQn interrupt configuration */
//	NVIC_SetPriority(SysTick_IRQn, 0);
}

static void LL_Init(void)
{
  

  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);

  /* System interrupt init*/
  /* SVC_IRQn interrupt configuration */
//  NVIC_SetPriority(SVC_IRQn, 0);
  /* PendSV_IRQn interrupt configuration */
//  NVIC_SetPriority(PendSV_IRQn, 0);
  /* SysTick_IRQn interrupt configuration */
//  NVIC_SetPriority(SysTick_IRQn, 0);

}

void App_Udatat_Erase(void)
{
		IAP_INF_T infor={0};
		
	if(select_usart == 0xaa)
	{
		read_flash_holfword(FLASH_WRITE_START_ADDR, (uint16_t *)&infor, sizeof(infor));
		fw_sizes = infor.firmware;
		fw_checksum = infor.firmware_checksum;
		NbrOfPage = FLASH_PagesMask(fw_sizes);
	//	FW_ErasePage(NbrOfPage);
	}
}

void enter_bootloader(void)
{
	/* Flash unlock */
	FLASH_Unlock();
	LED_ON;
	RED_LED_ON;
	Usart22_Init();
	Usart11_Init();
	App_Udatat_Erase();
	para_init();
	  while(1)
	  {
		/* Execute the IAP driver in order to re-program the Flash */
		SerialDownload();
	  }
}

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
	LL_SYSCFG_SetRemapMemory(LL_SYSCFG_REMAP_FLASH);

	LL_Init();
	SystemClock_Config();
	Gpio_init();
	check_ID(); // check UID of IC if correct
	LED_ON;
	key_check();
	
	if(select_usart == 0xaa || select_usart == 0xbb || force_updata == 0xcc)
	{
		if(FROM_APP)iap_flash_write_iap_inf(0);
		enter_bootloader();
	}
	else
	{
		if (((*(vu32*)ApplicationAddress) & 0x2FFF0000 ) == 0x20000000)
		{
			//RED_LED_ON;
			JomToApp();
		}
		else
		{
			if(FROM_APP)iap_flash_write_iap_inf(0);
			force_updata = 0xcc;
			enter_bootloader();
		}
	}

}

/*******************************************************************************
* Function Name  : IAP_Init
* Description    : Initialize the IAP:Configure RCC, USART and GPIOs.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void Usart11_Init(void)
{
	  LL_USART_InitTypeDef USART_InitStruct;
	
	  LL_GPIO_InitTypeDef GPIO_InitStruct;
	
	  /* Peripheral clock enable */
	  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_USART1);
	 // LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
	 // LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
	  /**USART2 GPIO Configuration	
	  PA2	------> USART2_TX
	  PA3	------> USART2_RX 
	  */
	  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
	  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
	  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	  /* USART2 interrupt Init */
	 // NVIC_SetPriority(USART2_IRQn, 0);
	 // NVIC_EnableIRQ(USART2_IRQn);
	
	  USART_InitStruct.BaudRate = 9600;
	  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
	  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	  LL_USART_Init(USART1, &USART_InitStruct);
	  //LL_USART_EnableDMAReq_RX(USART2);
	 // LL_USART_EnableDMAReq_TX(USART2);
	
	//	LL_USART_DisableIT_CTS(USART2);
	
	//	LL_USART_DisableOverrunDetect(USART2);
	
	//	LL_USART_ConfigAsyncMode(USART2);
	
	  LL_USART_Enable(USART1);
	
	  //LL_USART_ReceiveData9(USART2);
	//	LL_USART_RequestRxDataFlush(USART2); // clear rx flag
	 // LL_USART_ClearFlag_ORE(USART1);
	//	LL_USART_ClearFlag_IDLE(USART2);
	 // LL_USART_EnableIT_IDLE(USART2);
	//	LL_USART_EnableIT_RXNE(USART2);
}


void Usart22_Init(void)
{
	  LL_USART_InitTypeDef USART_InitStruct;
	
	  LL_GPIO_InitTypeDef GPIO_InitStruct;
	
	  /* Peripheral clock enable */
	  //LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_USART1);
	  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
	 // LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
	  /**USART2 GPIO Configuration	
	  PA2	------> USART2_TX
	  PA3	------> USART2_RX 
	  */
	  GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
	  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	  GPIO_InitStruct.Pin = LL_GPIO_PIN_3;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
	  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	  /* USART2 interrupt Init */
	 // NVIC_SetPriority(USART2_IRQn, 0);
	 // NVIC_EnableIRQ(USART2_IRQn);
	
	  USART_InitStruct.BaudRate = 115200;
	  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
	  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	  LL_USART_Init(USART2, &USART_InitStruct);
	  //LL_USART_EnableDMAReq_RX(USART2);
	 // LL_USART_EnableDMAReq_TX(USART2);
	
	//	LL_USART_DisableIT_CTS(USART2);
	
	//	LL_USART_DisableOverrunDetect(USART2);
	
	//	LL_USART_ConfigAsyncMode(USART2);
	
	  LL_USART_Enable(USART2);
	
	  //LL_USART_ReceiveData9(USART2);
	//	LL_USART_RequestRxDataFlush(USART2); // clear rx flag
	 // LL_USART_ClearFlag_ORE(USART1);
	//	LL_USART_ClearFlag_IDLE(USART2);
	 // LL_USART_EnableIT_IDLE(USART2);
	//	LL_USART_EnableIT_RXNE(USART2);
}


#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  while(1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
