/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : ymodem.c
* Author             : MCD Application Team
* Date First Issued  : 05/21/2007
* Description        : This file provides all the software functions related to
*                      the ymodem protocol.
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
#include "ymodem.h" 
#include "gpio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 file_name[FILE_NAME_LENGTH]={0};
u32 FlashDestination = 0; /* Flash user program offset */
u16 PageSize = 0;
u32 EraseCounter = 0x0;
u32 NbrOfPage = 0;
FLASH_Status FLASHStatus;
u32 RamSource=0;
u32 fw_sizes = 0x00;
u32 fw_checksum = 0x00;
u8 ffile_size[FILE_SIZE_LENGTH]={0};
extern u8 tab_1024[1024];

void para_init(void)
{
	FlashDestination = ApplicationAddress; /* Flash user program offset */
	PageSize = 0x400;
	FLASHStatus = FLASH_COMPLETE;
}
/*u8 Reverse8Bit(u8 data)
{
	u8 i;
	u8 temp = 0;
	for(i=0; i<8; i++)
	{
		temp |= ((data>>i) & 0x01)<<(7-i);
	}

	return temp;
}

u16 Reverse16Bit(u8 data)
{
	u8 i;
	u16 temp = 0;
	for(i=0; i<16; i++)
	{
		temp |= ((data>>i) & 0x0001)<<(15-i);
	}

	return temp;
}


// CRC-16/IBM 检验法
u16 CRC_check(u8 *spt, u8 sizes)
{
	u16 crc_xor = 0x0000;// 结果异或值
    u16 crc_value = 0x0000; // 初始值
    u16 polynomial = 0x8005;// 多项式
    u16 bit15_check = 0x8000; // 检测左移位的数是否为1
    u16 i , j;
    u16 temp;
	u8 reverse_value = 0;
    for (i=0; i<sizes; i++)
    {
    	reverse_value = Reverse8Bit(spt[i]); // 输入值反转
       // crc_value |= ((crc_value>>8) ^ reverse_value)<<8; // 将CRC的高8位与输入反转后的数据相异或，低8位不变，之后存入CRC变量中
		crc_value = crc_value ^ ((u16)reverse_value<<8); // 将CRC的高8位与输入反转后的数据相异或，低8位不变，之后存入CRC变量中
        for(j=0; j<8; j++)
        {
        	if(crc_value & bit15_check)// 检测左移位的数是否为1
        	{
        		crc_value = (crc_value << 1) ^ polynomial;
        	}
			else
			{
				crc_value <<= 1;
			}
        }
    }

	crc_value = Reverse16Bit(crc_value);
	crc_value ^= crc_xor; // crc_xor 是结果异或值，固定的值
    return crc_value;
}
*/
// CRC-16/IBM 检验法(简易写法)
u16 CRC16_IBM_Check(u8 *spt, u8 sizes)
{
    u16 crc_value = 0x0000; // 初始值
    u16 polynomial = 0xA001;// 多项式
    u16 i , j;
//    u16 temp;
	u8 reverse_value = 0;
    for (i=0; i<sizes; i++)
    {
    	reverse_value = spt[i]; 
		crc_value = crc_value ^ reverse_value; 
        for(j=0; j<8; j++)
        {
        	if(crc_value & 0x0001)// 检测右移位的数是否为1
        	{
        		crc_value = (crc_value >> 1) ^ polynomial;
        	}
			else
			{
				crc_value >>= 1;
			}
        }
    }

    return crc_value;
}

uint8_t check_sum_add(uint8_t *spt, uint8_t index)
{
	uint8_t i;
	uint32_t temp=0;

	spt[index] = 0;
	
	for(i=0;i<index; i++)
	{
		temp += spt[i];
	}

	//uart1_s_buffer[0] = temp & 0xff;
	//display_test11(uart1_s_buffer[0]);

	return (temp & 0xff);
}

