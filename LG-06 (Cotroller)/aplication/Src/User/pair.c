
#include "uart.h"
#include "stdio.h"
#include "string.h"
#include "gpio.h"
#include "common.h"
#include "oled.h"
#include "stm32f0xx_ll_usart.h"
#include "stm32f0xx_ll_dma.h"
#include "rf24l01.h"
#include "keyscan.h"

static void Paier_Indicate(void);
extern void read_flash_holfword(uint16_t *data);
static void Pair_send_data(void);
uint16_t Pair_times = 0;
uint8_t ble_name[23]={0};
uint8_t send_mode = 0;

uint8_t ICcode_checksum_verify(uint8_t *spt, uint8_t lenght)
{
	uint8_t i;
	uint16_t temp =0;
	uint8_t checksum =0;
	
	for(i=1; i< lenght; i++)
	{
		temp += (uint16_t)spt[i];
	}

	checksum = temp;
	
	if(checksum != spt[0])
	{
		return FALSE;
	}

	return TRUE;
}


void Pairing_time(void)
{
	if(Pair_times)Pair_times--;
}

void initerval_set(void)
{
	usart_send.send_interval = 100;
}

void RF_reconver(void)
{
	RF24L01_CE_OFF;
	time_delay_ms(300);
	//Nrf24L01_init();
	NRF24L01_Write_Reg( FLUSH_TX,0xff );	//清除FIFO
	NRF24L01_Write_Reg( STATUS,0xff );	//清除STATUS
    NRF24L01_Write_Reg( RF_CH, wifi_id.wifi_rfc );             			//初始化通道
    NRF24L01_Write_Reg( RF_SETUP, 0x27 );					//谢频数据率为250K,此值越高侧发送距离越近否侧越远
															// 发送功能为7dbm
	NRF24L01_Set_TxAddr( &wifi_id.TX_adress[0], 5 );					  //设置TX 地址
	NRF24L01_Set_RxAddr( 0, &wifi_id.TX_adress[0], 5 ); 				  //设置P0管道RX 地址用于接收ACK应答
}
// 进入对码状态
void Enter_into_Pair(void)
{
	uint8_t addr[]={0x02,0,0,0,0};
	uint8_t Send_status = 0;
	uint8_t temp = 0;
	uint8_t sizes = 0;
	LCD_Fill(2, 24, 240, 240, BLACK);
	bornd_code_dis(3, 3);
//	ENTER_ATS;
	 RF24L01_CE_OFF;
	 NRF24L01_Write_Reg( RF_CH, 0 );						 //初始化通道
	 NRF24L01_Write_Reg( RF_SETUP, RF_DR_HIGH|RF_DR_LOW|(RF_PWR & 0x02));  //谢频数据率为250K,此值越高侧发送距离越近否侧越远
															 // 发送功能为-4dbm 
	 NRF24L01_Set_TxAddr( &addr[0], 5 );					  //设置TX 地址
	 NRF24L01_Set_RxAddr( 0, &addr[0], 5 ); 				  //设置P0管道RX 地址用于接收ACK应答
		
	App_Buffer[0].app_send_buffer[1] = 0xcc;
	App_Buffer[0].app_send_buffer[2] = 0x02;
	App_Buffer[0].app_send_buffer[3] = wifi_id.TX_adress[0];
	App_Buffer[0].app_send_buffer[4] = wifi_id.TX_adress[1];
	App_Buffer[0].app_send_buffer[5] = wifi_id.TX_adress[2];
	App_Buffer[0].app_send_buffer[6] = wifi_id.TX_adress[3];
	App_Buffer[0].app_send_buffer[7] = wifi_id.TX_adress[4];
	App_Buffer[0].app_send_buffer[8] = wifi_id.wifi_rfc;
	
	App_Buffer[0].app_send_buffer[0] = check_sum_add(9, App_Buffer[0].app_send_buffer);
	App_Buffer[0].app_send_size = 9;
	RF24L01_Set_Mode( MODE_TX );
	RF24L01_CE_OFF;
	app_read_buffer[1]=0;
	while(1)
	{
		Feed_IWDG(); //喂狗
		sizes = NRF24L01_RxPacket(app_read_buffer);
		if(sizes != 0)
		{
			if(app_read_buffer[1] == 0xbb)
			{
				if(app_read_buffer[2] == 0xbb)
				{
					app_read_buffer[1] = 0;
					Send_status = 1;
					sizes = 0;
					break;
				}
			}
		}
		
		if(Pair_times == 0)
		{
			RF24L01_CE_OFF;
			RF24L01_Set_Mode( MODE_TX );
			Pair_times = 350;
			NRF24L01_TxPacket( &App_Buffer[0].app_send_buffer[0], App_Buffer[0].app_send_size );
			
			Paier_Indicate();
			RF24L01_CE_OFF;
			time_delay_ms(1);
			RF24L01_Set_Mode( MODE_RX );
			RF24L01_CE_ON;
			time_delay_ms(1);
			if(temp >= 15)
			{
				break;
			}
			temp += 1;
			
		}
		
		
	}

	if(Send_status){
		bornd_code_dis(3, 4);
	}
	else{
		bornd_code_dis(3, 5);
	}

	RF_reconver();
	App_Buffer[0].app_send_size = 0;
}

