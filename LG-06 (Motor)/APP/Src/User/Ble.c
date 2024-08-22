
#include "common.h"
#include "uart.h"
#include "string.h"
#include "stdio.h"
#include "gpio.h"
#include "stm32f0xx_ll_dma.h"
#include "stm32f0xx_ll_usart.h"
#include "stm32f0xx_ll_gpio.h"

uint8_t get_buffer[50]={0};
uint8_t MAC_addr[12] = {0};
uint8_t wk_tag = 0;



void usart_set_the_data(uint8_t *spt, uint8_t sizes)
{
	usart_send.send_lengt = sizes;
//	Usart22_Set_Tx_Complete(); // set transmission flag
	memcpy(&usart_send.usart1_send_b[0], spt, usart_send.send_lengt); // load data to DMA send buffer
	
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, (uint32_t)usart_send.send_lengt);
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_2);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);
}

void reset_usart_RX(void)
{
	Reset_RX_Complete(); // clear event flag
	LL_USART_Enable(USART1);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3, USART_TAG); //总的采集通道数量ADC_TAG
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_3); // turn on DMA1 channel 3
}

void Usart_GetData_From_BLE(uint8_t *spt)
{
	uint8_t RX_size = 0;
	
	//while(!Usart22_return_RX_Complete());
	if(return_RX_Complete())
	{
		Reset_RX_Complete(); // clear event flag
		RX_size = usart_rev.RX_lengt; // backup data size, to avoid reset
		memcpy(spt, &usart_rev.usart1_rec_b[0], RX_size); // load data to DMA send buffer
		LL_USART_Enable(USART1);
		LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3, USART_TAG); //总的采集通道数量ADC_TAG
		LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_3); // turn on DMA1 channel 3
		
		//String_Printf(get_buffer, string_lengt(get_buffer));
	}
}

// set the baud is 115200
// set the ble is host
// set the ble name is C400-host
// set the Broadcast interval is 200mS
uint8_t check_the_ble_baud(void)
{
	uint8_t send_times = 0;
//	uint8_t baud_tag = 0;
	uint8_t r_baud[]="AT+NAME\r\n"; // set the ble is host
	uint8_t name_check = 0;
//	LL_USART_Disable(USART1);
//	Usart11_Init(115200);	
//	Green_led_onORoff(1);
//	time_delay_ms(400);
	while(1)
	{
		Feed_IWDG();
		if(return_RX_Complete())
		{
			Usart_GetData_From_BLE(get_buffer);

			if(strncmp("720Yun-1", (const char *)&get_buffer[6],string_lengt("720Yun-1")) == 0) // Search to device
			{
			//	baud_tag = 8;
				name_check = 8;
				break;
				
			}
			else
			{
				name_check = 88;
			}
		}
		if(boot_time == 0)
		{
			boot_time = 200;
			usart_set_the_data(r_baud, string_lengt(r_baud));
			if(send_times >= 3)
			{
				send_times = 0;
				break;
			}
			send_times += 1;
		}
	}
/*
	boot_time = 0;
	send_times = 0;

	if(name_check == 0)
	{
		name_check = 0;
		LL_USART_Disable(USART1);
		Usart11_Init(115200);	
		time_delay_ms(400);
		while(1)
		{
			Feed_IWDG();
			Usart_GetData_From_BLE(get_buffer);

			if(strncmp("ZY-E10", (const char *)&get_buffer[6],string_lengt("ZY-E10")) == 0) // Search to device
			{
				//baud_tag = 4;
				name_check = 4;
				break;
			}
			
			if(boot_time == 0)
			{
				boot_time = 200;
				usart_set_the_data(r_baud, string_lengt(r_baud));
				if(send_times >= 3)
				{
					send_times = 0;
					break;
				}
				send_times += 1;
			}
		}

		
	}
*/
	return name_check;
}

