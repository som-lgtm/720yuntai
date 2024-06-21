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

//�Ĵ�����������
#define NRF_READ_REG    0x00	//�����üĴ�������5λΪ�Ĵ�����ַ
#define NRF_WRITE_REG   0x20	//д���üĴ�������5λΪ�Ĵ�����ַ
#define RD_RX_PLOAD     0x61	//��RX��Ч���ݣ�1~32�ֽ�
#define WR_TX_PLOAD     0xA0	//дTX��Ч���ݣ�1~32�ֽ�
#define FLUSH_TX        0xE1	//���TX FIFO�Ĵ���������ģʽ��ʹ��
#define FLUSH_RX        0xE2	//���RX FIFO�Ĵ���������ģʽ��ʹ��
#define REUSE_TX_PL     0xE3	//����ʹ����һ�����ݣ�CEΪ�ߣ����ݰ������Ϸ���
#define R_RX_PL_WID     0x60
#define NOP             0xFF	//�ղ���������������״̬�Ĵ���
#define W_ACK_PLOAD		0xA8
#define WR_TX_PLOAD_NACK 0xB0
//SPI(NRF24L01)�Ĵ�����ַ
#define CONFIG          0x00	//���üĴ�����ַ��bit0:1����ģʽ,0����ģʽ;bit1:��ѡ��;bit2:CRCģʽ;bit3:CRCʹ��;
							    //bit4:�ж�MAX_RT(�ﵽ����ط������ж�)ʹ��;bit5:�ж�TX_DSʹ��;bit6:�ж�RX_DRʹ��	
#define EN_AA           0x01	//ʹ���Զ�Ӧ���� bit0~5 ��Ӧͨ��0~5
#define EN_RXADDR       0x02	//���յ�ַ���� bit0~5 ��Ӧͨ��0~5
#define SETUP_AW        0x03	//���õ�ַ���(��������ͨ��) bit0~1: 00,3�ֽ� 01,4�ֽ�, 02,5�ֽ�
#define SETUP_RETR      0x04	//�����Զ��ط�;bit0~3:�Զ��ط�������;bit4~7:�Զ��ط���ʱ 250*x+86us
#define RF_CH           0x05	//RFͨ��,bit0~6����ͨ��Ƶ��
#define RF_SETUP        0x06	//RF�Ĵ�����bit3:��������( 0:1M 1:2M);bit1~2:���书��;bit0:�����Ŵ�������
#define STATUS          0x07	//״̬�Ĵ���;bit0:TX FIFO����־;bit1~3:��������ͨ����(���:6);bit4:�ﵽ�����ط�����
								//bit5:���ݷ�������ж�;bit6:���������ж�
#define MAX_TX  		0x10	//�ﵽ����ʹ����ж�
#define TX_OK   		0x20	//TX��������ж�
#define RX_OK   		0x40	//���յ������ж�

#define OBSERVE_TX      0x08	//���ͼ��Ĵ���,bit7~4:���ݰ���ʧ������;bit3~0:�ط�������
#define CD              0x09	//�ز����Ĵ���,bit0:�ز����
#define RX_ADDR_P0      0x0A	//����ͨ��0���յ�ַ����󳤶�5���ֽڣ����ֽ���ǰ
#define RX_ADDR_P1      0x0B	//����ͨ��1���յ�ַ����󳤶�5���ֽڣ����ֽ���ǰ
#define RX_ADDR_P2      0x0C	//����ͨ��2���յ�ַ������ֽڿ����ã����ֽڣ�����ͬRX_ADDR_P1[39:8]���
#define RX_ADDR_P3      0x0D	//����ͨ��3���յ�ַ������ֽڿ����ã����ֽڣ�����ͬRX_ADDR_P1[39:8]���
#define RX_ADDR_P4      0x0E	//����ͨ��4���յ�ַ������ֽڿ����ã����ֽڣ�����ͬRX_ADDR_P1[39:8]���
#define RX_ADDR_P5      0x0F	//����ͨ��5���յ�ַ������ֽڿ����ã����ֽڣ�����ͬRX_ADDR_P1[39:8]���
#define TX_ADDR         0x10	//���͵�ַ(���ֽ���ǰ),ShockBurstTMģʽ�£�RX_ADDR_P0���ַ���
#define RX_PW_P0        0x11	//��������ͨ��0��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P1        0x12	//��������ͨ��1��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P2        0x13	//��������ͨ��2��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P3        0x14	//��������ͨ��3��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P4        0x15	//��������ͨ��4��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P5        0x16	//��������ͨ��5��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define NRF_FIFO_STATUS 0x17	//FIFO״̬�Ĵ���;bit0:RX FIFO�Ĵ����ձ�־;bit1:RX FIFO����־;bit2~3����
								//bit4:TX FIFO �ձ�־;bit5:TX FIFO����־;bit6:1,ѭ��������һ���ݰ�.0,��ѭ��								

#define REPEAT_CNT          15		//重复次数
#define INIT_ADDR           0x01,0x00,0x00,0x00,0x00
//#define INIT_ADDR           0x34,0x43,0x10,0x10,0x01
#define RF_DR_HIGH	(0<<3) //������Ƶ������
#define RF_DR_LOW	(1<<5) //������Ƶ������
#define RF_PWR	0x07

#define IRQ_ALL  ( (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT) )


#define RX_DR         	6 
#define TX_DS         	5 
#define MAX_RT        	4 
#define DYNAMIC_PACKET      1 		//1:动态数据包, 0:固定

#define DYNPD			0x1C
#define FEATRUE			0x1D

// status
#define STATUS          0x07	//状态寄存器;bit0:TX FIFO满标志;bit1~3:接收数据通道号(最大:6);bit4:达到最多次重发次数

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



