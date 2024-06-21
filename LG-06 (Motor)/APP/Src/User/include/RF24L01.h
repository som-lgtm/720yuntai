#ifndef __24L01_H
#define __24L01_H
#include "stm32f030x8.h"

//#define USE_HARD_SPI		0
#define USE_SOFT_SPI		0
#define IRQ_READ_STATUS   0

#ifdef USE_SOFT_SPI


#define RF24L01_SCK_ON		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3)
#define RF24L01_SCK_OFF		LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3)

#define RF24L01_MOSI_ON		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_5)
#define RF24L01_MOSI_OFF		LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_5)

#define RF24L01_MISO	LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_4)

#endif

#define RF24L01_CSN_ON		LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_15)
#define RF24L01_CSN_OFF		LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_15)

#define RF24L01_CE_ON		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6)
#define RF24L01_CE_OFF		LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6)

#define SPI_WAIT_TIMEOUT			((uint16_t)0xFFFF)

#define EN_CRC       	3 
#define CRCO         	2 
#define PWR_UP       	1 
#define PRIM_RX      	0 

#define ENAA_P5      	5 
#define ENAA_P4      	4 
#define ENAA_P3      	3 
#define ENAA_P2      	2 
#define ENAA_P1      	1 
#define ENAA_P0      	0 

#define ERX_P5       	5 
#define ERX_P4       	4 
#define ERX_P3       	3 
#define ERX_P2      	2 
#define ERX_P1       	1 
#define ERX_P0       	0 

#define AW_RERSERVED 	0x0 
#define AW_3BYTES    	0x1
#define AW_4BYTES    	0x2
#define AW_5BYTES    	0x3

#define ARD_250US    	(0x00<<4)
#define ARD_500US    	(0x01<<4)
#define ARD_750US    	(0x02<<4)
#define ARD_1000US   	(0x03<<4)
#define ARD_2000US   	(0x07<<4)
#define ARD_4000US   	(0x0F<<4)
#define ARC_DISABLE   	0x00
#define ARC_15        	0x0F

//¼Ä´æÆ÷²Ù×÷ÃüÁî
#define NRF_READ_REG    0x00	//¶ÁÅäÖÃ¼Ä´æÆ÷£¬µÍ5Î»Îª¼Ä´æÆ÷µØÖ·
#define NRF_WRITE_REG   0x20	//Ð´ÅäÖÃ¼Ä´æÆ÷£¬µÍ5Î»Îª¼Ä´æÆ÷µØÖ·
#define RD_RX_PLOAD     0x61	//¶ÁRXÓÐÐ§Êý¾Ý£¬1~32×Ö½Ú
#define WR_TX_PLOAD     0xA0	//Ð´TXÓÐÐ§Êý¾Ý£¬1~32×Ö½Ú
#define FLUSH_TX        0xE1	//Çå³ýTX FIFO¼Ä´æÆ÷£¬·¢ÉäÄ£Ê½ÏÂÊ¹ÓÃ
#define FLUSH_RX        0xE2	//Çå³ýRX FIFO¼Ä´æÆ÷£¬½ÓÊÕÄ£Ê½ÏÂÊ¹ÓÃ
#define REUSE_TX_PL     0xE3	//ÖØÐÂÊ¹ÓÃÉÏÒ»°üÊý¾Ý£¬CEÎª¸ß£¬Êý¾Ý°ü±»²»¶Ï·¢ËÍ
#define R_RX_PL_WID     0x60
#define NOP             0xFF	//¿Õ²Ù×÷£¬¿ÉÒÔÓÃÀ´¶Á×´Ì¬¼Ä´æÆ÷
#define W_ACK_PLOAD		0xA8
#define WR_TX_PLOAD_NACK 0xB0
//SPI(NRF24L01)¼Ä´æÆ÷µØÖ·
#define CONFIG          0x00	//ÅäÖÃ¼Ä´æÆ÷µØÖ·£¬bit0:1½ÓÊÕÄ£Ê½,0·¢ÉäÄ£Ê½;bit1:µçÑ¡Ôñ;bit2:CRCÄ£Ê½;bit3:CRCÊ¹ÄÜ;
							    //bit4:ÖÐ¶ÏMAX_RT(´ïµ½×î´óÖØ·¢´ÎÊýÖÐ¶Ï)Ê¹ÄÜ;bit5:ÖÐ¶ÏTX_DSÊ¹ÄÜ;bit6:ÖÐ¶ÏRX_DRÊ¹ÄÜ	
