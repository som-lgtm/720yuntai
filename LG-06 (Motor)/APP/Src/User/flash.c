/**
  ******************************************************************************
  * @file    FLASH/FLASH_Program/main.c  
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "flash.h"

///////////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "stm32f030x8.h"
#include "string.h"
#include "UID_Encryption.h"


uint8_t whether_write_flash=0;

FLASH_Status FLASH_GetStatus(void)
{
  FLASH_Status FLASHstatus = FLASH_COMPLETE;
  
  if((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY) 
  {
    FLASHstatus = FLASH_BUSY;
  }
  else 
  {  
    if((FLASH->SR & (uint32_t)FLASH_FLAG_WRPERR)!= (uint32_t)0x00)
    { 
      FLASHstatus = FLASH_ERROR_WRP;
    }
    else 
    {
      if((FLASH->SR & (uint32_t)(FLASH_SR_PGERR)) != (uint32_t)0x00)
      {
        FLASHstatus = FLASH_ERROR_PROGRAM; 
      }
      else
      {
        FLASHstatus = FLASH_COMPLETE;
      }
    }
  }
  /* Return the FLASH Status */
  return FLASHstatus;
}

FLASH_Status FLASH_WaitForLastOperation(uint32_t Timeout)
{ 
  FLASH_Status status = FLASH_COMPLETE;
   
  /* Check for the FLASH Status */
  status = FLASH_GetStatus();
  
  /* Wait for a FLASH operation to complete or a TIMEOUT to occur */
  while((status == FLASH_BUSY) && (Timeout != 0x00))
  {
    status = FLASH_GetStatus();
    Timeout--;
  }
  
  if(Timeout == 0x00 )
  {
    status = FLASH_TIMEOUT;
  }
  /* Return the operation status */
  return status;
}


FLASH_Status FLASH_ErasePage(uint32_t Page_Address)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters */
 // assert_param(IS_FLASH_PROGRAM_ADDRESS(Page_Address));
 
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
  
  if(status == FLASH_COMPLETE)
  { 
    /* If the previous operation is completed, proceed to erase the page */
    FLASH->CR |= FLASH_CR_PER;
    FLASH->AR  = Page_Address;
    FLASH->CR |= FLASH_CR_STRT;
    
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
    
    /* Disable the PER Bit */
    FLASH->CR &= ~FLASH_CR_PER;
  }
    
  /* Return the Erase Status */
  return status;
}

void erase_flash_OnePage(uint32_t Page_Address)
{
	//FLASH_Unlock();
	
	/* Clear pending flags (if any) */	
//	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR); 
	
	if (FLASH_ErasePage(Page_Address)!= FLASH_COMPLETE)
    {
     /* Error occurred while sector erase. 
         User can add here some code to deal with this error  */
      while (1)
      {
      }
    }
}

/*
void write_flash_word(uint32_t Address, uint32_t data)
{
	//uint8_t result=0;
	
	if (FLASH_ProgramWord(Address, data) != FLASH_COMPLETE)
    {
    	while(1)
    	{}
    }

    //return 1;
}*/

FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters */
//  assert_param(IS_FLASH_PROGRAM_ADDRESS(Address));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
  
  if(status == FLASH_COMPLETE)
  {
    /* If the previous operation is completed, proceed to program the new data */
    FLASH->CR |= FLASH_CR_PG;
  
    *(__IO uint16_t*)Address = Data;
    __NOP();
    __NOP();
    __NOP();
    __NOP();

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
    
    /* Disable the PG Bit */
    FLASH->CR &= ~FLASH_CR_PG;
  } 
  
  /* Return the Program Status */
  return status;
}


void write_flash_holfword(uint32_t Address, uint16_t data)
{
	if (FLASH_ProgramHalfWord(Address, data) != FLASH_COMPLETE)
	{
	//	while(1)
		{
		}
	}
}

void FLASH_Unlock(void)
{
  if((FLASH->CR & FLASH_CR_LOCK) != RESET)
  {
    /* Unlocking the program memory access */
    FLASH->KEYR = FLASH_FKEY1;
    FLASH->KEYR = FLASH_FKEY2;
  }
}