// 首次使用一定要烧ID码
void Burn_code_value(void)
{
	uint8_t addr[]={INIT_ADDR};
	uint8_t get_id = 0;
	uint8_t sizes = 0;
	uint8_t send_s = 0;

	if(wifi_id.id_default == 0x07)
	{
		//Set_New_idcode();
		//usart_set_and_get(RESET_DEVICE, 11);
		return;
	}
	
	
    RF24L01_CE_OFF;
    NRF24L01_Write_Reg( RF_CH, 0 );             			//初始化通道
   // NRF24L01_Write_Reg( RF_SETUP, RF_DR_HIGH|RF_DR_LOW|(RF_PWR & 0x06));	//谢频数据率为250K,此值越高侧发送距离越近否侧越远
															// 发送功能为4dbm 
	NRF24L01_Set_TxAddr( &addr[0], 5 ); 					 //设置TX 地址
	NRF24L01_Set_RxAddr( 0, &addr[0], 5 );					 //设置P0管道RX 地址用于接收ACK应答

	
	App_Buffer[0].app_send_buffer[1] = 0xbb;
	App_Buffer[0].app_send_buffer[2] = 0xff;
	
	App_Buffer[0].app_send_buffer[0] = check_sum_add(3, App_Buffer[0].app_send_buffer);
	App_Buffer[0].app_send_size = 3;
	initerval_set();
	send_mode = 0;
	RF24L01_Set_Mode( MODE_TX );
	LCD_Fill(0, 24, 240, 240, BLACK);
	bornd_code_dis(3, 1);

	while(!get_id)
	{
		Feed_IWDG(); //喂狗
 		 
		 Pair_send_data();
		 if(send_mode == MODE_RX)
		 {
		 	sizes = NRF24L01_RxPacket(app_read_buffer);
			if(sizes != 0)
			{
				if(ICcode_checksum_verify(app_read_buffer, sizes)){
					if(app_read_buffer[1] == 0xaa)
					{
						if(app_read_buffer[2] == 0x01)
						{
							memcpy(&wifi_id.TX_adress[0], &app_read_buffer[3], 5); // load data to DMA send buffer
							wifi_id.wifi_rfc = app_read_buffer[8];
							wifi_id.id_default = 0x07;
							get_id = 1;
							bornd_code_dis(3, 2);
							sizes = 0;
							App_Buffer[0].app_send_size = 0;
							if_write_flash();
							RF24L01_CE_OFF;
						}
					}
				}
			}
		 }
 	}

		
		App_Buffer[0].app_send_buffer[1] = 0xaa;
		App_Buffer[0].app_send_buffer[2] = 0xaa;
		
		App_Buffer[0].app_send_buffer[0] = check_sum_add(3, App_Buffer[0].app_send_buffer);
		App_Buffer[0].app_send_size = 3;
		initerval_set();
		RF24L01_Set_Mode( MODE_TX );

		while(1)
		{
			Feed_IWDG(); //喂狗
			if(Pair_times == 0)
			{
				Pair_times = 50;
				NRF24L01_TxPacket( &App_Buffer[0].app_send_buffer[0], App_Buffer[0].app_send_size );
				send_s += 1;
				if(send_s >= 6)break;
			}
		}
	
	//time_delay_ms(100);
	
	if(get_id)
	{
		RF_reconver();
		App_Buffer[0].app_send_size = 0;
	}
	
	//BLE_Change_Name();
}

