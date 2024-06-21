/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : common.h
* Author             : MCD Application Team
* Date First Issued  : 05/21/2007
* Description        : This file provides all the headers of the common functions.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _COMMON_H
#define _COMMON_H

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "string.h"
#include "stm32f0xx.h"
//#include "ymodem.h" 
#include "stm32f0xx_ll_usart.h"

/*!< STM32F10x Standard Peripheral Library old types (maintained for legacy purpose) */
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef int32_t sc32;  /*!< Read Only */
typedef int16_t sc16;  /*!< Read Only */
typedef int8_t sc8;   /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef uint32_t uc32;  /*!< Read Only */
typedef uint16_t uc16;  /*!< Read Only */
typedef uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */



typedef enum {FALSE = 0, TRUE = !FALSE} bool;

typedef enum
{
  FLASH_BUSY = 1,
  FLASH_ERROR_WRP,
  FLASH_ERROR_PROGRAM,
  FLASH_COMPLETE,
  FLASH_TIMEOUT
}FLASH_Status;

typedef struct
{
	uint8_t MCU_uid[12];			/* the uid of mcu */
	uint8_t first_used; 		   /* first use the product*/
	uint8_t usart_number;			  /* usart select: usart 1 or usart 2*/
	uint8_t  ISR_tag;                /*the flag of isr source */
	uint8_t  Reserved;                /*Reserved */
	uint16_t firmware; // firmware sizes
	uint16_t firmware_version;
	uint32_t firmware_checksum;
}IAP_INF_T;

//#define IS_FUNCTIONAL_STATE(STATE) ((STATE == DISABLE) || (STATE == ENABLE))

//typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;


/* Exported types ------------------------------------------------------------*/
typedef  void (*pFunction)(void);

/* Exported constants --------------------------------------------------------*/
/* Constants used by Serial Command Line Mode */
#define CMD_STRING_SIZE     128

#define ApplicationAddress 0x8002000
#define FLASH_WRITE_START_ADDR	(0x08001c00)	 /* Start @ of user Flash area */
#define Page_SIZE 	(0X400)
#define ErasePage_SIZE 	(0xD400)	 /* 0XFFFF-0X2000-0Xc00: 0Xc00是最后几页的自定义数据，0X2000是最前面的BOOTLOAD数据 */
#define ErasePage 	(ErasePage_SIZE/Page_SIZE)

#define ISR_FLAG   ((__IO uint8_t *)(FLASH_WRITE_START_ADDR+0x0e))    /* ISR来源标志存储在FLASH中的地址 */
#define FROM_APP   (((*ISR_FLAG)&0xFF) == 0x48)             /* 如果为真表示ISR来源是APP,否则来源是BOOT */
#define FILE_NAME_LENGTH        (256)
#define FILE_SIZE_LENGTH        (17)
              
/* Exported macro ------------------------------------------------------------*/
/* Common routines */
#define IS_AF(c)  ((c >= 'A') && (c <= 'F'))
#define IS_af(c)  ((c >= 'a') && (c <= 'f'))
#define IS_09(c)  ((c >= '0') && (c <= '9'))
#define ISVALIDHEX(c)  IS_AF(c) || IS_af(c) || IS_09(c)
#define ISVALIDDEC(c)  IS_09(c)
#define CONVERTDEC(c)  (c - '0')

#define CONVERTHEX_alpha(c)  (IS_AF(c) ? (c - 'A'+10) : (c - 'a'+10))
#define CONVERTHEX(c)   (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))

// Selection of the chip
#define VTAB031x    0
#define VTAB030x    1
#define VTAB_SEL    VTAB030x

#define APP_VECTOR_TABLE_BASE_ADDR         (0x08002000)        /*  APP中的向量表基地址(应用程序(APP)配置的偏移地址) */

