#ifndef __UART_H
#define __UART_H
#include "stm32f0xx.h"

#define ANSWER_FALSE 0
#define ANSWER_SUCCESS	0XCC
#define LOCUS_BUFFER_MAX	8
#define USART_TAG	40
#define USART_SEND_TAG	20

void uart_send_data(unsigned short data);
void uart_int(uint32_t bands);
void uart_print(unsigned char str[]);
uint8_t checksum_verify(uint8_t *spt, uint8_t lenght);
uint8_t Usart22_return_Tx_Complete(void);
void Usart22_Reset_RX_Complete(void);
void Launch_Cache(void);
uint8_t return_RX_Complete(void);
void Reset_RX_Complete(void);
uint8_t return_DMA_SendComplete(void);
void write_bootTag(uint8_t tag);

typedef struct{
	uint8_t usart1_rec_dd[20];
	uint8_t usart1_rec_b[20];
	uint8_t Rx_flag;
	uint8_t RX_lengt;
}USART_RECEIVER;

typedef struct{
//	uint8_t usart1_send_dd[21];
	uint8_t usart1_send_b[21];
	uint8_t Tx_tag;
	uint8_t data_load;
	uint8_t send_lengt;
	__IO uint8_t send_interval;
	uint8_t send_event;

}USART_SENDER;

typedef struct{
//	uint8_t usart2_rec_dd[20];
	uint8_t usart2_rec_b[20];
	uint8_t Rx_flag22;
	uint8_t RX_lengt22;
}USART22_RECEIVER;

typedef struct{
//	uint8_t usart2_send_dd[20];
	uint8_t usart2_semd_b[20];
	uint8_t Tx_tag22;
	uint8_t events_flag22;
	uint8_t send_lengt22;
	__IO uint8_t send_interval22;
	uint8_t load_flag22;

}USART22_SENDER;

typedef struct{
	uint8_t app_send_buffer[20];
	//uint8_t app_read_buffer[20];
	uint8_t app_send_size;
	uint8_t app_send_size_back;
}APP_BUFFER;

typedef struct{
	uint8_t usart11_shadow_buffer[21];
	//uint8_t app_read_buffer[20];
	uint8_t shadow_send_size;
	uint8_t shadow_read_size;
}USART11_SHADOW;

extern APP_BUFFER App_Buffer[LOCUS_BUFFER_MAX];
extern USART11_SHADOW Usart11_shadow[3];
extern uint8_t app_read_buffer[USART_TAG];
extern USART22_RECEIVER usart22_rev;
extern USART22_SENDER usart22_send;
extern USART_SENDER usart_send;
extern USART_RECEIVER usart_rev;
extern uint8_t con_buffer[USART_TAG];

#endif