void Ble_init(void)
{
	uint8_t send_times = 0;
	uint8_t name_b[]="AT+NAME720Yun-1\r\n";//set the ble is host
	//uint8_t broInt_b[]="AT+ADVIN2\r\n"; //set the Broadcast interval is 200mS
	//uint8_t Role_b[]="AT+ROLE0\r\n"; // set the ble is host
	uint8_t Baud_b[]="AT+BAUD4\r\n"; // set the baud is 115200
	uint8_t Reset_b[]="AT+RESET\r\n"; // reset the device
	uint8_t temp = 0;

	temp = check_the_ble_baud();
	
	if(temp != 8) // first time to use
	{
		if(temp != 88)
		{
			LL_USART_Disable(USART1);
			Usart11_Init(115200);
			time_delay_ms(400);
			temp  = 0xaa;
		}
		
		while(1)
		{
			Feed_IWDG();
			Usart_GetData_From_BLE(get_buffer);

			if(strncmp("720Yun-1", (const char *)&get_buffer[6],string_lengt("720Yun-1")) == 0) // Search to device
			{
			//	set_tag = 1;
				String_Printf(USART_2, get_buffer, string_lengt(get_buffer));
				
				memset(get_buffer, 0, sizeof(get_buffer));// clear the get_buffer
				
			}
			else if(strncmp("OK", (const char *)&get_buffer[0],string_lengt("OK")) == 0) // Search to device
			{
				String_Printf(USART_2, get_buffer, string_lengt(get_buffer));
				memset(get_buffer, 0, sizeof(get_buffer));// clear the get_buffer
				break;
			}
			
			
			if(boot_time == 0)
			{
				boot_time = 200;
				usart_set_the_data(name_b, string_lengt(name_b));
				if(send_times >= 3)
				{
					send_times = 0;
					break;
				}
				send_times += 1;
			}
		}

		memset(get_buffer, 0, sizeof(get_buffer));// clear the get_buffer
		boot_time = 0;
		send_times = 0;
		/*
		while(1)
		{
			Feed_IWDG();
			Usart_GetData_From_BLE(get_buffer);

			if(strncmp("OK", (const char *)&get_buffer[0],string_lengt("OK")) == 0) // Search to device
			{
				String_Printf(USART_2, get_buffer, string_lengt(get_buffer));
				memset(get_buffer, 0, sizeof(get_buffer));// clear the get_buffer
				break;
			}
			
			if(boot_time == 0)
			{
				boot_time = 200;
				usart_set_the_data(Role_b, string_lengt(Role_b));
				if(send_times >= 3)
				{
					send_times = 0;
					break;
				}
				send_times += 1;
			}
		}
		*/
		boot_time = 0;
		send_times = 0;

		while(1)
		{
			Usart_GetData_From_BLE(get_buffer);

			if(strncmp("OK", (const char *)&get_buffer[0],string_lengt("OK")) == 0) // Search to device
			{
				String_Printf(USART_2, get_buffer, string_lengt(get_buffer));
				memset(get_buffer, 0, sizeof(get_buffer));// clear the get_buffer
				break;
			}
			
			if(boot_time == 0)
			{
				boot_time = 200;
				usart_set_the_data(Baud_b, string_lengt(Baud_b));
				if(send_times >= 3)
				{
					send_times = 0;
					break;
				}
				send_times += 1;
			}
		}

		
		boot_time = 0;
		send_times = 0;
			
		usart_set_the_data(Reset_b, string_lengt(Reset_b));
		//if_write_flash();
		//write_flash_active();
		
		if(temp  == 0xaa)
		{
			LL_USART_Disable(USART1);
			Usart11_Init(9600);
			time_delay_ms(400);
		}
		//BLE_PWR_OFF; // ble pwr off
		//time_delay_ms(100);
		//BLE_PWR_ON;
		reset_usart_RX();
	}
	
}

