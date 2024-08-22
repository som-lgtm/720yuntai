
#define UART_TURN_ON	1

#ifdef UART_TURN_ON

#include "stm32f0xx_ll_usart.h"
#include "stdio.h"
#include "common.h"
#include "motor_dri.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_bus.h"
#include "string.h"
#include "uart.h"
#include "stm32f0xx_ll_dma.h"
#include "stm32f0xx_ll_rcc.h"


#define USART_RDR_ADDR	0x40013824
#define USART_TDR_ADDR	0x40013828
#define USART22_RDR_ADDR	0x40004424
#define USART22_TDR_ADDR	0x40004428

uint8_t app_read_buffer[USART_TAG]={0};
uint8_t con_buffer[USART_TAG] = {0};
uint8_t app_send_if=0;
uint8_t read_id = 0;
//uint8_t buffer_send_size = 0;
USART_RECEIVER usart_rev={
0};

USART_SENDER usart_send={
0};

USART22_RECEIVER usart22_rev={
0};

USART22_SENDER usart22_send={
0};

APP_BUFFER App_Buffer[LOCUS_BUFFER_MAX] = {0};
APP_BUFFER App_buffer_spt = {0};


USART11_SHADOW Usart11_shadow[3] = {0};

void send_interval_count(void)
{
	if(usart_send.send_interval)usart_send.send_interval--;
	if(usart22_send.send_interval22)usart22_send.send_interval22--;
}

static void Nvic_irq_set(void)
{
	NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0);
	NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
}

static void Usart1_Receiver_Dma_Init(void)
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
	//LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_1, 0);
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_3, USART_RDR_ADDR);
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_3, (uint32_t)&usart_rev.usart1_rec_b[0]);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3, USART_TAG); //总的采集通道数量ADC_TAG

	LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_3, LL_DMA_DIRECTION_PERIPH_TO_MEMORY); // 将串口数据搬到内存中
	
	LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PRIORITY_HIGH); 
	
	LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PERIPH_NOINCREMENT); //外设地址后移禁止
	
	LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MEMORY_INCREMENT); //内存ENABLE 后移
	
	LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PDATAALIGN_BYTE);
	
	LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PDATAALIGN_BYTE);
	LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MODE_NORMAL);

	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_3);
}

static void Usart1_Send_Dma_Init(void)
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
	//LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_1, 0);
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_2, USART_TDR_ADDR);
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_2, (uint32_t)&usart_send.usart1_send_b[0]);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, USART_SEND_TAG); //总的采集通道数量ADC_TAG

	LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_2, LL_DMA_DIRECTION_MEMORY_TO_PERIPH); // 将串口数据搬到内存中
	
	LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PRIORITY_HIGH); 
	
	LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PERIPH_NOINCREMENT); //外设地址后移禁止
	
	LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MEMORY_INCREMENT); //内存ENABLE 后移
	
	LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PDATAALIGN_BYTE);
	
	LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PDATAALIGN_BYTE);
	LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MODE_NORMAL);

	LL_DMA_ClearFlag_TC2(DMA1);
	LL_DMA_ClearFlag_GI2(DMA1);
//	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_2);
	
	Nvic_irq_set();
	//LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);
}

static void Usart1_Dma_Init(void)
{
	Usart1_Receiver_Dma_Init();
	Usart1_Send_Dma_Init();
}
/* USART2 init function */

static void Usart11_Init(void)
{
	
	  LL_USART_InitTypeDef USART_InitStruct;
	
	  LL_GPIO_InitTypeDef GPIO_InitStruct;
	
	  /* Peripheral clock enable */
	  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_USART1);
	  //LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
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
	  NVIC_SetPriority(USART1_IRQn, 0);
	  NVIC_EnableIRQ(USART1_IRQn);
	
	  USART_InitStruct.BaudRate = 9600;
	  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
	  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	  LL_USART_Init(USART1, &USART_InitStruct);
//	  LL_USART_SetTransferBitOrder(USART1, LL_USART_BITORDER_MSBFIRST);
	  LL_USART_EnableDMAReq_RX(USART1);
	  LL_USART_EnableDMAReq_TX(USART1);
	
	//	LL_USART_DisableIT_CTS(USART2);
	
	//	LL_USART_DisableOverrunDetect(USART2);
	
	//	LL_USART_ConfigAsyncMode(USART2);
	
	  LL_USART_Enable(USART1);
	
	//	LL_USART_RequestRxDataFlush(USART2); // clear rx flag
	 // LL_USART_ClearFlag_ORE(USART1);
		LL_USART_ClearFlag_IDLE(USART1);
	  LL_USART_EnableIT_IDLE(USART1);
	//	LL_USART_EnableIT_RXNE(USART1);
}

