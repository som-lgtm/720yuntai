
#include "common.h"
#include "uart.h"
#include "string.h"

uint8_t boot_action = 0;

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

void enter_bootloader(uint8_t tag)
{
		__disable_irq();
		write_bootTag(tag);
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
											String_Printf("G", 1);
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
		String_Printf("G", 1);
		return;
	}

	for(i=1; i<20 && spt[i] != 0;i++)
	{
		name[i-1] = spt[i];
	}

	name[i-1] = '\0';
	
	
	FileSize = spt[i+1] | (uint16_t)(spt[i+2])<<8;
	
	if(strncmp("LG-06-CTR", (const char *)&name,string_lengt("LG-06-CTR")) == 0) // Search to device
	{
		if(FileSize <= 0xE000)
		{
			boot_action = 0;
			enter_bootloader(0xAA);
		}
		else
		{
			boot_action = 0;
			String_Printf("F", 1);
		}
	}
	else
	{
		boot_action = 0;
		String_Printf("N", 1);
	}

}


