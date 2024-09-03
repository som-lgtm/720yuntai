#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f0xx.h"

#define FLASH_WRITE_START_ADDR22	((uint32_t)0x08001C00)	 /* Start @ of user Flash area */
#define FLASH_UID_ADDR				((uint32_t)0x0800F800)	 /* 对应烧录器的地址 */
#define FLASH_WRITE_START_ADDR	((uint32_t)0x0800EC00)	 /* Start @ of user Flash area */
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

void para_read_from_flash(void);
void write_flash_tag_verify(void);
void read_flash_holfword(uint32_t address, uint8_t *data, uint16_t sizes);

#endif
