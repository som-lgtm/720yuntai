/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : download.c
* Author             : MCD Application Team
* Date First Issued  : 05/21/2007
* Description        : This file provides the software which allows to download
*                      an image to internal Flash.
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
#include "common.h"
#include "gpio.h"
#include "ymodem.h" 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


u8 tab_1024[1024]={0};
u8 get_name_tag = 0;

void lights_flash(void)
{
	static uint8_t led_tag = 0;
	led_tag = ~led_tag;
	if(led_tag)
	{
		RED_LED_ON;
		LED_OFF;
	}
	else
	{
		RED_LED_OFF;
		LED_ON;
	}
}


void JomToApp(void)
{
	IAP_INF_T infor={0};
	
	//if(FROM_APP == 0)iap_flash_write_iap_inf(0x48);
	read_flash_holfword(FLASH_WRITE_START_ADDR, (uint16_t *)&infor, sizeof(infor));
	if(FROM_APP == 0)infor.ISR_tag = 0x48;
	infor.usart_number = 0; // Select flag for online upgrade serial port, 0xaa is the usart 1; 0xbb is the usart 2
	write_flash_holfword_buffer((uint16_t *)&infor, sizeof(infor));// clear flag and save
	 __disable_irq();	 
	 JumpAddress = *(vu32*) (ApplicationAddress + 4);
	 Jump_To_Application = (pFunction) JumpAddress;
	// MSR_MSP(*(u32*)JumpAddress);
	  __set_MSP(*(volatile unsigned int*) ApplicationAddress);
	 Jump_To_Application();

}
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : SerialDownload
* Description    : Download a file via serial port
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SerialDownload(void)
{
//  u8 Number[10] = "          ";
  s32 Size = 0;
	u8 statu = 0;
	static u16 test= 0;

	FlashDestination = ApplicationAddress;
//  SerialPutString("Waiting for the file to be sent ... (press 'a' to abort)\n\r");
	if(select_usart == 0xaa || select_usart == 0xbb)
	{
		if(select_usart == 0xaa)OTA_Ack(6, 0);
		Size = Ymodem_Receive(&tab_1024[0]);
	}
	
	if (Size > 0)
	{
		//RED_LED_OFF;
		//LED_ON;
		//if(select_usart == 0xbb)Send_Byte(0x53); // to tell app udata succueed
		JomToApp();
	}
	else // wait for updata again
	{
		if(Size == -2)OTA_Ack(6, 4);
		Size = 0;
		select_usart = 0xaa;
		//Send_Byte(0x18); // updata fail
		get_name_tag = 0;

		while(1)
		{
			test += 1;
			if(test == 6000)
			{
				test = 0;
				lights_flash();
				//if(force_updata == 0xcc)OTA_Ack(6, 5);
			}
			statu = Handshake_Get();
			if(statu == 1)break;
			else if(statu == 2)continue;
			if(key_check())if(force_updata == 0xcc)NVIC_SystemReset();
			/*else if(statu == 0)
			{
				if(select_usart == 0xaa)select_usart = 0xbb;
				else if(select_usart == 0xbb)select_usart = 0xaa;
			}*/
		}
	}
/*  else if (Size == -1)
  {
    SerialPutString("\n\n\rThe image size is higher than the allowed space memory!\n\r");
  }
  else if (Size == -2)
  {
    SerialPutString("\n\n\rVerification failed!\n\r");
  }
  else if (Size == -3)
  {
    SerialPutString("\r\n\nAborted by user.\n\r");
  }
  else
  {
    SerialPutString("\n\rFailed to receive the file!\n\r");
  }*/
}

// opcode:  value 0 is acking the version of the firmware
// value 6 is acking the decoding results :
// data-> 0:send the next page data
//	    -> 1: resend current page data
//      -> 2: Resend from the first packet
//      -> 3: Receive complete
//      -> 4: Cancel the entire send operation
void OTA_Ack(uint8_t opcode, uint8_t data)
{
	uint8_t ack_bufer[20] = {0};
	IAP_INF_T infor={0};
		
	
	if(opcode == 0)
	{
		//read_flash_holfword(FLASH_WRITE_START_ADDR, (uint16_t *)&infor, sizeof(infor));
		ack_bufer[0] = 0x78;
		ack_bufer[1] = opcode;
		ack_bufer[2] = 9;
		ack_bufer[3] = 0x02;
		ack_bufer[4] = 0x02;
		ack_bufer[5] = 1; //infor.firmware_version / 100;
		ack_bufer[6] = 0; //infor.firmware_version / 10 % 10;
		ack_bufer[7] = 0; //infor.firmware_version % 10;
		ack_bufer[8] = 0;
		ack_bufer[9] = 0x45;
		ack_bufer[10] = 0x52;
		ack_bufer[11] = 0x31;
		ack_bufer[12] = check_sum_add(ack_bufer, 12);
		
		UsartSendBuff(ack_bufer, 13);
	}
	else if(opcode == 0x06)
	{
		ack_bufer[0] = 0x78;
		ack_bufer[1] = opcode;
		ack_bufer[2] = 1;
		ack_bufer[3] = data;
		ack_bufer[4] = check_sum_add(ack_bufer, 4);
		
		UsartSendBuff(ack_bufer, 5);
	}
	else if(opcode == 0x05)
	{
		
	}
		
}