uint8_t checksum_verify(uint8_t *spt, uint8_t lenght)
{
	uint8_t i;
	uint16_t temp =0;
	uint8_t checksum =0;
	
	for(i=0; i< lenght; i++)
	{
		temp += (uint16_t)spt[i];
	}

	checksum = temp;
	
	if(checksum != spt[lenght])
	{
		return 0;
	}

	return 1;
}

uint8_t Bit32_checksum_verify(uint8_t *spt, uint32_t lenght)
{
	uint32_t i;
	uint32_t temp =0;
	//uint8_t checksum =0;
	
	for(i=0; i< lenght; i++)
	{
		temp += spt[i];
	}

	if(temp != fw_checksum)
	{
		return 0;
	}

	return 1;
}

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Receive_Byte
* Description    : Receive byte from sender
* Input          : - c: Character
*                  - timeout: Timeout
* Output         : None
* Return         : 0: Byte received
*                  -1: Timeout
*******************************************************************************/
s32 Receive_Byte (u8 *c, u32 timeout)
{
  while (timeout-- > 0)
  {
    if (SerialKeyPressed(c) == 1)
    {
      return 0;
    }
  }
  return -1;
}

void time_delay_us(uint32_t tttt)
{
	uint8_t i=0;
	uint32_t j=0;
	for(i=0; i<100; i++)
	{
		for(j=0; j<tttt; j++)
			{}
	}
}

/*******************************************************************************
* Function Name  : Send_Byte
* Description    : Send a byte
* Input          : - c: Character
* Output         : None
* Return         : 0: Byte sent
*******************************************************************************/
u32 Send_Byte (u8 c)
{
//	time_delay_us(10000);

  SerialPutChar(c);
  return 0;
}