// pairing
void scan_device(void)
{
/*	static uint8_t scan_times = 0;
	uint8_t con_flag = 0;
	uint8_t scan_b[]="AT+INQ\r\n";
	uint8_t clear_band[]="AT+CLRBAND\r\n";
	uint8_t power0_b[]="AT+POWR0\r\n";
	uint8_t power4_b[]="AT+POWR4\r\n";
	uint8_t mac_con[25]={0};

//	LED_ON;
	ble_wkup();
	time_delay_ms(5);

	
	usart_set_the_data(power0_b, string_lengt(power0_b)); //Set the power of ble(-14db)
	time_delay_ms(100);
	//reset_usart_RX();

//	usart_set_the_data(clear_band, string_lengt(clear_band)); //Cancel the mac to Dependend
//	delay_ms(15000);
	reset_usart_RX();
	while(1)
	{
		Usart_GetData_From_BLE(get_buffer);
		if(strncmp("OK", (const char *)&get_buffer[0],4) == 0) // Search to device
		{
			memset(get_buffer, 0, sizeof(get_buffer));// clear the get_buffer
			//delay_ms(5000);
			break;
		}

		if(boot_time == 0)
		{
			boot_time = 200;
			if(scan_times >= 5)break;
			usart_set_the_data(clear_band, string_lengt(clear_band)); //Cancel the mac to Dependend
			scan_times += 1;
		}
	}

	scan_times = 0;
	
	usart_set_the_data(scan_b, string_lengt(scan_b));
	boot_time = 5000;
	while(1)
	{
		Usart_GetData_From_BLE(get_buffer);
		if(strncmp("ZY-C400", (const char *)&get_buffer[24],string_lengt("ZY-C400")) == 0) // Search to device
		{
			memcpy(MAC_addr, &get_buffer[7], 12); // load mac of ble device
			sprintf( (char *)mac_con,"AT+BAND%s\r\n", MAC_addr); // Dependend connection
			con_flag = 1;
			String_Printf(mac_con, string_lengt(mac_con));
			memset(get_buffer, 0, sizeof(get_buffer));// clear the get_buffer
			//String_Printf(MAC_addr, string_lengt(MAC_addr));
		//	break;
		}
		else if(strncmp("+INQE", (const char *)&get_buffer[0],5) == 0)
		{
			memset(get_buffer, 0, sizeof(get_buffer));// clear the get_buffer
			if(con_flag == 0)
			{	
				if(scan_times >= 3)break;
				scan_times += 1;
				usart_set_the_data(scan_b, string_lengt(scan_b));
			}
			else
			{
				break;
			}
		}
		else
		{
		
		}

		
		memset(get_buffer, 0, sizeof(get_buffer));// clear the get_buffer
		if(boot_time == 0)break;

	}

	usart_set_the_data(power4_b, string_lengt(power4_b)); //Set the power of ble(4db)
	time_delay_ms(100);
	reset_usart_RX();
	if(con_flag)
	{
		boot_time = 2000;
		usart_set_the_data(mac_con, string_lengt(mac_con)); // Connect with ble device
		while(1)
		{
			Usart_GetData_From_BLE(get_buffer);
			if(strncmp("+CONNECTED>>0x203137560724", (const char *)&get_buffer[0],26) == 0)
			{
				String_Printf(get_buffer, string_lengt(get_buffer));
				con_flag = 2;
				break;
			}
			else if(strncmp("+DISCONNECT", (const char *)&get_buffer[0],11) == 0)
			{
				String_Printf(get_buffer, string_lengt(get_buffer));
				
			//	usart_set_the_data(clear_band, string_lengt(clear_band)); // Connect with ble device
				break;
			}

			if(boot_time == 0)break;
		}

		if(con_flag == 2)
		{
			time_delay_ms(3000);
		}
	}*/
	
}

// init to pair ble device
void Key_To_ble_Pair(void)
{
/*	if((KeyLeft == 0) && (KeyRight == 0))
	{
		time_delay_ms(40);
		if((KeyLeft == 0) && (KeyRight == 0))
		{
			scan_device();
		}
	}*/
}

void ble_device_enter_sleep(uint8_t modes)
{
/*//	uint8_t discon_b[]="AT+DISC\r\n";
	uint8_t sleep_b[]="AT+SLEEP\r\n";
	uint8_t clear_band[]="AT+CLRBAND\r\n";

	if(modes)
	{
		time_delay_ms(200);
		usart_set_the_data(clear_band, string_lengt(clear_band)); //disconnected()
		time_delay_ms(200);
		reset_usart_RX();
		//usart_set_the_data(discon_b, string_lengt(discon_b)); //disconnected()
		//delay_ms(100);
		time_delay_ms(200);
		usart_set_the_data(sleep_b, string_lengt(sleep_b)); //enter deep sleep()
		time_delay_ms(200);
		reset_usart_RX();
		//BLE_WK_ON;
	}
	else
	{
		wk_tag = 202;
		//reset_usart_RX();
		//delay_ms(1000);
		//BLE_WK_ON;
		
	//	delay_ms(200);
///		sprintf( (char *)mac_con,"AT+BAND%s\r\n", MAC_addr); // Dependend connection
	//	String_Printf(mac_con, string_lengt(mac_con));
///		usart_set_the_data(mac_con, string_lengt(mac_con)); // Connect with ble device
///		delay_ms(200);
////		reset_usart_RX();
///		BLE_WK_ON;
	}*/
}

void ble_wk_event(void)
{
/*	uint8_t mac_con[25]={0};
	
	if(wk_tag == 2)
	{
		wk_tag = 1;
		//BLE_WK_ON;
			sprintf( (char *)mac_con,"AT+BAND%s\r\n", MAC_addr); // Dependend connection
			//String_Printf(mac_con, string_lengt(mac_con));
			usart_set_the_data(mac_con, string_lengt(mac_con)); // Connect with ble device
	}*/
}

void ble_wkup(void)
{
//	wk_tag = 0;
//	BLE_WK_ON;
}

void ble_ount(void)
{
//	if(wk_tag > 2)wk_tag--;
}

uint8_t return_wk_tag(void)
{
	return wk_tag;
}

void battery_send_test(float baaa)
{
	uint8_t mac_con[20]={0};
	sprintf( (char *)mac_con,"%f\r\n", baaa); // Dependend connection
	String_Printf(USART_2, mac_con, string_lengt(mac_con));
}