/* USART1 init function */
void MX_USART1_UART_Init(void)
{
	Usart11_Init();
	Usart1_Dma_Init();
}

void Interval_set(void)
{
	usart_send.send_interval = 80;
}

void Usart11_SetEvent(uint8_t lenght)
{
	usart_send.send_event = TRUE;
	usart_send.send_lengt = lenght;
}
void Usart11_ResetEvent(void)
{
	usart_send.send_event = FALSE;
}
uint8_t Usart11_ReturnEvent(void)
{
	return usart_send.send_event;
}

void SendData_Load(void)
{
	usart_send.data_load = TRUE;
}
void ResendData_Load(void)
{
	usart_send.data_load = FALSE;
}
uint8_t Return_SendData_Load(void)
{
	return usart_send.data_load;
}

void Reset_Tx_Complete(void)
{
	usart_send.Tx_tag = FALSE;
	Interval_set();
}
void Set_Tx_Complete(void)
{
	usart_send.Tx_tag = TRUE;
	usart_send.send_interval = 20;
}
uint8_t return_DMA_SendComplete(void)
{
	return usart_send.Tx_tag;
}

void Reset_RX_Complete(void)
{
	usart_rev.Rx_flag = FALSE;
}
void Set_RX_Complete(void)
{
	usart_rev.Rx_flag = TRUE;
}
uint8_t return_RX_Complete(void)
{
	return usart_rev.Rx_flag;
}

// turn on Dma to send usart data
void Open_Send_Dma(void)
{
//	static uint8_t aaaa=0;
//	 uint8_t index = 0;
	if(return_DMA_SendComplete())return; // wait for send complete
	if(usart_send.send_interval)return; // Sending interval
	
	Launch_Cache(); //
	if(App_buffer_spt.app_send_size == 0)return; // 检测到发送缓冲区没有数据退出不发送
	
	usart_send.send_lengt = App_buffer_spt.app_send_size;
	Set_Tx_Complete(); // set transmission flag
	memcpy(&usart_send.usart1_send_b[0], &App_buffer_spt.app_send_buffer[0], usart_send.send_lengt); // load data to DMA send buffer
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, (uint32_t)usart_send.send_lengt);
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_2);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);
	App_buffer_spt.app_send_size = 0; // 清除发送缓冲区长度
	
}

// DMA send Interrupt handling function
void Dma_SendIRQ_Dispose (void)
{
	LL_DMA_DisableIT_TC(DMA1, LL_DMA_CHANNEL_2);
	LL_DMA_ClearFlag_TC2(DMA1);
	LL_DMA_ClearFlag_GI2(DMA1);
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
	Reset_Tx_Complete();
}

void Usart11_Send_Handling(uint8_t TX_size, uint8_t *value_da)
{
	//	if(!Usart11_ReturnEvent())return; // without data to return;
	//	if(return_DMA_SendComplete())return; // wait for send complete
		while(return_DMA_SendComplete())
		{}
	//	if(usart_send.send_interval)return; // Sending interval
		usart_send.send_lengt = TX_size;
	//	ResendData_Load(); // clear data load flag
		Set_Tx_Complete(); // set transmission flag
		memcpy(&usart_send.usart1_send_b[0], value_da, usart_send.send_lengt); // load data to DMA send buffer
	//	ResendData_Load();
		LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, (uint32_t)usart_send.send_lengt);
		LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_2);
		LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);
}

// LL_DMA_GetDataLength():Get the remaining bytes, A total of 20 bytes
// USART_TAG: total of transfer bytes = 20

void usart1_receiver(void)
{
		usart_rev.RX_lengt = USART_TAG - LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_3);
		
		if(usart_rev.RX_lengt == 0)return; // No valid data, so return
		
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3); // To avoid interference
		LL_USART_Disable(USART1);
		if(usart_rev.RX_lengt > USART_TAG)usart_rev.RX_lengt = USART_TAG;
		
		Set_RX_Complete(); // To processing valid data
//		LL_USART_TransmitData8(USART1, usart_rev.RX_lengt);
}