void Paier_Indicate(void)
{
}

void Pair_send_data(void)
{
	uint8_t buff[]={0};
//	uint8_t i;
//	uint8_t index = 0;
	uint8_t Send_status = 0;
	static uint8_t send_times = 0; //再发次数
//	static uint8_t send_overTime = 0; //发送超时次数
	
	if(boot_time)return; // Sending interval
	if(App_Buffer[0].app_send_size == 0)return;
	
	send_mode = 0;
	RF24L01_CE_OFF;
	RF24L01_Set_Mode( MODE_TX );		//发送模式
	
//	memcpy(&Cache.Cache_SBf[0], App_Buffer[0].app_send_buffer, App_Buffer[0].app_send_size); // load data to DMA send buffer
	Send_status = NRF24L01_TxPacket( &App_Buffer[0].app_send_buffer[0], App_Buffer[0].app_send_size );
	if(Send_status == TX_OK)
	{
		RF24L01_CE_OFF;
		//Usart11_shadow[index].shadow_send_size = 0;
		boot_time = 500;
		RF24L01_Set_Mode( MODE_RX );		//发送成功后切换为接收模式进入接收监听状态
//		send_times = 0;
//		send_overTime = 0;
		RF24L01_CE_ON;
		send_mode = MODE_RX;
	}
	else if(Send_status == MAX_TX)//如果是重发次数超了就再发
	{
		boot_time = 500;
		RF24L01_CE_OFF;
		RF24L01_Set_Mode( MODE_RX );		//发送成功后切换为接收模式进入接收监听状态
		RF24L01_CE_ON;
		send_mode = MODE_RX;
		
		sprintf( (char *)buff,"%d", send_times);
		Oled_EnlPrint(0, 3, buff,"",ENGLISH);
		send_times += 1;
	}
	else //如果是发送超时重新初始化后重发数据
	{
		boot_time = 500;
		RF24L01_CE_OFF;
		RF24L01_Set_Mode( MODE_RX );		//发送成功后切换为接收模式进入接收监听状态
		RF24L01_CE_ON;
		send_mode = MODE_RX;
	}
}


///////////////////////////////////////////////////////////  change ble name //////////////////////////////////////////////////


void GetBLE_Name(void)
{
	uint8_t buff[]="AT+NAMBZY-Mars 2-X\r\n";

	/*if(glob_para.wifi_id.wifi_Rfid <= 0x0f)
	{
		sprintf( (char *)buff,"AT+NAMBZY-A6502S-000%x\r\n", glob_para.wifi_id.wifi_Rfid);
	}
	else if(glob_para.wifi_id.wifi_Rfid <= 0xff)
	{
		sprintf( (char *)buff,"AT+NAMBZY-A6502S-00%x\r\n", glob_para.wifi_id.wifi_Rfid);
	}
	else if(glob_para.wifi_id.wifi_Rfid <= 0x0fff)
	{
		sprintf( (char *)buff,"AT+NAMBZY-A6502S-0%x\r\n", glob_para.wifi_id.wifi_Rfid);
	}
	else
	{
		sprintf( (char *)buff,"AT+NAMBZY-A6502S-%x\r\n", glob_para.wifi_id.wifi_Rfid);
	}*/

	memcpy(&ble_name[0], buff, 20); // load data to DMA send buffer*/
}

void pair_key_check(void)
{
	uint8_t temp = 0;
	
	
	if(SHUTTER)
	{
		boot_time = 1000;
		
		while(1)
		{
			Feed_IWDG();
			if(SHUTTER == 0)break;
			
			if(boot_time == 0)
			{
				if(SHUTTER)
				{
					temp = TRUE;
				}
				
				break;
			}
		}
	}

	if(temp == TRUE)
	{
		Enter_into_Pair();
	}
}



