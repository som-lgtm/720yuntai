
#include "uart.h"
#include "stdio.h"
#include "string.h"
#include "gpio.h"
#include "common.h"
#include "stm32f0xx_ll_usart.h"
#include "stm32f0xx_ll_dma.h"
#include "stm32f0xx_ll_gpio.h"
#include "rf24l01.h"
uint16_t Pair_times = 0;

void Send_NameToBle(void);

void Pairing_time(void)
{
	if(Pair_times)Pair_times--;
}

void lights_flash(void)
{
	static uint8_t led_tag = 0;
	led_tag = ~led_tag;
	if(led_tag)
	{
		RED_LED_ON;
		Green_led_onORoff(0);
	}
	else
	{
		RED_LED_OFF;
		Green_led_onORoff(1);
	}
}
uint8_t check_sum_add22(uint8_t index, uint8_t *spt)
{
	uint8_t i;
	uint16_t temp=0;

	spt[index] = 0;
	
	for(i=0;i<index; i++)
	{
		temp += (uint16_t)spt[i];
	}

	//uart1_s_buffer[0] = temp & 0xff;
	//display_test11(uart1_s_buffer[0]);

	return (temp & 0xff);
}

// 首次使用一定要烧ID码
void Burn_code_value(void)
{
	uint8_t addr[]={0x02,0,0,0,0};
//	uint8_t buff[15];
	uint8_t sizes = 0;
	uint8_t send_mode = 0;
	uint8_t timeover = 0;
	uint8_t get_id = 0;
	uint8_t test_mode = 0;
	
	RF24L01_CE_OFF;
	NRF24L01_Write_Reg( RF_CH, 0 ); 						//初始化通道
	NRF24L01_Write_Reg( RF_SETUP, RF_DR_HIGH|RF_DR_LOW|(RF_PWR & 0x02));  //谢频数据率为250K,此值越高侧发送距离越近否侧越远
															// 发送功能为-4dbm 
	NRF24L01_Set_TxAddr( &addr[0], 5 ); 					 //设置TX 地址
	NRF24L01_Set_RxAddr( 0, &addr[0], 5 );					 //设置P0管道RX 地址用于接收ACK应答
	lights_flash();

	
	RF24L01_Set_Mode( MODE_RX );
	RF24L01_CE_ON;
//	boot_time = 200;

	
	while(1)
	{
		Feed_IWDG(); //喂狗
		//RF24L01_Set_Mode( MODE_RX );
		//RF24L01_CE_ON;
		sizes = NRF24L01_RxPacket(app_read_buffer);
		if(sizes != 0)
		{
			if(app_read_buffer[1] == 0xcc)
			{
				if(app_read_buffer[2] == 0x02)
				{
					memcpy(&glob_para.wifi_id.TX_adress[0], &app_read_buffer[3], 5); // load data to DMA send buffer
					glob_para.wifi_id.wifi_rfc = app_read_buffer[8];
					if_write_flash();
					write_flash_active();
					send_mode = 1;
					App_Buffer[0].app_send_buffer[1] = 0xbb;
					App_Buffer[0].app_send_buffer[2] = 0xbb;
					
					App_Buffer[0].app_send_buffer[0] = check_sum_add22(3, App_Buffer[0].app_send_buffer);
					App_Buffer[0].app_send_size= 3;
					
					RF24L01_CE_OFF;
					RF24L01_Set_Mode( MODE_TX );
					get_id = 1;
					sizes = 0;
					//break;
				}
			}
			/*else if(app_read_buffer[1] == 0x0b)
			{
				if(app_read_buffer[2] == 0x0c)
				{
					if(app_read_buffer[3] == 0xff)
					{
						test_mode = 1;
						break;
					}
				}
			}*/
		}


		if(Pair_times == 0)
		{
			Pair_times = 200;
			lights_flash();
			timeover += 1;
			if(timeover >= 15)break;
			if(send_mode)
			{
				RF24L01_Set_Mode( MODE_TX );
				NRF24L01_TxPacket( &App_Buffer[0].app_send_buffer[0], App_Buffer[0].app_send_size );
				RF24L01_CE_OFF;
				time_delay_ms(1);
				RF24L01_Set_Mode( MODE_RX );
				RF24L01_CE_ON;
				if(timeover>=10)break;
			}
		}
	}

	if(get_id)
	{
	//	LED_ON;
	//	RED_LED_ON;
	//	time_delay_ms(1000);
		//LED_OFF;
		//RED_LED_ON;
	}

	if(test_mode)
	{
		//FW_test_main();
	}
	
//	Nrf24L01_init();
	RF24L01_CE_OFF;
	time_delay_ms(1);
	//Nrf24L01_init();
	NRF24L01_Write_Reg( FLUSH_TX,0xff );	//清除FIFO
	NRF24L01_Write_Reg( STATUS,0xff );	//清除STATUS
	NRF24L01_Write_Reg( RF_CH, glob_para.wifi_id.wifi_rfc ); 						//初始化通道
    NRF24L01_Write_Reg( RF_SETUP, 0x27 );					//谢频数据率为250K,此值越高侧发送距离越近否侧越远
															// 发送功能为-4dbm 
	NRF24L01_Set_TxAddr( &glob_para.wifi_id.TX_adress[0], 5 );						//设置TX 地址
	NRF24L01_Set_RxAddr( 0, &glob_para.wifi_id.TX_adress[0], 5 );					//设置P0管道RX 地址用于接收ACK应答
	RF24L01_Set_Mode( MODE_RX );
	RF24L01_CE_ON;
	App_Buffer[0].app_send_size = 0;
	
//	Ble_init();
//	Green_led_onORoff(0);
	RED_LED_OFF;
}