void FLASH_Lock(void)
{
  /* Set the LOCK Bit to lock the FLASH control register and program memory access */
  FLASH->CR |= FLASH_CR_LOCK;
}

void FLASH_ClearFlag(uint32_t FLASH_FLAG)
{
  /* Check the parameters */
//  assert_param(IS_FLASH_CLEAR_FLAG(FLASH_FLAG));
  
  /* Clear the flags */
  FLASH->SR = FLASH_FLAG;
}

void write_flash_holfword_buffer(uint32_t address, uint16_t *data, uint16_t sizes)
{
	uint32_t p_address = address;
	uint16_t i;
	//uint16_t data_len=0;
	//uint16_t data_len1=0;
	uint16_t temp1=0;

	//temp1 = (uint32_t)(&glob_para.stop_time);
	//temp2 = (uint32_t)&glob_para.read_fisrt;

	if(sizes%2)sizes += 1;
	temp1 = sizes / 2;
/*
	if(sizes > 0x400)
	{
		data_len = (sizes - 0x400)/2; // ³ý2 ÊÇÒòÎª°ë×ÖÐ´´æ
		data_len1 = 0x400 / 2;
		temp1 = 1;
	}
	else
	{
		data_len1 = sizes / 2;
	}*/
	
	FLASH_Unlock();
	
	/* Clear pending flags (if any) */	
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR); 
	//erase_flash_OnePage(p_address);
	//erase_flash_OnePage(p_address+400); // Ò»Ò³1K,  ¹²2k
		FLASH_ErasePage(p_address);
		FLASH_ErasePage(p_address+FLASH_PAGE_SIZE);
		FLASH_ErasePage(p_address+(FLASH_PAGE_SIZE*2));
//	else
//	{
//		FLASH_ErasePage(p_address);
//	}
	
//	if(temp1)FLASH_ErasePage(p_address+FLASH_PAGE_SIZE);
	Feed_IWDG(); //Î¹¹·

		for(i=0;i< (temp1);i++) // 
		{
			if(p_address < (address+(FLASH_PAGE_SIZE*3)))
			{
				write_flash_holfword(p_address, (uint16_t)data[i]);

				p_address+=2;
			}
		}
/*	if(temp1)
	{
		p_address = address+FLASH_PAGE_SIZE;
		for(i=0;i< data_len;i++) // µÚ¶þÒ³
		{
			if(p_address < (address+FLASH_PAGE_SIZE+0x400))
			{
				write_flash_holfword(p_address, (uint16_t)data[i+0x200]);

				p_address+=2;
			}
		}
	}
	*/
	
	Feed_IWDG(); //Î¹¹·

	 FLASH_Lock(); 
}

void read_flash_holfword(uint32_t address, uint16_t *data, uint16_t sizes)
{
	uint32_t p_address = address;
	uint16_t i;
//	uint16_t data_len=0;
	uint16_t data_len1=0;
	uint16_t p_temp=0;
//	uint8_t temp1=0;
	
	if(sizes%2)sizes += 1; // ³ý2 ÊÇÒòÎª°ë×ÖÐ´´æ
//	String_Printf(USART_2,"AA",2);
//	String_Printf(USART_2,(uint8_t *)&sizes,2);
/*	if(sizes > 0x400)
	{
		data_len = (sizes - 0x400); 
		data_len1 = 0x400;
		temp1 = 1;
	}
	else*/
	{
		data_len1 = sizes/2;
	}
	Feed_IWDG();//Î¹¹·

	for(i=0;i<data_len1;i++)
	{
		if(p_address < (address+(FLASH_PAGE_SIZE + FLASH_PAGE_SIZE + FLASH_PAGE_SIZE)))
		{
			p_temp = *(__IO uint16_t *)p_address;
			data[i] = p_temp;
			p_address += 2;
		}
	}

	/*if(temp1)
	{
		address  += FLASH_PAGE_SIZE;
		p_address = address;
		for(i=0;i<data_len;i++)
		{
			if(p_address < (address+FLASH_PAGE_SIZE))
			{
				p_temp = *(__IO uint8_t *)p_address;
				data[i+0x400] = p_temp;
				p_address += 1;
			}
		}
	}*/
	
	Feed_IWDG(); //Î¹¹·

}