/* APP的中断服务函数入口地址 */
#define APP_VECTOR_RESET                   (APP_VECTOR_TABLE_BASE_ADDR + 0x04)     /* APP RESET中断向量地址 */
#define APP_VECTOR_NMI                     (APP_VECTOR_TABLE_BASE_ADDR + 0x08)     /* APP NMI中断向量地址 */
#define APP_VECTOR_HARDFAULT               (APP_VECTOR_TABLE_BASE_ADDR + 0x0c)     /* APP HardFault中断向量地址 */
#define APP_VECTOR_SVCALL                  (APP_VECTOR_TABLE_BASE_ADDR + 0x2c)     /* APP SVCall中断向量地址 */
#define APP_VECTOR_PENDSV                  (APP_VECTOR_TABLE_BASE_ADDR + 0x38)     /* APP PendSV中断向量地址 */
#define APP_VECTOR_SYSTICK                 (APP_VECTOR_TABLE_BASE_ADDR + 0x3c)     /* APP SysTick中断向量地址 */
#define APP_VECTOR_WWDG                    (APP_VECTOR_TABLE_BASE_ADDR + 0x40)     /* APP WWDG中断向量地址 */
#if (VTAB_SEL == VTAB031x)
#define APP_VECTOR_PVD_VDDIO2              (APP_VECTOR_TABLE_BASE_ADDR + 0x44)     /* APP PVD_VDDIO2中断向量地址 */
#endif
#define APP_VECTOR_RTC                     (APP_VECTOR_TABLE_BASE_ADDR + 0x48)     /* APP RTC(combined EXTI lines17,19 and 20)中断向量地址 */
#define APP_VECTOR_FLASH                   (APP_VECTOR_TABLE_BASE_ADDR + 0x4c)     /* APP FLASH中断向量地址 */
#define APP_VECTOR_RCC                     (APP_VECTOR_TABLE_BASE_ADDR + 0x50)     /* APP RCC中断向量地址 */
#define APP_VECTOR_EXTI0_1                 (APP_VECTOR_TABLE_BASE_ADDR + 0x54)     /* APP EXTI0_1中断向量地址 */
#define APP_VECTOR_EXTI2_3                 (APP_VECTOR_TABLE_BASE_ADDR + 0x58)     /* APP EXTI2_3中断向量地址 */
#define APP_VECTOR_EXTI4_15                (APP_VECTOR_TABLE_BASE_ADDR + 0x5C)     /* APP EXTI4_15中断向量地址 */
#if (VTAB_SEL == VTAB031x)
#define APP_VECTOR_TSC                     (APP_VECTOR_TABLE_BASE_ADDR + 0x60)     /* APP TSC中断向量地址 */
#endif
#define APP_VECTOR_DMA_CH1                 (APP_VECTOR_TABLE_BASE_ADDR + 0x64)     /* APP DMA_CH1中断向量地址 */
#define APP_VECTOR_DMA_CH2_3               (APP_VECTOR_TABLE_BASE_ADDR + 0x68)     /* APP DMA_CH2_3中断向量地址 */
#define APP_VECTOR_DMA_CH4_5               (APP_VECTOR_TABLE_BASE_ADDR + 0x6c)     /* APP DMA_CH4_5中断向量地址 */
#define APP_VECTOR_ADC                     (APP_VECTOR_TABLE_BASE_ADDR + 0x70)     /* APP ADC中断向量地址 */
#define APP_VECTOR_TIM1_BRK_UP_TRG_COM     (APP_VECTOR_TABLE_BASE_ADDR + 0x74)     /* APP TIM1_BRK_UP_TRG_COM中断向量地址 */
#define APP_VECTOR_TIM1_CC                 (APP_VECTOR_TABLE_BASE_ADDR + 0x78)     /* APP TIM1_CC中断向量地址 */
#if (VTAB_SEL == VTAB031x)
#define APP_VECTOR_TIM2                    (APP_VECTOR_TABLE_BASE_ADDR + 0x7C)     /* APP TIM2中断向量地址 */
#endif
#define APP_VECTOR_TIM3                    (APP_VECTOR_TABLE_BASE_ADDR + 0x80)     /* APP TIM3中断向量地址 */
#define APP_VECTOR_TIM6                    (APP_VECTOR_TABLE_BASE_ADDR + 0x84)     /* APP TIM6中断向量地址 */
#if (VTAB_SEL == VTAB031x)
#define APP_VECTOR_TIM7                    (APP_VECTOR_TABLE_BASE_ADDR + 0x88)     /* APP TIM7中断向量地址 */
#endif
#define APP_VECTOR_TIM14                   (APP_VECTOR_TABLE_BASE_ADDR + 0x8c)     /* APP TIM14中断向量地址 */
#define APP_VECTOR_TIM15                   (APP_VECTOR_TABLE_BASE_ADDR + 0x90)     /* APP TIM15中断向量地址 */
#define APP_VECTOR_TIM16                   (APP_VECTOR_TABLE_BASE_ADDR + 0x94)     /* APP TIM16中断向量地址 */
#define APP_VECTOR_TIM17                   (APP_VECTOR_TABLE_BASE_ADDR + 0x98)     /* APP TIM17中断向量地址 */
#define APP_VECTOR_I2C1                    (APP_VECTOR_TABLE_BASE_ADDR + 0x9c)     /* APP I2C1中断向量地址 */
#define APP_VECTOR_I2C2                    (APP_VECTOR_TABLE_BASE_ADDR + 0xa0)     /* APP I2C2中断向量地址 */
#define APP_VECTOR_SPI1                    (APP_VECTOR_TABLE_BASE_ADDR + 0xa4)     /* APP SPI1中断向量地址 */
#define APP_VECTOR_SPI2                    (APP_VECTOR_TABLE_BASE_ADDR + 0xa8)     /* APP SPI2中断向量地址 */
#define APP_VECTOR_USART1                  (APP_VECTOR_TABLE_BASE_ADDR + 0xac)     /* APP USART1中断向量地址 */
#if (VTAB_SEL == VTAB030x)
#define APP_VECTOR_USART2                  (APP_VECTOR_TABLE_BASE_ADDR + 0xb0)     /* APP USART2中断向量地址 */
#endif
#define APP_VECTOR_USART3_4_5_6            (APP_VECTOR_TABLE_BASE_ADDR + 0xb4)     /* APP USART3_4_5_6中断向量地址 */
#if (VTAB_SEL == VTAB031x)
#define APP_VECTOR_CEC_CAN                 (APP_VECTOR_TABLE_BASE_ADDR + 0xb8)     /* APP CEC_CAN中断向量地址 */
#endif
#define APP_VECTOR_USB                     (APP_VECTOR_TABLE_BASE_ADDR + 0xbc)     /* APP USB中断向量地址 */