uint8_t checksum_confirm(uint8_t *spt, uint8_t lenght)
{
	uint8_t i;
	uint16_t temp =0;
	uint8_t checksum =0;
	
	for(i=0; i< (lenght-1); i++)
	{
		temp += (uint16_t)spt[i];
	}

	checksum = temp;
	
	if(checksum != spt[lenght-1])
	{
		return FALSE;
	}

	return TRUE;
}

void usart11_Recdata_Handling(void)
{
	uint8_t RX_size = 0;
//	static uint8_t ack_times = 0;
	
	if(return_RX_Complete()) // event handling
	{
		Reset_RX_Complete(); // clear event flag
		RX_size = usart_rev.RX_lengt; // backup data size, to avoid reset
		memcpy(app_read_buffer, &usart_rev.usart1_rec_b[0], RX_size); // load data to DMA send buffer
		LL_USART_Enable(USART1);
		LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3, USART_TAG); //总的采集通道数量ADC_TAG
		LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_3); // turn on DMA1 channel 3
		//if(checksum_verify(app_read_buffer, RX_size) == FALSE)return; 
		
		
		/*
		if(checksum_confirm(app_read_buffer, RX_size) == FALSE)
		{
			//App_Buffer[1].app_send_size = usart22_send.send_lengt22;
			return; 
		}*/
		
//		ack_times = 0;

		receiver_data_from_A650();
	}
}


///////////////////////////////////////////////////usart22/////////////////////////////////////////
static void Usart22_Nvic_irq_set(void)
{
	NVIC_SetPriority(DMA1_Channel4_5_IRQn, 0);
	NVIC_EnableIRQ(DMA1_Channel4_5_IRQn);
}

static void Usart22_Receiver_Dma_Init(void)
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
	//LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_1, 0);
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_5, USART22_RDR_ADDR);
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t)&usart22_rev.usart2_rec_b[0]);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, USART_SEND_TAG); //总的采集通道数量ADC_TAG

	LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_5, LL_DMA_DIRECTION_PERIPH_TO_MEMORY); // 将串口数据搬到内存中
	
	LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_5, LL_DMA_PRIORITY_HIGH); 
	
	LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_5, LL_DMA_PERIPH_NOINCREMENT); //外设地址后移禁止
	
	LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_5, LL_DMA_MEMORY_INCREMENT); //内存ENABLE 后移
	
	LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_5, LL_DMA_PDATAALIGN_BYTE);
	
	LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_5, LL_DMA_PDATAALIGN_BYTE);
	LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_5, LL_DMA_MODE_NORMAL);

	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5);
}

static void Usart22_Send_Dma_Init(void)
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
	//LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_1, 0);
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_4, USART22_TDR_ADDR);
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_4, (uint32_t)&usart22_send.usart2_semd_b[0]);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, USART_SEND_TAG); //总的采集通道数量ADC_TAG

	LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_4, LL_DMA_DIRECTION_MEMORY_TO_PERIPH); // 将串口数据搬到内存中
	
	LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_4, LL_DMA_PRIORITY_HIGH); 
	
	LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_4, LL_DMA_PERIPH_NOINCREMENT); //外设地址后移禁止
	
	LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_4, LL_DMA_MEMORY_INCREMENT); //内存ENABLE 后移
	
	LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_4, LL_DMA_PDATAALIGN_BYTE);
	
	LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_4, LL_DMA_PDATAALIGN_BYTE);
	LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_4, LL_DMA_MODE_NORMAL);

	LL_DMA_ClearFlag_TC4(DMA1);
	LL_DMA_ClearFlag_GI4(DMA1);
//	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_2);
	
	Usart22_Nvic_irq_set();
	//LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);
}

static void Usart22_Dma_Init(void)
{
	Usart22_Receiver_Dma_Init();
	Usart22_Send_Dma_Init();
}