/*******************************************************************************
* Function Name  : Receive_Packet
* Description    : Receive a packet from sender
* Input 1        : - data
* Input 2        : - length
* Input 3        : - timeout
* Output         : *length:
*                  0: end of transmission
*                  -1: abort by sender
*                  >0: packet length
* Return         : 0: normally return
*                  -1: timeout or packet error
*                  1: abort by user
*******************************************************************************/
static s32 Receive_Packet (u8 *data, s32 *length, u32 timeout)
{
  u16 i, packet_size;
  u8 c;
//  u8 *crc_value;
  *length = 0;
  if (Receive_Byte(&c, timeout) != 0)
  {
	//  LED_ON;
    return -1;
  }
  switch (c)
  {
  case SOH:
  	//LED_OFF;
    packet_size = PACKET_SIZE;
    break;
  case STX:
    packet_size = PACKET_1K_SIZE;
    break;
  case APP:
  	packet_size = PACKET_SIZE;
  	break;
  case EOT:
    return 0;
  case APP_END:
  	return -3;
  default:
    return -1;
  }
  
//  crc_value = data;

  *data = c;
  
  if(data[0] == 0x78)
  {
  	  for(i = 1; i<3; i++)
  	  {
		  if (Receive_Byte(data + i, timeout) != 0)
		  {
			return -1;
		  }
  	  }
	  
	  for (i = 3; i < (data[2] + PACKET_HEADER+1); i ++)
	  {
		if (Receive_Byte(data + i, timeout) != 0)
		{
		  return -1;
		}
	  }
  }
  else
  {
	  for (i = 1; i < (packet_size + PACKET_OVERHEAD); i ++)
	  {
		if (Receive_Byte(data + i, timeout) != 0)
		{
		  return -1;
		}
	  }
  }

	if(data[0] == 0x78)
	{
	 // if(CRC16_IBM_Check(&data[0], packet_size+PACKET_HEADER) != ((u16)crc_value[packet_size+PACKET_HEADER] | (u16)crc_value[packet_size+PACKET_HEADER+1]<<8))
	  if(checksum_verify(&data[0], data[2] + PACKET_HEADER) == 0)
	  {
		  //OTA_Ack(0x06, 1);
		  return -2;
	  }
	}
	else
	{
		if (data[PACKET_SEQNO_INDEX] != ((data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff))
		{
		  return -1;
		}
	}
	
  *length = packet_size;
  return 0;
}
/*******************************************************************************
* Function Name  : Ymodem_Receive
* Description    : Receive a file using the ymodem protocol
* Input          : Address of the first byte
* Output         : None
* Return         : The size of the file
*******************************************************************************/
s32 Ymodem_Receive (u8 *buf)
{
  u8 packet_data[PACKET_1K_SIZE + PACKET_OVERHEAD]={0}, *file_ptr, *buf_ptr;
  s32 i, packet_length, session_done, file_done, packets_received, errors, session_begin, size = 0;

  for (session_done = 0, errors = 0, session_begin = 0; ;)
  {
    for (packets_received = 0, file_done = 0, buf_ptr = buf; ;)
    {
    	lights_flash();
      switch (Receive_Packet(packet_data, &packet_length, NAK_TIMEOUT))
      {
      	case -3: // APP end of transmistion and enter app program 
		{
			
			return 1;
		}
      	case -2: // CRC ERROR
		{
			force_updata = 0xcc;
			//UsartSendBuff(packet_data, 4);
			OTA_Ack(0x06, 1);
			//Upgrade_failure_ErasePage();
			break;
		}
        case 0:
          errors = 0;
          switch (packet_length)
          {
            /* Abort by sender */
			case - 1:    
              Send_Byte(ACK);
		//	  LED_ON;
			Upgrade_failure_ErasePage();
              return 0;
			/* End of transmission */
            case 0:     
              Send_Byte(ACK);
              file_done = 1;
		//	  RED_LED_ON;
              break;
			/* Normal packet */
            default:    
            if(packet_data[0] == 0x78)
            {
            	if(session_begin == 0)
            	{
					FW_ErasePage(NbrOfPage);
					//FLASH_ErasePage(0x800FC00); // CLEAR the custom paramater of app program
            	}
				/*
				if(NbrOfPage)
				{
					FW_ErasePage(NbrOfPage);
					NbrOfPage = 0;
				}*/
				
				memcpy(buf_ptr, packet_data + PACKET_HEADER, packet_length);
				
				RamSource =(u32)buf;
				if(fw_updata(packet_length) != 0)
				{
					Upgrade_failure_ErasePage();
					return -2;
				}
				packet_data[3] = 0;
				//UsartSendBuff(packet_data, 4);
				session_begin = 1;
				packets_received += packet_data[2];
				
				if(packets_received >= fw_sizes)
				{
					if(Bit32_checksum_verify((u8 *)ApplicationAddress, fw_sizes) == 0) // Firmware package error
					{
						OTA_Ack(0x06, 4);
						force_updata = 0xcc;
						Upgrade_failure_ErasePage();
						return 0;
					}
					else // updata secceed
					{
						OTA_Ack(0x06, 3);
						//size = packets_received;
						//FLASH_ErasePage(0x800FC00); // CLEAR the custom paramater of app program
						return packets_received;
					}
				}
				OTA_Ack(0x06, 0);
				//continue;
            }
			else
            {
	            if ((packet_data[PACKET_SEQNO_INDEX] & 0xff) != (packets_received & 0xff))
	            {
	              Send_Byte(NAK);
	            }
	            else
	            {
	              if (packets_received == 0)   
	              {/* Filename packet */
	                if (packet_data[PACKET_HEADER] != 0)    
	                {/* Filename packet has valid data */
	                 for (i = 0, file_ptr = packet_data + PACKET_HEADER; (*file_ptr != 0) && (i < FILE_NAME_LENGTH);)
	                  {
	                    file_name[i++] = *file_ptr++;
	                  }
	                  file_name[i] = '\0';
	                  for (i = 0, file_ptr++; /*(*file_ptr != 0) &&*/ (i < FILE_SIZE_LENGTH-1);)
	                  {
	                  	if((*file_ptr) == ' ')break;
						if((*file_ptr) == 0)break;
	                  //  file_size[i++] = *file_ptr++;
	                    ffile_size[i++] = *file_ptr++;
	                  }
	                  ffile_size[i] = '\0';
	                  Str2Int(ffile_size, &size);

	                  /* Test the size of the image to be sent */
	                  /* Image size is greater than Flash size */
					  if (size > ErasePage_SIZE) 
	                  {
	                    /* End session */
	                    Send_Byte(CA);
	                    Send_Byte(CA);
	                    return -1;
	                  }
					  fw_sizes = size;
	                  /* Erase the needed pages where the user application will be loaded */
	                  /* Define the number of page to be erased */
	                  NbrOfPage = FLASH_PagesMask(size);
	                  /* Erase the FLASH pages */
	                 // for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
	                 // {
	                 //   FLASHStatus = FLASH_ErasePage(FlashDestination + (PageSize * EraseCounter));
	                //  }
					  FW_ErasePage(NbrOfPage);
					  
					  //FLASH_ErasePage(0x800FC00); // CLEAR the custom paramater of app program
	                  Send_Byte(ACK);
	                  Send_Byte(CRC16);
	                }
					/* Filename packet is empty, end session */
	                else   
	                {
	                  Send_Byte(ACK);
	                  file_done = 1;
	                  session_done = 1;
	                  break;
	                }
	              }
				  /* Data packet */
	              else    
	              {
	                memcpy(buf_ptr, packet_data + PACKET_HEADER, packet_length);
	                RamSource =(u32)buf;
					if(fw_updata(packet_length) != 0)
					{
						Upgrade_failure_ErasePage();
					return -2;
					}
	                Send_Byte(ACK);
	              }
	              packets_received ++;
	              session_begin = 1;
	            }
            }
          }
          break;
          case 1:
            Send_Byte(CA);
            Send_Byte(CA);
			Upgrade_failure_ErasePage();
            return -3;
          default:
        //  if (session_begin > 0)
          {
            errors ++;
          }
          if (errors > MAX_ERRORS)
          {
						Upgrade_failure_ErasePage();
          	if(select_usart == 0xbb)
          	{
            	Send_Byte(CA);
            	Send_Byte(CA);
          	}
			
			if(select_usart == 0xaa)
			{
				OTA_Ack(0x06, 4); // 接收超时，取消整个升级任务
			}
			
			
			force_updata = 0xcc;
            return 0;
          }
          if(select_usart == 0xbb)Send_Byte(CRC16);
		  
		  if(select_usart == 0xaa)OTA_Ack(0x06, 1); // 重发当前数据包
		  // 超时处理
		  
          break;
      }
      if (file_done != 0)
      {
        break;
      }
    }
    if (session_done != 0)
    {
      break;
    }
  }
  return (s32)size;
}

s8 fw_updata(s32 packet_length)
{
	u8 j;
	for (j = 0;(j < packet_length) && (FlashDestination <  ApplicationAddress + fw_sizes);j += 4)
	{
	  /* Program the data received into STM32F10x Flash */
	  FLASH_ProgramWord(FlashDestination, *(u32*)RamSource);
	  if (*(u32*)FlashDestination != *(u32*)RamSource)
	  {
		/* End session */
		Send_Byte(CA);
		Send_Byte(CA);
		return -2;
	  }
	  FlashDestination += 4;
	  RamSource += 4;
	}

	return 0;
}

void FW_ErasePage(u32 nofpage)
{
	for(EraseCounter = 0; (EraseCounter < nofpage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
	{
	  FLASHStatus = FLASH_ErasePage(FlashDestination + (PageSize * EraseCounter));
	}                                 
	
}
/*******************(C)COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/

//升级失败后擦除flash
void Upgrade_failure_ErasePage(void)
{
	IAP_INF_T infor={0};
	FlashDestination = ApplicationAddress;
	PageSize = 0X400;
	FW_ErasePage(ErasePage);
	
	read_flash_holfword(FLASH_WRITE_START_ADDR, (uint16_t *)&infor, sizeof(infor));
	if(infor.usart_number)
	{
		infor.usart_number = 0;
		write_flash_holfword_buffer((uint16_t *)&infor,sizeof(infor));
	}

	NVIC_SystemReset();
}

