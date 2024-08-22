#ifndef __UART_H
#define __UART_H

#include "common.h"

#define USART_RDR_ADDR	0x40013824
#define USART_TDR_ADDR	0x40013828
#define USART22_RDR_ADDR	0x40004424
#define USART22_TDR_ADDR	0x40004428

//#define USART_TDR_ADDR	0x40013828
#define USART_TAG	24
#define USART22_TAG	25
#define USART_SEND_TAG	25
#define ANSWER_FALSE 0
#define ANSWER_SUCCESS	0XFF
#define LOCUS_BUFFER_MAX	27

typedef struct{
	uint8_t usart1_rec_dd[20];
	uint8_t usart1_rec_b[USART_TAG];
	uint8_t Rx_flag;
	uint8_t RX_lengt;
}USART_RECEIVER;

typedef struct{
//	uint8_t usart1_send_dd[21];
	uint8_t usart1_send_b[USART_SEND_TAG];
	uint8_t Tx_tag;
	uint8_t data_load;
	uint8_t send_lengt;
	uint16_t send_interval;
	uint8_t send_event;

}USART_SENDER;

typedef struct{
//	uint8_t usart2_rec_dd[20];
	uint8_t usart2_rec_b[USART22_TAG];
	uint8_t Rx_flag22;
	uint8_t RX_lengt22;
}USART22_RECEIVER;

typedef struct{
//	uint8_t usart2_send_dd[20];
	uint8_t usart2_send_b[USART_SEND_TAG];
	uint8_t Tx_tag22;
	uint8_t events_flag22;
	uint8_t send_lengt22;
	uint16_t send_interval22;
	uint8_t load_flag22;

}USART22_SENDER;

typedef struct{
//	uint8_t usart2_rec_dd[20];
	uint8_t usart3_rec_b[20];
	uint8_t Rx_flag33;
	uint8_t RX_lengt33;
}USART33_RECEIVER;

typedef struct{
//	uint8_t usart2_send_dd[20];
	uint8_t usart3_send_b[USART_SEND_TAG];
	uint8_t Tx_tag33;
	uint8_t events_flag33;
	uint8_t send_lengt33;
	uint8_t send_interval33;
	uint8_t load_flag33;

}USART33_SENDER;


typedef struct{
	uint8_t app_send_buffer[USART_SEND_TAG];
	//uint8_t app_read_buffer[20];
	uint8_t app_send_size;
	uint8_t app_read_size;
}APP_BUFFER;

typedef struct{
	uint8_t usart11_shadow_buffer[USART_SEND_TAG];
	//uint8_t app_read_buffer[20];
	uint8_t shadow_send_size;
	uint8_t shadow_read_size;
}USART11_SHADOW;
typedef struct{
	uint8_t Cache_SBf[USART_SEND_TAG];
	uint8_t shift_tag;
	uint8_t ack_OK;
	uint8_t send_times;
}Cache_Buffer;

void uart_int(void);
void print_word_data( uint32_t data, uint8_t p[20]);
uint8_t checksum_verify(uint8_t *spt, uint8_t lenght);
uint8_t return_RX_Complete(void);
void Reset_RX_Complete(void);
uint8_t return_DMA_SendComplete(void);
void Usart22_Reset_RX_Complete(void);
uint8_t Usart22_return_RX_Complete(void);
void Set_Tx_Complete(void);
void Usart11_Send_Handling(uint8_t TX_size, uint8_t *value_da);

void uart_send_data(unsigned short data);
void Usart11_Init(uint32_t baud);
void Usart22_init(uint32_t baud);

// ReadAndSend.c
void app_Receiver(void);
uint8_t Check_Buffer_Empty(USART11_SHADOW *sptt);


extern APP_BUFFER App_Buffer[LOCUS_BUFFER_MAX];
extern USART11_SHADOW Usart11_shadow[LOCUS_BUFFER_MAX];

extern uint8_t app_read_buffer[USART22_TAG];
extern uint8_t con_buffer[USART_SEND_TAG];
extern USART_SENDER usart_send;
extern Cache_Buffer Cache;
extern USART_RECEIVER usart_rev;
extern USART22_SENDER usart22_send;
extern USART22_RECEIVER usart22_rev;


#endif