#define EN_AA           0x01	//Ê¹ÄÜ×Ô¶¯Ó¦´ð¹¦ÄÜ bit0~5 ¶ÔÓ¦Í¨µÀ0~5
#define EN_RXADDR       0x02	//½ÓÊÕµØÖ·ÔÊÐí bit0~5 ¶ÔÓ¦Í¨µÀ0~5
#define SETUP_AW        0x03	//ÉèÖÃµØÖ·¿í¶È(ËùÓÐÊý¾ÝÍ¨µÀ) bit0~1: 00,3×Ö½Ú 01,4×Ö½Ú, 02,5×Ö½Ú
#define SETUP_RETR      0x04	//½¨Á¢×Ô¶¯ÖØ·¢;bit0~3:×Ô¶¯ÖØ·¢¼ÆÊýÆ÷;bit4~7:×Ô¶¯ÖØ·¢ÑÓÊ± 250*x+86us
#define RF_CH           0x05	//RFÍ¨µÀ,bit0~6¹¤×÷Í¨µÀÆµÂÊ
#define RF_SETUP        0x06	//RF¼Ä´æÆ÷£¬bit3:´«ÊäËÙÂÊ( 0:1M 1:2M);bit1~2:·¢Éä¹¦ÂÊ;bit0:ÔëÉù·Å´óÆ÷ÔöÒæ
#define STATUS          0x07	//×´Ì¬¼Ä´æÆ÷;bit0:TX FIFOÂú±êÖ¾;bit1~3:½ÓÊÕÊý¾ÝÍ¨µÀºÅ(×î´ó:6);bit4:´ïµ½×î¶à´ÎÖØ·¢´ÎÊý
								//bit5:Êý¾Ý·¢ËÍÍê³ÉÖÐ¶Ï;bit6:½ÓÊÕÊý¾ÝÖÐ¶Ï
#define MAX_TX  		0x10	//´ïµ½×î´ó·¢ËÍ´ÎÊýÖÐ¶Ï
#define TX_OK   		0x20	//TX·¢ËÍÍê³ÉÖÐ¶Ï
#define RX_OK   		0x40	//½ÓÊÕµ½Êý¾ÝÖÐ¶Ï

#define OBSERVE_TX      0x08	//·¢ËÍ¼ì²â¼Ä´æÆ÷,bit7~4:Êý¾Ý°ü¶ªÊ§¼ÆÊýÆ÷;bit3~0:ÖØ·¢¼ÆÊýÆ÷
#define CD              0x09	//ÔØ²¨¼ì²â¼Ä´æÆ÷,bit0:ÔØ²¨¼ì²â
#define RX_ADDR_P0      0x0A	//Êý¾ÝÍ¨µÀ0½ÓÊÕµØÖ·£¬×î´ó³¤¶È5¸ö×Ö½Ú£¬µÍ×Ö½ÚÔÚÇ°
#define RX_ADDR_P1      0x0B	//Êý¾ÝÍ¨µÀ1½ÓÊÕµØÖ·£¬×î´ó³¤¶È5¸ö×Ö½Ú£¬µÍ×Ö½ÚÔÚÇ°
#define RX_ADDR_P2      0x0C	//Êý¾ÝÍ¨µÀ2½ÓÊÕµØÖ·£¬×îµÍ×Ö½Ú¿ÉÉèÖÃ£¬¸ß×Ö½Ú£¬±ØÐëÍ¬RX_ADDR_P1[39:8]ÏàµÈ
#define RX_ADDR_P3      0x0D	//Êý¾ÝÍ¨µÀ3½ÓÊÕµØÖ·£¬×îµÍ×Ö½Ú¿ÉÉèÖÃ£¬¸ß×Ö½Ú£¬±ØÐëÍ¬RX_ADDR_P1[39:8]ÏàµÈ
#define RX_ADDR_P4      0x0E	//Êý¾ÝÍ¨µÀ4½ÓÊÕµØÖ·£¬×îµÍ×Ö½Ú¿ÉÉèÖÃ£¬¸ß×Ö½Ú£¬±ØÐëÍ¬RX_ADDR_P1[39:8]ÏàµÈ
#define RX_ADDR_P5      0x0F	//Êý¾ÝÍ¨µÀ5½ÓÊÕµØÖ·£¬×îµÍ×Ö½Ú¿ÉÉèÖÃ£¬¸ß×Ö½Ú£¬±ØÐëÍ¬RX_ADDR_P1[39:8]ÏàµÈ
#define TX_ADDR         0x10	//·¢ËÍµØÖ·(µÍ×Ö½ÚÔÚÇ°),ShockBurstTMÄ£Ê½ÏÂ£¬RX_ADDR_P0ÓëµØÖ·ÏàµÈ
#define RX_PW_P0        0x11	//½ÓÊÕÊý¾ÝÍ¨µÀ0ÓÐÐ§Êý¾Ý¿í¶È(1~32×Ö½Ú),ÉèÖÃÎª0Ôò·Ç·¨
#define RX_PW_P1        0x12	//½ÓÊÕÊý¾ÝÍ¨µÀ1ÓÐÐ§Êý¾Ý¿í¶È(1~32×Ö½Ú),ÉèÖÃÎª0Ôò·Ç·¨
#define RX_PW_P2        0x13	//½ÓÊÕÊý¾ÝÍ¨µÀ2ÓÐÐ§Êý¾Ý¿í¶È(1~32×Ö½Ú),ÉèÖÃÎª0Ôò·Ç·¨
#define RX_PW_P3        0x14	//½ÓÊÕÊý¾ÝÍ¨µÀ3ÓÐÐ§Êý¾Ý¿í¶È(1~32×Ö½Ú),ÉèÖÃÎª0Ôò·Ç·¨
#define RX_PW_P4        0x15	//½ÓÊÕÊý¾ÝÍ¨µÀ4ÓÐÐ§Êý¾Ý¿í¶È(1~32×Ö½Ú),ÉèÖÃÎª0Ôò·Ç·¨
#define RX_PW_P5        0x16	//½ÓÊÕÊý¾ÝÍ¨µÀ5ÓÐÐ§Êý¾Ý¿í¶È(1~32×Ö½Ú),ÉèÖÃÎª0Ôò·Ç·¨
#define NRF_FIFO_STATUS 0x17	//FIFO×´Ì¬¼Ä´æÆ÷;bit0:RX FIFO¼Ä´æÆ÷¿Õ±êÖ¾;bit1:RX FIFOÂú±êÖ¾;bit2~3±£Áô
								//bit4:TX FIFO ¿Õ±êÖ¾;bit5:TX FIFOÂú±êÖ¾;bit6:1,Ñ­»··¢ËÍÉÏÒ»Êý¾Ý°ü.0,²»Ñ­»·								