static void Usart22_init(void)
{
//	uint32_t periphclk = LL_RCC_PERIPH_FREQUENCY_NO;
	  LL_USART_InitTypeDef USART_InitStruct;
	
	  LL_GPIO_InitTypeDef GPIO_InitStruct;
//	LL_RCC_ClocksTypeDef RCC_Clocks;
	  /* Peripheral clock enable */
	 // LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_USART1);
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
	  NVIC_SetPriority(USART2_IRQn, 0);
	  NVIC_EnableIRQ(USART2_IRQn);
	
	  USART_InitStruct.BaudRate = 115200;
	  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
	  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	  LL_USART_Init(USART2, &USART_InitStruct);

	  
      /* USART2 clock is PCLK */
 /*     LL_RCC_GetSystemClocksFreq(&RCC_Clocks);
      periphclk = RCC_Clocks.PCLK1_Frequency;
	  LL_USART_SetBaudRate(USART2, periphclk, LL_USART_OVERSAMPLING_16, 115200);
	 // LL_USART_SetAutoBaudRateMode(USART2, LL_USART_AUTOBAUD_DETECT_ON_STARTBIT);
	 LL_USART_ConfigCharacter(USART2, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);
	 LL_USART_SetHWFlowCtrl(USART2, LL_USART_HWCONTROL_NONE);
	 LL_USART_SetTransferDirection(USART2, LL_USART_DIRECTION_TX_RX);
	 */
	 // LL_USART_EnableDMAReq_RX(USART2);
	 // LL_USART_EnableDMAReq_TX(USART2);
	
	//	LL_USART_DisableIT_CTS(USART2);
	
	//	LL_USART_DisableOverrunDetect(USART2);
	
	//	LL_USART_ConfigAsyncMode(USART2);
	
	  LL_USART_Enable(USART2);
	
	  //LL_USART_ReceiveData9(USART2);
	//	LL_USART_RequestRxDataFlush(USART2); // clear rx flag
	 // LL_USART_ClearFlag_ORE(USART1);
	//	LL_USART_ClearFlag_IDLE(USART2);
	//  LL_USART_EnableIT_IDLE(USART2);
		LL_USART_EnableIT_RXNE(USART2);
}

void MX_USART2_UART_Init(void)
{
	Usart22_init();
	//Usart22_Dma_Init();
}

void Usart22_Interval_set(void)
{
	usart22_send.send_interval22 = 80;
}

void Usart22_Event_Flag(uint8_t lenght) // set sending tag
{
//	usart22_send.events_flag22 = TRUE;
	usart22_send.send_lengt22 = lenght;
}
void Usart22_Reset_Event_Flag(void)
{
	usart22_send.events_flag22 = FALSE;
}
uint8_t Usart22_Return_Event_Flag(void)
{
	return usart22_send.events_flag22;
}

void Usart22_Reset_Tx_Complete(void)
{
	usart22_send.Tx_tag22 = FALSE;
	Usart22_Interval_set();
}
void Usart22_Set_Tx_Complete(void)
{
	usart22_send.Tx_tag22 = TRUE;
	usart22_send.send_interval22 = 20;
}
	
uint8_t Usart22_return_Tx_Complete(void)
{
	return usart22_send.Tx_tag22;
}

void Usart22_Load_SendData(uint8_t typek)
{
	usart22_send.load_flag22 = typek;
}
uint8_t Return_Usart22_Load_SendData(void)
{
	return usart22_send.load_flag22;
}

void Usart22_Reset_RX_Complete(void)
{
	usart22_rev.Rx_flag22 = FALSE;
}
void Usart22_Set_RX_Complete(void)
{
	usart22_rev.Rx_flag22 = TRUE;
}
uint8_t Usart22_return_RX_Complete(void)
{
	return usart22_rev.Rx_flag22;
}

void Launch_Cache(void)
{
	uint8_t index = 0;
	
	if(App_buffer_spt.app_send_size)return; // 检测到发送缓冲区有数据退出不装载待发数据包

	for(index = 0; index < LOCUS_BUFFER_MAX; index++)
	{
		if(App_Buffer[index].app_send_size)break; // 检测待发送包是否有数据包，如果有跳出并将其装载到发送缓冲区
	}
	
	if(index >= LOCUS_BUFFER_MAX)return; // 如果待发送包没有数据就退出

	memcpy(&App_buffer_spt.app_send_buffer[0], App_Buffer[index].app_send_buffer, App_Buffer[index].app_send_size); // load data to Launch Cache buffer
	App_buffer_spt.app_send_size = App_Buffer[index].app_send_size; // 同步发送长度
	App_buffer_spt.app_send_size_back = App_buffer_spt.app_send_size; // 备份 发送长度
	App_Buffer[index].app_send_size = 0; // 清除待发送数据包长度
}