/* 中断服务函数指针类型定义 */
typedef void( *_app_interrupt_ser_t )( void );

/* 跳转到指定地址存储的函数指针对应的函数 */
#define APP_JUMP(addr)                     ((_app_interrupt_ser_t)(*(uint32_t *)(addr)))


/* Exported functions ------------------------------------------------------- */
void Int2Str(u8* str,s32 intnum);
u32 Str2Int(u8 *inputstr,s32 *intnum);
u32 GetIntegerInput(s32 * num);
u32 SerialKeyPressed(u8 *key);
u8 GetKey(void);
void SerialPutChar(u8 c);
void UsartSendBuff(u8 *stp, u8 sizes);
void SerialPutString(u8 *s);
void GetInputString(u8 * buffP);
u32 FLASH_PagesMask(vu32 Size);
void FLASH_DisableWriteProtectionPages(void);
void Main_Menu(void);
void test_display(void);
void para_init(void);

// flash.c
void FLASH_Unlock(void);
FLASH_Status FLASH_ErasePage(uint32_t Page_Address);
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data);
void write_flash_holfword_buffer(uint16_t *data, uint8_t sizes);
void read_flash_holfword(uint32_t address, uint16_t *spt, uint8_t sizes);
void iap_flash_write_iap_inf(uint8_t isr_tag);

// download.c
void JomToApp(void);
void SerialDownload(void);
u8 Handshake_Get(void);
u8 NameAndSize_Check(uint8_t *spt);
void lights_flash(void);

extern u32 JumpAddress;
extern pFunction Jump_To_Application;
extern uint8_t select_usart;
extern u32 fw_sizes;
extern u32 fw_checksum;
extern u32 NbrOfPage;
extern u8 file_name[FILE_NAME_LENGTH];
extern u8 ffile_size[FILE_SIZE_LENGTH];
extern u32 FlashDestination;
extern uint8_t force_updata;

//extern uint8_t id_bufer[14];

#endif  /* _COMMON_H */

/*******************(C)COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