uint8_t Check_Firmware(uint8_t *spt)
{
	uint8_t name[32] = {0};
	uint32_t FileSize = 0;
//	uint8_t i;

	FileSize = ((uint32_t)spt[6]<<24) + ((uint32_t)spt[7]<<16) + ((uint32_t)spt[8]<<8) + spt[9];
	memcpy(name, &spt[14], 32); // load name of firmware
	fw_checksum = ((uint32_t)spt[10]<<24) + ((uint32_t)spt[11]<<16) + ((uint32_t)spt[12]<<8) + spt[13];

	
//	if(strncmp("Tank 3", (const char *)&name,4) == 0) // Search to device
	{
		if(FileSize <= ErasePage_SIZE)
		{
			//boot_action = 0;
		//	OTA_Ack(0x06, 0x00); 
			fw_sizes = FileSize;
			NbrOfPage = FLASH_PagesMask(fw_sizes);
			//enter_bootloader(0xAA, FileSize);
			return 1;
		}
		else
		{
		//	boot_action = 0;
			OTA_Ack(0x06, 0x05); // 超出容量，消取升级
		}
	}
//	else
//	{
		//boot_action = 0;
//		OTA_Ack(0x06, 0x04); // 固件错误，取消升级
		
		//UsartSendBuff(spt, 18);
//	}

	return 0;
}

s32 Read_Usart(u8 c, u32 timeout)
{
  while (timeout-- > 0)
  {
    if(LL_USART_IsActiveFlag_RXNE(USART1))
    {
		c = LL_USART_ReceiveData8(USART1);
		select_usart = 0xaa;
		return (s32)c;
    }
    else if(LL_USART_IsActiveFlag_RXNE(USART2))
    {
		c = LL_USART_ReceiveData8(USART2);
		select_usart = 0xbb;
		return (s32)c;
    }
  }
  return -1;
}

u8 Handshake_Get(void)
{
	uint8_t bufer[50]={0};
	uint8_t i;
	s32 data_type=0;

	data_type = Read_Usart(0,100);
	if(data_type >=0)
	{
		if(data_type == 0x90)
		{
			for (i = 1; i < 8; i ++)
			{
			  if (Receive_Byte(&bufer[i], TIMEOUT) != 0)
			  {
					break;
			  }
			}
			
			if(bufer[1] == 0x04) // ack for version to app
			{
				OTA_Ack(6, 0);
				return 2;
			}
		}
		
		if(data_type == 0x03)
		{
			for (i = 1; i < 10; i ++)
			{
			  if (Receive_Byte(&bufer[i], TIMEOUT) != 0)
			  {
					break;
			  }
			}

			if(bufer[1] == 0x02 && bufer[2] == 0x03)
			{
				Send_Byte(0x47); // get firmware name and sizes from app
				get_name_tag = 1;
				return 2;
			}
		}

		if(data_type == 0x78) // app send name of firmware to motor
		{
			for (i = 1; i < 3; i ++)
			{
			  if (Receive_Byte(&bufer[i], TIMEOUT) != 0)
			  {
			  	break;
			  }
			}

			if(bufer[1] == 0x80) // ack for version to app
			{
				for (i = 3; i < bufer[2]+1+3; i ++) // 3：是起始读数地址
				{
				  if (Receive_Byte(&bufer[i], TIMEOUT) != 0)
				  {
					break;
				  }
				}
				OTA_Ack(0, 0);
				return 2;
			}
			else if(bufer[1] == 0x85)
			{
				return 2;
			}
			else if(bufer[1] == 0x96)
			{
				for (i = 3; i < bufer[2]+1+3; i ++) // 3：是起始读数地址
				{
				  if (Receive_Byte(&bufer[i], TIMEOUT) != 0)
				  {
					break;
				  }
				}

				if(Check_Firmware(bufer))return 1;
				else return 0;
			}
		}

		if(data_type == 0x4e) // get name and sizes of firmware from usb updata 
		{
			if(get_name_tag == 0)return 2;
			for (i = 1; i < 132; i ++)
			{
			  if (Receive_Byte(&bufer[i], TIMEOUT) != 0)
			  {
			  	break;
			  }
			}

			if(NameAndSize_Check(bufer) == 1)return 1;
			else return 0;
		}
	}

	return 0;
}

u8 NameAndSize_Check(uint8_t *spt)
{
	u8 name[20] = {0};
	u16 FileSize = 0;
	u8 i;

	for(i=1; i<20 && spt[i] != 0;i++)
	{
		name[i-1] = spt[i];
	}

	name[i-1] = '\0';
	
	FileSize = spt[i+1] | (u16)(spt[i+2])<<8;
	
//	if(strncmp("DL200", (const char *)&name,4) == 0) // Search to device
	{
		if(FileSize <= ErasePage_SIZE)
		{
			if(spt[0] == 0x4f)
			{
				fw_sizes = FileSize;
				NbrOfPage = FLASH_PagesMask(fw_sizes);
				
				FW_ErasePage(NbrOfPage);
			}
			
			Send_Byte(0x44); // enter bootloader
			return 1;
		}
		else
		{
			Send_Byte(0x46); // Package size overflow
		}
	}
//	else
//	{
//		Send_Byte(0x4e); // name error
//	}

	return 0;
}

/*******************(C)COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