// turn on Dma to send usart data; start transfer
void Usart22_Open_Send_Dma(void)
{
/*//	 uint8_t index = 0;
	if(Usart22_return_Tx_Complete())return; // wait for send complete
	if(usart22_send.send_interval22)return; // Sending interval
	Launch_Cache(); //

	if(App_buffer_spt.app_send_size == 0)return; // 检测到发送缓冲区没有数据退出不发送
	
	usart22_send.send_lengt22 = App_buffer_spt.app_send_size;
	Usart22_Set_Tx_Complete(); // set transmission flag
	memcpy(&usart22_send.usart2_semd_b[0], &App_buffer_spt.app_send_buffer[0], usart22_send.send_lengt22); // load data to DMA send buffer
	App_buffer_spt.app_send_size = 0; // 清除发送缓冲区长度

	// 开串口发送DMA
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, (uint32_t)usart22_send.send_lengt22);
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_4);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);*/
}

// DMA send Interrupt handling function
void Usart22_Dma_SendIRQ_Dispose (void)
{
/*	LL_DMA_DisableIT_TC(DMA1, LL_DMA_CHANNEL_4);
	LL_DMA_ClearFlag_TC4(DMA1);
	LL_DMA_ClearFlag_GI4(DMA1);
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
	Usart22_Reset_Tx_Complete();*/
}
// LL_DMA_GetDataLength():Get the remaining bytes, A total of 20 bytes
// USART_TAG: total of transfer bytes = 20

void Usart22_ReadByte(void)
{
	if(read_id < 20)
	{
		usart22_rev.usart2_rec_b[read_id] = LL_USART_ReceiveData8(USART2);
	//	LL_USART_TransmitData8(USART2, usart22_rev.usart2_rec_b[read_id]);
		read_id +=1;
	}
	else
	{
		LL_USART_ReceiveData8(USART2);
	}
}


void Usart22_Receiver(void)
{
		usart22_rev.RX_lengt22 = read_id;
		read_id = 0;
		Usart22_Set_RX_Complete(); // To processing valid data
}

void usart22_Recdata_Handling(void)
{
	uint8_t RX_size = 0;
//	static uint8_t ack_times = 0;
	
	if(Usart22_return_RX_Complete()) // event handling
	{
		Usart22_Reset_RX_Complete(); // clear event flag
		RX_size = usart22_rev.RX_lengt22; // backup data size, to avoid reset
		memcpy(app_read_buffer, &usart22_rev.usart2_rec_b[0], RX_size); // load data to DMA send buffer
	//	String_Printf(app_read_buffer, RX_size);
		bootloader_action_if(app_read_buffer);
		check_enter_boot_if(app_read_buffer);
	}
}

void usart22_read_byte(void)
{
	Usart22_Receiver();
	usart22_Recdata_Handling();
}

void uart_send_data(unsigned short data)
{
//	while(!(USART_GetFlagStatus(USART1, USART_FLAG_TXE)));
//	USART_SendData(USART1, data);
}


void uart_print(unsigned char str[])
{
	uint8_t i;
	
	for(i=0;i<sizeof(str);i++)
	{
		uart_send_data((uint16_t)str[i]);
	}
}

void uart_string_send(unsigned char *str)
{

	while(0 != *str)
	{
		uart_send_data(*str);
		str++;
	}
}
void print_word_data( uint32_t data, uint8_t p[20])
{

	unsigned char buff[20];
	
	sprintf( (char *)buff,"%s = \"%d\"",p,data);
	uart_string_send(&buff[0]);
}

void usart1_send_byte(uint8_t datat)
{
		while(!LL_USART_IsActiveFlag_TXE(USART1));
		LL_USART_TransmitData8(USART1, datat);
}

void usart_timer_test(void)
{
	static uint16_t temp=0;
	temp++;
	//if(temp >= 1000)
	{
	//	temp = 0;
		LL_USART_TransmitData8(USART1, 0xaa);
	}
}

void String_Printf(uint8_t *sptf, uint8_t sizes)
{
	uint8_t i=0;
	/*if(Usart22_return_Tx_Complete())return;
	usart22_send.send_lengt22 = sizes;
	Usart22_Reset_RX_Complete(); // set transmission flag
	memcpy(&usart22_send.usart2_semd_b[0], sptf, usart22_send.send_lengt22); // load data to DMA send buffer
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, (uint32_t)usart22_send.send_lengt22);
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_4);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);*/
	
	for(i=0; i<sizes; i++)
	{
		while(!LL_USART_IsActiveFlag_TXE(USART2));
		LL_USART_TransmitData8(USART2, sptf[i]);
	}
}

#endif



