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
#include "stm32f0xx.h"
#define WRITE_FLASH_MODE	0

#if WRITE_FLASH_MODE
/** @addtogroup STM32F0xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup FLASH_Program
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/* Private define ------------------------------------------------------------*/
#ifdef STM32F072
 #define FLASH_PAGE_SIZE         ((uint32_t)0x00000800)   /* FLASH Page Size */
 #define FLASH_USER_START_ADDR   ((uint32_t)0x08009000)   /* Start @ of user Flash area */
 #define FLASH_USER_END_ADDR     ((uint32_t)0x08020000)   /* End @ of user Flash area */
#elif defined (STM32F091)
 #define FLASH_PAGE_SIZE         ((uint32_t)0x00000800)   /* FLASH Page Size */
 #define FLASH_USER_START_ADDR   ((uint32_t)0x08009000)   /* Start @ of user Flash area */
 #define FLASH_USER_END_ADDR     ((uint32_t)0x08040000)   /* End @ of user Flash area */
#else
 #define FLASH_PAGE_SIZE         ((uint32_t)0x00000400)   /* FLASH Page Size */
 #define FLASH_USER_START_ADDR   ((uint32_t)0x08006000)   /* Start @ of user Flash area */
 #define FLASH_USER_END_ADDR     ((uint32_t)0x08007000)   /* End @ of user Flash area */
#endif /* STM32F072 */
#define DATA_32                 ((uint32_t)0x12345678)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t EraseCounter = 0x00, Address = 0x00;
uint32_t Data = 0x3210ABCD;
uint32_t NbrOfPage = 0x00;
__IO FLASH_Status FLASHStatus = FLASH_COMPLETE;
__IO TestStatus MemoryProgramStatus = PASSED;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

#endif

///////////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "stm32f030x8.h"

#define FLASH_PAGE_SIZE 		((uint32_t)0x00000400)	 /* FLASH Page Size */
#define FLASH_FKEY1                          ((uint32_t)0x45670123)        /*!< Flash program erase key1 */
#define FLASH_FKEY2                          ((uint32_t)0xCDEF89AB)        /*!< Flash program erase key2: used with FLASH_PEKEY1
                                                                                to unlock the write access to the FPEC. */
#define FLASH_ER_PRG_TIMEOUT         ((uint32_t)0x000B0000)
#define OB_RDP_Level_0   ((uint8_t)0xAA)
#define OB_RDP_Level_1   ((uint8_t)0xBB)
#define FLASH_FLAG_BSY                 FLASH_SR_BSY     /*!< FLASH Busy flag */
#define FLASH_FLAG_PGERR               FLASH_SR_PGERR   /*!< FLASH Programming error flag */
#define FLASH_FLAG_WRPERR              FLASH_SR_WRPERR  /*!< FLASH Write protected error flag */
#define FLASH_FLAG_EOP                 FLASH_SR_EOP     /*!< FLASH End of Programming flag */

uint8_t whether_write_flash=0;
//PARA_STRUCT glob_para;

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
    
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
    
    /* Disable the PER Bit */
    FLASH->CR &= ~FLASH_CR_PER;
  }
    
  /* Return the Erase Status */
  return status;
}

void erase_flash_OnePage(void)
{
	//FLASH_Unlock();
	
	/* Clear pending flags (if any) */	
//	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR); 
	
	if (FLASH_ErasePage(FLASH_WRITE_START_ADDR)!= FLASH_COMPLETE)
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
		while(1)
		{
		}
	}
}

uint32_t FLASH_GetWriteProtectionOptionByte(void)
{
	return (uint32_t)(FLASH->WRPR);
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

void write_flash_holfword_buffer(uint16_t *data, uint8_t sizes)
{
	uint32_t p_address = FLASH_WRITE_START_ADDR;
	uint8_t i;
//	uint8_t data_len=0;
	//uint32_t temp1, temp2;

	//temp1 = (uint32_t)(&glob_para.stop_time);
	//temp2 = (uint32_t)&glob_para.read_fisrt;

	//data_len = sizeof(glob_para); //(uint8_t)(temp1-temp2);
	
	FLASH_Unlock();
	
	/* Clear pending flags (if any) */	
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR); 
	erase_flash_OnePage();
	
	//Feed_IWDG(); //Î¹¹·
	
	for(i=0;i< sizes/2;i++)
	{
		if(p_address < (FLASH_WRITE_START_ADDR+FLASH_PAGE_SIZE))
		{
			write_flash_holfword(p_address, (uint16_t)data[i]);

			p_address+=2;
		}
	}
	
	//Feed_IWDG(); //Î¹¹·

	 FLASH_Lock(); 
}

void read_flash_holfword(uint32_t address,uint16_t *spt, uint8_t sizes)
{
	uint32_t p_address = address;
	uint8_t i;
//	uint8_t data_len=0;
	uint16_t p_temp=0;
	
	//data_len =sizeof(glob_para); //(uint8_t)(temp1-temp2);

	//Feed_IWDG();//Î¹¹·
	
	for(i=0;i<sizes/2;i++)
	{
		if(p_address < (address+FLASH_PAGE_SIZE))
		{
			p_temp = *(__IO uint16_t *)p_address;
			spt[i] = p_temp;
			p_address += 2;
		}
	}
	
	//Feed_IWDG(); //Î¹¹·
}

void iap_flash_write_iap_inf(uint8_t isr_tag)
{
	IAP_INF_T infor={0};
	
	read_flash_holfword(FLASH_WRITE_START_ADDR, (uint16_t *)&infor, sizeof(infor));

	infor.ISR_tag = isr_tag;
	write_flash_holfword_buffer((uint16_t *)&infor,sizeof(infor));
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



/**
  * @brief  Programs a word at a specified address.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  __IO uint32_t tmp = 0;

  /* Check the parameters */
//  assert_param(IS_FLASH_ADDRESS(Address));


  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
  
  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to program the new first 
    half word */
    FLASH->CR |= FLASH_CR_PG;
  
    *(__IO uint16_t*)Address = (uint16_t)Data;
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
 
    if(status == FLASH_COMPLETE)
    {
      /* if the previous operation is completed, proceed to program the new second 
      half word */
      tmp = Address + 2;

      *(__IO uint16_t*) tmp = Data >> 16;
    
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
        
      /* Disable the PG Bit */
	  FLASH->CR &= ~FLASH_CR_PG;
    }
    else
    {
      /* Disable the PG Bit */
	  FLASH->CR &= ~FLASH_CR_PG;
    }
  }         
   
  /* Return the Program Status */
  return status;
}


/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