#define REPEAT_CNT          15		//é‡å¤æ¬¡æ•°
#define INIT_ADDR           0x01,0x00,0x00,0x00,0x00
//#define INIT_ADDR           0x34,0x43,0x10,0x10,0x01
#define RF_DR_HIGH	(0<<3) //ÉèÖÃÉäÆµÊý¾ÝÂÊ
#define RF_DR_LOW	(1<<5) //ÉèÖÃÉäÆµÊý¾ÝÂÊ
#define RF_PWR	0x07

#define IRQ_ALL  ( (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT) )


#define RX_DR         	6 
#define TX_DS         	5 
#define MAX_RT        	4 
#define DYNAMIC_PACKET      1 		//1:åŠ¨æ€æ•°æ®åŒ…, 0:å›ºå®š

#define DYNPD			0x1C
#define FEATRUE			0x1D

// status
#define STATUS          0x07	//çŠ¶æ€å¯„å­˜å™¨;bit0:TX FIFOæ»¡æ ‡å¿—;bit1~3:æŽ¥æ”¶æ•°æ®é€šé“å·(æœ€å¤§:6);bit4:è¾¾åˆ°æœ€å¤šæ¬¡é‡å‘æ¬¡æ•°

#define RF24L01_IRQ_GPIO_PORT			GPIOG
#define RF24L01_IRQ_GPIO_CLK			RCC_APB2Periph_GPIOG
#define RF24L01_IRQ_GPIO_PIN			GPIO_Pin_6

#define RF24L01_GET_IRQ_STATUS( )		(LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_12) )

typedef enum ModeType
{
	MODE_TX = 0,
	MODE_RX
}nRf24l01ModeType;

void Irq_gpio_init(void);
void Nrf24L01_init(void);
void RF24L01_Set_Mode( nRf24l01ModeType Mode );
void RLF_Read_Main(void);
void NRF24L01_Write_Buf( uint8_t RegAddr, uint8_t *pBuf, uint8_t len );
void NRF24L01_Read_Buf( uint8_t RegAddr, uint8_t *pBuf, uint8_t len );
void RF24L01_Init( void );
void NRF24L01_check( void );
void wifi24G_data_Dispose(void);
void NRF24L01_Write_Reg( uint8_t RegAddr, uint8_t Value );
void NRF24L01_Set_TxAddr( uint8_t *pAddr, uint8_t len );
void NRF24L01_Set_RxAddr( uint8_t PipeNum, uint8_t *pAddr, uint8_t Len );	
void wifi24G_send_data(void);
uint8_t NRF24L01_RxPacket( uint8_t *rxbuf );
uint8_t NRF24L01_TxPacket( uint8_t *txbuf, uint8_t Length );
uint8_t drv_spi_read_write_byte(uint8_t TxByte);
uint8_t NRF24L01_Read_Reg( uint8_t RegAddr );
void wifi_sleep(void);

void test(void);

//gpio.c
void GPIO_INUP_Init(GPIO_TypeDef *GPIOx, uint32_t PinMask);

#endif



