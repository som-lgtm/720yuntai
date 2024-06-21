/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : ymodem.h
* Author             : MCD Application Team
* Date First Issued  : 05/21/2007
* Description        : This file provides all the software function headers of
*                      the ymodem.c file.
********************************************************************************
* History:
* 05/21/2007: V0.1 
********************************************************************************
THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A 
RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR 
CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT OF SUCH 
SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN
IN CONNECTION WITH THEIR PRODUCTS.
********************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _YMODEM_H_
#define _YMODEM_H_
#include "common.h"

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define PACKET_SEQNO_INDEX      (1)
#define PACKET_SEQNO_COMP_INDEX (2)

#define PACKET_HEADER           (3)
#define PACKET_TRAILER          (2)
#define PACKET_OVERHEAD         (PACKET_HEADER + PACKET_TRAILER)
#define PACKET_SIZE             (128)
#define PACKET_1K_SIZE          (1024)
#define PACKET_16_SIZE          (16)


#define SOH                     (0x01)  /* start of 128-byte data packet */
#define STX                     (0x02)  /* start of 1024-byte data packet */
#define APP                     (0x78)  /* start of 16-byte data packet */
#define APP_END					(0x06) // app end of transmission

#define EOT                     (0x04)  /* end of transmission */
#define ACK                     (0x06)  /* acknowledge */
#define NAK                     (0x15)  /* negative acknowledge */
#define CA                      (0x18)  /* two of these in succession aborts transfer */
#define CRC16                   (0x43)  /* 'C' == 0x43, request 16-bit CRC */

#define ABORT1                  (0x41)  /* 'A' == 0x41, abort by user */
#define ABORT2                  (0x61)  /* 'a' == 0x61, abort by user */

#define NAK_TIMEOUT             (0x100000)
#define MAX_ERRORS              (1)
#define HANDSHAKE				 (3)
#define TIMEOUT             (0x10000)
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
s32 Ymodem_Receive (u8 *);
void FW_ErasePage(u32 nofpage);
s8 fw_updata(s32 packet_length);
u32 Send_Byte (u8 c);
s32 Receive_Byte (u8 *c, u32 timeout);
u16 CRC16_IBM_Check(u8 *spt, u8 sizes);
uint8_t check_sum_add(uint8_t *spt, uint8_t index);
void OTA_Ack(uint8_t opcode, uint8_t data);
void Upgrade_failure_ErasePage(void);

#endif  /* _YMODEM_H_ */

/*******************(C)COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
