
#include "common.h"
#include "uart.h"
#include "string.h"

uint8_t boot_action = 0;
uint32_t firmware_checksum =0;


uint8_t check_sum_add11(uint8_t index, uint8_t *spt)
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

uint32_t return_checksum(void)
{
	return firmware_checksum;
}

void set_firmsware_checksum(uint32_t checksum)
{
	firmware_checksum = checksum;
}

uint8_t string_lengt(uint8_t *strings)
{
	uint8_t i=0;
	while(*strings != '\0')
	{
		i += 1;
		strings++;
	}

	return i;
}

void enter_bootloader(uint8_t tag, uint16_t sizes)
{
		__disable_irq();
		write_bootTag(tag, sizes);
		NVIC_SystemReset();

}

void check_enter_boot_if(uint8_t *spt)
{
	if(spt[0] == 0x03)
	{
		if(spt[1] == 0x02)
		{
			if(spt[2] == 0x03)
			{
				if(spt[3] == 0x04)
				{
					if(spt[4] == 0x05)
					{
						if(spt[5] == 0x06)
						{
							if(spt[6] == 0x07)
							{
								if(spt[7] == 0x08)
								{
									if(spt[8] == 0x09)
									{
										if(spt[9] == 0x0a)
										{
											boot_action  = 1;
											String_Printf(USART_2,"G", 1);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void bootloader_action_if(uint8_t *spt)
{
	uint8_t name[20] = {0};
	uint16_t FileSize = 0;
	uint8_t i;
	if(boot_action == 0)return;
	if(spt[0] != 0x4e)
	{
		String_Printf(USART_2,"G", 1);
		return;
	}

	for(i=1; i<20 && spt[i] != 0;i++)
	{
		name[i-1] = spt[i];
	}

	name[i-1] = '\0';
	
	
	FileSize = spt[i+1] | (uint16_t)(spt[i+2])<<8;
	
	if(strncmp("LG-06", (const char *)&name,string_lengt("LG-06")) == 0) // Search to device
	{
		if(FileSize <= 0xD400)
		{
			boot_action = 0;
			enter_bootloader(0xBB, FileSize);
		}
		else
		{
			boot_action = 0;
			String_Printf(USART_2,"F", 1);
		}
	}
	else
	{
		boot_action = 0;
		String_Printf(USART_2,"N", 1);
	}

}

/////////////////  OTA ////////////////

void OTA_Ack(uint8_t opcode, uint8_t data)
{
	uint8_t ack_bufer[15] = {0};
	
	if(opcode == 0)
	{
		ack_bufer[0] = 0x78;
		ack_bufer[1] = opcode;
		ack_bufer[2] = 9;
		ack_bufer[3] = 0x02;
		ack_bufer[4] = 0x02;
		ack_bufer[5] = FW_VERSIONS / 100;
		ack_bufer[6] = FW_VERSIONS / 10 % 10;
		ack_bufer[7] = FW_VERSIONS % 10;		
		ack_bufer[8] = 0;
		ack_bufer[9] = 0x45;
		ack_bufer[10] = 0x52;
		ack_bufer[11] = 0x31;
		ack_bufer[12] = check_sum_add11(12, ack_bufer);
		
		String_Printf(USART_1, ack_bufer, 13);
	}
	else if(opcode == 0x06)
	{
		ack_bufer[0] = 0x78;
		ack_bufer[1] = opcode;
		ack_bufer[2] = 1;
		ack_bufer[3] = data;
		ack_bufer[4] = check_sum_add11(4, ack_bufer);
		
		String_Printf(USART_1, ack_bufer, 5);
	}
}

void Check_Firmware(uint8_t *spt)
{
	uint8_t name[32] = {0};
	uint32_t FileSize = 0;
	uint32_t checksum = 0;
//	uint8_t i;

	FileSize = ((uint32_t)spt[6]<<24) + ((uint32_t)spt[7]<<16) + ((uint32_t)spt[8]<<8) + spt[9];
	checksum = ((uint32_t)spt[10]<<24) + ((uint32_t)spt[11]<<16) + ((uint32_t)spt[12]<<8) + spt[13];
	memcpy(name, &spt[14], 32); // load name of firmware

	
	if(strncmp("LG-06", (const char *)&name,string_lengt("LG-06")) == 0) // Search to device
	{
		if(FileSize <= 0xD400)
		{
			//boot_action = 0;
		//	OTA_Ack(0x06, 0x02); // 从第一包数据重新发
			set_firmsware_checksum(checksum);
			enter_bootloader(0xAA, FileSize);
		}
		else
		{
			boot_action = 0;
			OTA_Ack(0x06, 0x04); // 超出容量，消取升级
			//enter_bootloader(0xAA, FileSize);
		}
	}
	else
	{
		boot_action = 0;
		OTA_Ack(0x06, 0x04); // 固件错误，取消升级
	}
}

void Enter_OTA_If(uint8_t *sspt)
{
	if(sspt[0] == 0x78)
	{
		if(sspt[1] == 0x80)
		{
			OTA_Ack(0, 0);
			//enter_bootloader(0xAA);
		}
		else if(sspt[1] == 0x96)
		{
			Check_Firmware(sspt);
		}
	}
}

void Read_versions(uint8_t *sspt)
{
	uint8_t ack_bufer[15] = {0};
	
	if(sspt[0] == 0x78)
	{
		if(sspt[1] == 0x80)
		{
			ack_bufer[0] = 0x78;
			ack_bufer[1] = 0x80;
			ack_bufer[2] = FW_VERSIONS / 100;
			ack_bufer[3] = FW_VERSIONS / 10 % 10;
			ack_bufer[4] = FW_VERSIONS % 10;		
			//ack_bufer[5] = check_sum_add(5, ack_bufer);
			
			String_Printf(USART_2, ack_bufer, 5);
		}
	}
}