void FLASH_OB_Unlock(void)
{
  if((FLASH->CR & FLASH_CR_OPTWRE) == RESET)
  { 
    /* Unlocking the option bytes block access */
    FLASH->OPTKEYR = FLASH_OPTKEY1;
    FLASH->OPTKEYR = FLASH_OPTKEY2;
  }
}

void FLASH_OB_Lock(void)
{
  /* Set the OPTWREN Bit to lock the option bytes block access */
  FLASH->CR &= ~FLASH_CR_OPTWRE;
}

FLASH_Status FLASH_OB_RDPConfig(uint8_t OB_RDP)
{
  FLASH_Status status = FLASH_COMPLETE;
  
  /* Check the parameters */
//  assert_param(IS_OB_RDP(OB_RDP));
  status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
  
  if(status == FLASH_COMPLETE)
  {
    FLASH->CR |= FLASH_CR_OPTER;
    FLASH->CR |= FLASH_CR_STRT;
    
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
    
    if(status == FLASH_COMPLETE)
    {
      /* If the erase operation is completed, disable the OPTER Bit */
      FLASH->CR &= ~FLASH_CR_OPTER;
      
      /* Enable the Option Bytes Programming operation */
      FLASH->CR |= FLASH_CR_OPTPG;
       
      OB->RDP = OB_RDP;

      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT); 
    
      if(status != FLASH_TIMEOUT)
      {
        /* if the program operation is completed, disable the OPTPG Bit */
        FLASH->CR &= ~FLASH_CR_OPTPG;
      }
    }
    else 
    {
      if(status != FLASH_TIMEOUT)
      {
        /* Disable the OPTER Bit */
        FLASH->CR &= ~FLASH_CR_OPTER;
      }
    }
  }
  /* Return the protection operation Status */
  return status;
}

void FLASH_OB_Launch(void)
{
  /* Set the OBL_Launch bit to launch the option byte loading */
  FLASH->CR |= FLASH_CR_OBL_LAUNCH;
}


void flash_read_protect(void)
{
	FLASH_Unlock();
	FLASH_OB_Unlock();
	FLASH_OB_RDPConfig(OB_RDP_Level_1);
	FLASH_OB_Lock();
	FLASH_Lock();
	FLASH_OB_Launch();

	/*if(FLASH_OB_RDPConfig(OB_RDP_Level_1) == FLASH_COMPLETE )
	{
		FLASH_OB_Launch();
		FLASH_OB_Lock();
		FLASH_Lock();
	}*/
	
}
void para_read_from_flash(void)
{
	read_flash_holfword(FLASH_WRITE_START_ADDR, (uint16_t *)&glob_para.read_fisrt, sizeof(glob_para));
}


void if_write_flash(void)
{
	whether_write_flash = 1;
}

void write_flash_active(void)
{
	if(whether_write_flash)
	{
		whether_write_flash = 0;
		write_flash_holfword_buffer(FLASH_WRITE_START_ADDR, (uint16_t *)&glob_para.read_fisrt, sizeof(glob_para));
	}
}

void write_bootTag(uint8_t tag, uint16_t sizess)
{
	IAP_INF_T bufer={0};

	read_flash_holfword(FLASH_WRITE_START_ADDR22, (uint16_t *)&bufer, sizeof(bufer));
	bufer.usart_number = tag;
	bufer.firmware = sizess;
	bufer.firmware_version = FW_VERSIONS;
	bufer.firmware_checksum = return_checksum();
	write_flash_holfword_buffer(FLASH_WRITE_START_ADDR22, (uint16_t *)&bufer, sizeof(bufer));
}

void check_uid(void)
{
	//IAP_INF_T iit={0};
	uint8_t device_id22[12]={0x00, 0x02, 0x04, 0x06, 0x08, 0x0a, 0x0c, 0x0e, 0x10, 0x12, 0x14, 0x16};
	//uint8_t i = 0;
	
	if(UID_Encryption_Key_Check((void *)FLASH_UID_ADDR, (void *)0x1FFFF7AC, device_id22, LENGTH_12, LITTLE_ENDIA, ALGORITHM_2))
	{
		while(1)// can't be used
		{
			Feed_IWDG();
		}
	}
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
